#include "slex.h"

static uint64_t regex_match(const char *pattern, const char *str) {
  char err[128];
  regex_t regex;
  int status = regcomp(&regex, pattern, REG_EXTENDED);
  if(status != 0) {
    memset(err, 0, sizeof(err));
    regerror(status, &regex, err, sizeof(err));
    fprintf(stderr, "%s", err);
    return 0;
  }

  regmatch_t match;
  status = regexec(&regex, str, 1, &match, 0);
  if(status == 0) {
    return match.rm_eo - match.rm_so;  
  } else if(status != REG_NOMATCH) {
    memset(err, 0, sizeof(err));
    regerror(status, &regex, err, sizeof(err));
    fprintf(stderr, "%s\n", err);
  }

  return 0;
}

bool slex_init(slex *l, const char *str) {
  if(l == NULL || str == NULL) {
    return false;
  }

  l->str = str;
  l->len = strlen(str);
  l->pos = 0;

  return true;
}

bool slex_reset(slex *l) {
  if(l == NULL) {
    return false;
  }

  l->pos = 0;
  return true;
}

bool slex_scan(slex *l, const char *pattern, char *buf, uint64_t buf_size) {
  uint64_t match_len = regex_match(pattern, (l->str + l->pos));
  if(match_len > 0) {
    if(match_len >= buf_size) {
      fprintf(stderr, "slex_scan: match is larger than buffer\n");
      return false;
    }

    memcpy(buf, (l->str + l->pos), match_len);
    buf[match_len] = 0;
    l->pos += match_len;    

    return true;
  }
  return false;
}

bool slex_skip(slex *l, const char *pattern, uint64_t *skipped) {
  uint64_t match_len = regex_match(pattern, (l->str + l->pos));
  if(match_len > 0) {
    *skipped = match_len;
    l->pos += match_len;
    return true;
  }
  *skipped = 0;
  return false;
}

bool slex_match(slex *l, const char *pattern) {
  uint64_t skipped;
  if(slex_skip(l, pattern, &skipped)) {
    l->pos -= skipped;
    return true;
  }
  return false;
}
