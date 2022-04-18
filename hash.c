//
// Copyright (C) 2022 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: BSD-2-Clause
//
/*
 * This is a very very very basic hash function that has not been tested
 * for speed, collisions and certainly not security.
 *
 * But it seems to work fine enough for a hashmap.
 */
#include "hash.h"

#include <string.h>

// The first bytes taken from SHA256("hashfunction")
#define CONSTANT 0x031b5515

uint32_t mix(uint32_t x) {
  x ^= CONSTANT;
  x ^= (x << 13);
  x ^= (x >> 7);
  x ^= (x << 17);
  return x;
}

uint32_t hash(uint8_t *data, size_t len) {
  uint32_t hash = 0;
  for (; len;) {
    uint32_t value = 0;
    uint8_t read_len = (sizeof(uint32_t) < len) ? sizeof(uint32_t) : len;
    memcpy(&value, data, read_len);
    hash ^= mix(value);
    data += read_len;
    len -= read_len;
  }
  return hash;
}
