#include <assert.h>
#include <slex.h>

int main() {
  char buf[256];
  uint64_t buf_len = sizeof(buf);
  uint64_t skipped;

  slex l;

  assert(slex_init(&l, "Hello World!"));
  assert(slex_bos(&l));

  assert(slex_scan(&l, "[A-Z]", buf, buf_len));
  assert(strcmp(buf, "H") == 0);
  assert(slex_pos(&l) == 1);

  assert(slex_skip(&l, "[elo]+", &skipped));
  assert(skipped == 4);

  assert(slex_match(&l, " "));

  assert(slex_scan_until(&l, "ld", buf, buf_len));
  assert(strcmp(buf, " Wor") == 0);

  assert(slex_reset(&l));
  assert(slex_skip_until(&l, "!", &skipped));
  assert(skipped == 11);
  assert(slex_match(&l, "!"));

  assert(slex_reset(&l));
  assert(slex_scan_until(&l, "[^a-zA-Z ]", buf, buf_len));
  assert(strcmp(buf, "Hello World") == 0);
  assert(slex_rest_size(&l) == 1);
  assert(!slex_eos(&l));

  assert(slex_skip(&l, "!", &skipped));
  assert(slex_eos(&l));
  assert(slex_rest_size(&l) == 0);

  assert(slex_reset(&l));

  assert(slex_getch(&l) == 'H');
  assert(slex_peek(&l, 4, buf, buf_len));
  assert(strcmp(buf, "ello") == 0);
  assert(slex_exist(&l, "!"));
  assert(slex_skip(&l, "ello", NULL));
  assert(slex_rest(&l, buf, buf_len));
  assert(strcmp(buf, " World!") == 0);

  return 0;
}
