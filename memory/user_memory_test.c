#include "../utils.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static rand_t rand_;

#define TOTAL_SIZE (1 << 26)
#define BLOCK_SIZE (1 << 10)
#define BLOCK_NUM (TOTAL_SIZE / BLOCK_SIZE)
#define OPER_COUNT ((BLOCK_NUM / sizeof(size_t)) << 3)

#define WRITE_DATA 3

static size_t *buffer;

static inline void SIZE_T_WRITE(size_t *pos) { *pos = WRITE_DATA; }

static inline size_t SIZE_T_READ(size_t *pos) { return *pos; }

int test_memory(void) {
  int i, j;
  uint64_t begin, end;

  size_t *pos;  // r/w position
  uint64_t res; // read op result

  rand_init(&rand_);

  buffer = (size_t *)malloc(TOTAL_SIZE);
  if (buffer == NULL) {
    printf("malloc failed.\n");
    return 0;
  }

  // write
  for (i = 0; i < 10; ++i) {
    begin = rdtscp();
    for (j = 0; j < OPER_COUNT; ++j) {
      pos = (size_t *)((void *)buffer + (rand_gen(&rand_) % TOTAL_SIZE));
      SIZE_T_WRITE(pos);
    }
    end = rdtscp();

    printf("%lu\n", end - begin);
  }

  // read
  for (i = 0; i < 10; ++i) {
    begin = rdtscp();
    for (j = 0; j < OPER_COUNT; ++j) {
      pos = (size_t *)((void *)buffer + (rand_gen(&rand_) % TOTAL_SIZE));
      res += SIZE_T_READ(pos);
    }
    end = rdtscp();

    printf("%lu\n", end - begin);
  }

  free(buffer);
  return (int)res;
}
int main() {
  test_memory();
  return 0;
}