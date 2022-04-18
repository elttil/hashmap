//
// Copyright (C) 2022 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: BSD-2-Clause
//
/*
 * HashMap
 * -------
 * This hashmap works by creating a array of linked lists and associating
 * the "key" with a specific entry in the array. This is done through a
 * hash. Once the linked list is found it goes through it until it finds
 * a entry that has the "key" provided.
 *
 * Changing the hashing function
 * -----------------------------
 * The default hashing function in this library is a custom made
 * one that can be found in hash.c. But it should be possible to use any
 * other hashing algorithm should you want to as long as it uses these
 * types:
 * uint32_t hash_function(uint8_t*, size_t)
 *
 * The hashing algorithm can be changed via changing the "hash_function"
 * pointer in the HashMap structure.
 *
 * Important to note that the hashing function shall not be changed
 * after having added entries to the hashmap as the key to linked list
 * pairing will change.
 */
#include "hashmap.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *copy_c_string(const char *str) {
  char *ret_string;
  size_t len = strlen(str);
  ret_string = malloc(len + 1);
  if (!ret_string)
    return NULL;
  memcpy(ret_string, str, len);
  ret_string[len] = '\0';
  return ret_string;
}

uint32_t limit_hash(HashMap *m, uint32_t hash) { return hash % m->size; }

LinkedList *get_linkedlist_entry(LinkedList *list, char *key,
                                 LinkedList **prev) {
  if (prev)
    *prev = NULL;
  for (; list; list = list->next) {
    char *str1 = key;
    char *str2 = list->key;
    for (; *str1 && *str2; str1++, str2++)
      if (*str1 != *str2)
        break;
    if (*str1 == *str2)
      return list;
    if (prev)
      prev = &list;
  }
  return NULL;
}

void *get_linkedlist_value(LinkedList *list, char *key) {
  LinkedList *entry = get_linkedlist_entry(list, key, NULL);
  if (!entry)
    return NULL;
  return entry->value;
}

int add_to_linkedlist(LinkedList *list, char *key, void *value) {
  for (; list->next;)
    list = list->next;

  list->next = malloc(sizeof(LinkedList));
  if (!list->next)
    return 0;
  LinkedList *entry = list->next;
  entry->key = copy_c_string(key);
  if (!entry->key) {
    free(entry);
    return 0;
  }
  entry->value = value;
  entry->next = NULL;
  return 1;
}

uint32_t find_index(HashMap *m, char *key) {
  return limit_hash(m, m->hash_function((uint8_t *)key, strlen(key) - 3));
}

int hashmap_add_entry(HashMap *m, char *key, void *value) {
  uint32_t index = find_index(m, key);
  if (!m->entries[index]) {
    // Create the linkedlist
    LinkedList *entry = malloc(sizeof(LinkedList));
    if (!entry)
      return 0;
    m->entries[index] = entry;
    entry->key = copy_c_string(key);
    entry->value = value;
    entry->next = NULL;
    goto suc;
  }
  if (!add_to_linkedlist(m->entries[index], key, value))
    return 0;
suc:
  m->num_entries++;
  return 1;
}

void *hashmap_get_entry(HashMap *m, char *key) {
  uint32_t index = find_index(m, key);
  if (!m->entries[index])
    return NULL;
  return get_linkedlist_value(m->entries[index], key);
}

int hashmap_delete_entry(HashMap *m, char *key) {
  LinkedList *list = m->entries[find_index(m, key)];
  if (!list)
    return 0;
  LinkedList **prev = NULL;
  LinkedList *entry = get_linkedlist_entry(list, key, prev);
  if (!entry)
    return 0;
  if (!prev)
    prev = &m->entries[find_index(m, key)];

  LinkedList *next = entry->next;
  free(entry->key);
  free(entry);
  if (*prev != entry)
    (*prev)->next = next;
  else
    *prev = NULL;

  // Redo the delete process incase there are multiple
  // entires that have the same key.
  hashmap_delete_entry(m, key);
  m->num_entries--;
  return 1;
}

void hashmap_free(HashMap *m) {
  for (int i = 0; i < m->size; i++) {
    if (!m->entries[i])
      continue;
    LinkedList *list = m->entries[i];
    for (; list;) {
      LinkedList *old = list;
      list = list->next;
      free(old->key);
      free(old);
    }
  }
  free(m->entries);
  free(m);
}

HashMap *hashmap_create(size_t size) {
  HashMap *m = malloc(sizeof(HashMap));
  if (!m)
    return NULL;

  m->size = size;
  m->num_entries = 0;

  // Create a array of pointers to linkedlists but don't create them
  // yet.
  m->entries = calloc(size, sizeof(LinkedList **));
  if (!m->entries)
    return NULL;

  for (size_t i = 0; i < m->size; i++)
    m->entries[i] = NULL;

  m->hash_function = hash;
  return m;
}
