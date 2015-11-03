#include "slex.h"

static char *prepend_caret(const char *str) {
  uint64_t str_len;
  char *prepended;

  str_len = strlen(str);
  prepended = malloc(str_len + 2);
  memcpy(prepended + 1, str, str_len);
  prepended[0] = '^';
  prepended[str_len + 1] = 0;

  return prepended;
}

static bool regex_match(const char *pattern, const char *str, regmatch_t *match) {
  char err[128];
  regex_t regex;
  int status;

  status = regcomp(&regex, pattern, REG_EXTENDED);
  if(status != 0) {
    memset(err, 0, sizeof(err));
    regerror(status, &regex, err, sizeof(err));
    fprintf(stderr, "%s", err);
    return false;
  }

  if(match != NULL) {
    status = regexec(&regex, str, 1, match, 0);
  } else {
    status = regexec(&regex, str, 0, NULL, 0);
  }

  if(status == 0) {
    return true;
  } else if(status != REG_NOMATCH) {
    memset(err, 0, sizeof(err));
    regerror(status, &regex, err, sizeof(err));
    fprintf(stderr, "%s\n", err);
  }

  return false;
}

static bool regex_match_bol(const char *pattern, const char *str, regmatch_t *match) {
  char *pattern_bol;
  bool matched;

  pattern_bol = prepend_caret(pattern);
  matched = regex_match(pattern_bol, str, match);
  free(pattern_bol);

  return matched;
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
  regmatch_t match;
  uint64_t match_len;

  if(regex_match_bol(pattern, (l->str + l->pos), &match)) {
    match_len = match.rm_eo - match.rm_so;

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
  }
  return false;
}

bool slex_skip(slex *l, const char *pattern, uint64_t *skipped) {
  regmatch_t match;
  uint64_t match_len;

  if(regex_match_bol(pattern, (l->str + l->pos), &match)) {
    match_len = match.rm_eo - match.rm_so;
    if(skipped != NULL) {
      *skipped = match_len;
    }
    l->pos += match_len;
    return true;
  }

  if(skipped != NULL) {
    *skipped = 0;
  }
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

bool slex_scan_until(slex *l, const char *pattern, char *buf, uint64_t buf_size) {
  regmatch_t match;
  uint64_t match_len;

  if(regex_match(pattern, (l->str + l->pos), &match)) {
    match_len = match.rm_so;

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
  }
  return false;
}

bool slex_skip_until(slex *l, const char *pattern, uint64_t *skipped) {
  regmatch_t match;
  uint64_t match_len;

  if(regex_match(pattern, (l->str + l->pos), &match)) {
    match_len = match.rm_so;
    *skipped = match_len;
    l->pos += match_len;
    return true;
  }

  *skipped = 0;
  return false;
}

char slex_getch(slex *l) {
  return l->str[l->pos++];
}

bool slex_rest(slex *l, char *buf, uint64_t buf_size) {
  uint64_t rest_size;

  rest_size = slex_rest_size(l);

  if(rest_size == 0) {
    return false;
  }

  if(rest_size > buf_size) {
    fprintf(stderr, "slex_rest: rest size is larger than buffer\n");
    return false;
  }

  memcpy(buf, (l->str + l->pos), rest_size);
  buf[rest_size] = 0;
  return true;
}

bool slex_peek(slex *l, uint64_t len, char *buf, uint64_t buf_size) {
  if(len >= slex_rest_size(l) || len >= buf_size) {
    return false;
  }
  memcpy(buf, (l->str + l->pos), len);
  buf[len] = 0;
  return true;
}

bool slex_exist(slex *l, const char *pattern) {
  return regex_match(pattern, (l->str + l->pos), NULL);
}
