#include "../utils.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static rand_t rand_;

#define TOTAL_SIZE (1 << 20)
#define BLOCK_SIZE (1 << 10)
#define BLOCK_NUM (TOTAL_SIZE / BLOCK_SIZE)
#define OPER_COUNT ((BLOCK_NUM / sizeof(size_t)) << 20)

#define WRITE_DATA 3

#define IF_WRITE 0

static size_t *buffer;

static inline void SIZE_T_STORE(size_t *pos, size_t data)
{
  asm volatile("movq %1, (%0);"
               :
               : "r"(pos),
                 "r"(data)
               : "memory");
}

static inline size_t SIZE_T_LOAD(size_t *pos)
{
  size_t res;
  asm volatile("movq (%1), %0"
               : "=r"(res)
               : "r"(pos)
               : "memory");
  return res;
}
int test_memory(void)
{
  int i, j;
  uint64_t begin, end;

  size_t *pos;  // r/w position
  uint64_t res; // read op result

  size_t *offset; // start position for every operation

  rand_init(&rand_);

  buffer = (size_t *)malloc(TOTAL_SIZE);
  if (buffer == NULL)
  {
    printf("malloc failed.\n");
    return 0;
  }

  if (IF_WRITE)
  {
    // write
    for (i = 0; i < 10; ++i)
    {
      offset = (size_t *)((void *)buffer + BLOCK_SIZE * (rand_gen(&rand_) % BLOCK_NUM));
      begin = rdtscp();
      for (j = 0; j < OPER_COUNT; ++j)
      {
        pos = (size_t *)((void *)offset + (rand_gen(&rand_) % BLOCK_SIZE));
        SIZE_T_STORE(pos, j);
      }
      end = rdtscp();

      printf("%lu\n", end - begin);
    }
  }
  else
  {
    // read
    for (i = 0; i < 10; ++i)
    {
      offset = (size_t *)((void *)buffer + BLOCK_SIZE * (rand_gen(&rand_) % BLOCK_NUM));
      begin = rdtscp();
      for (j = 0; j < OPER_COUNT; ++j)
      {
        pos = (size_t *)((void *)offset + (rand_gen(&rand_) % BLOCK_SIZE));
        res += SIZE_T_LOAD(pos);
      }
      end = rdtscp();

      printf("%lu\n", end - begin);
    }
  }

  free(buffer);
  return (int)res;
}
int main()
{
  test_memory();
  return 0;
}
