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
bool slex_scan_until(slex *l, const char *pattern, char *buf, uint64_t buf_size);

bool slex_skip(slex *l, const char *pattern, uint64_t *skipped);
bool slex_skip_until(slex *l, const char *pattern, uint64_t *skipped);

bool slex_match(slex *l, const char *pattern);

char slex_getch(slex *l);
bool slex_rest(slex *l, char *buf, uint64_t buf_size);
bool slex_peek(slex *l, uint64_t len, char *buf, uint64_t buf_size);
bool slex_exist(slex *l, const char *pattern);

#define slex_str(l) ((l)->str)
#define slex_pos(l) ((l)->pos)
#define slex_len(l) ((l)->len)

#define slex_bos(l) ((l)->pos == 0)
#define slex_eos(l) ((l)->pos >= (l)->len)

#define slex_rest_size(l) ((l)->len - (l)->pos)

#endif
