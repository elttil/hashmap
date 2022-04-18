//
// Copyright (C) 2022 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: BSD-2-Clause
//
#ifndef HASH_H
#define HASH_H
#include <stddef.h>
#include <stdint.h>

uint32_t hash(uint8_t *data, size_t len);
#endif
