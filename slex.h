#ifndef __SLEX_H__
#define __SLEX_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

typedef struct {
  const char *str;
  uint64_t len;
  uint64_t pos;
} slex;

bool slex_init(slex *l, const char *str);
bool slex_reset(slex *l);

bool slex_scan(slex *l, const char *pattern, char *buf, uint64_t buf_size);
bool slex_skip(slex *l, const char *pattern, uint64_t *skipped);
bool slex_match(slex *l, const char *pattern);

#endif
