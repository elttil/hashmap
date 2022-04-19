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
int hashmap_add_entry(HashMap *m, char *key, void *value,
                      void (*upon_deletion)(char *, void *));
void *hashmap_get_entry(HashMap *m, char *key);
int hashmap_delete_entry(HashMap *m, char *key);
#endif
