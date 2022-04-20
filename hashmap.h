//
// Copyright (C) 2022 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: BSD-2-Clause
//
#ifndef HASHMAP_H
#define HASHMAP_H
#include <stddef.h>
#include <stdint.h>

typedef struct LinkedList {
  char *key;
  void *value;
  void (*upon_deletion)(char *, void *);
  struct LinkedList *next;
} LinkedList;

typedef struct HashMap {
  LinkedList **entries;
  size_t size;
  size_t num_entries;
  uint32_t (*hash_function)(uint8_t *data, size_t len);
} HashMap;

HashMap *hashmap_create(size_t size);
void hashmap_free(HashMap *m);

// hashmap_add_entry()
// -------------------
// This function adds a entry to the hashmap. The "key" passed to the
// hashmap will be by default allocated before adding it. This will not be
// done with the "value" argument. If this allocation is undesirable you
// can use `hashmap_add_entry_no_alloc_key()` that does not allocated the
// "key" passed to it.
int hashmap_add_entry(HashMap *m, char *key, void *value,
                      void (*upon_deletion)(char *, void *));

// hashmap_add_entry_no_alloc_key()
// --------------------------------
// Equivalent to hashmap_add_entry() except that the "key" argument does
// not get allocated to the heap.
int hashmap_add_entry_no_alloc_key(HashMap *m, char *key, void *value,
                                   void (*upon_deletion)(char *, void *));
void *hashmap_get_entry(HashMap *m, char *key);
int hashmap_delete_entry(HashMap *m, char *key);
#endif
