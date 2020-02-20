#include <stdint.h>
#include <stdio.h>
#include <math.h>
uint64_t rdtscp(void)
{
  uint32_t lo, hi;
  __asm__ __volatile__(
      "rdtscp"
      : "=a"(lo), "=d"(hi));
  return (uint64_t)hi << 32 | lo;
}

#define MAX_PRIME 1000000

void test_cpu()
{
  unsigned long long c;
  unsigned long long l, t;
  unsigned long long n = 0;

  uint64_t begin, end;
  int i;

  for (i = 0; i < 10; ++i)
  {
    n = 0;
    begin = rdtscp();
    for (c = 3; c < MAX_PRIME; c++)
    {
      t = sqrt(c);
      for (l = 2; l <= t; l++)
        if (c % l == 0)
          break;
      if (l > t)
        n++;
    }

    end = rdtscp();

    printf("%lu\n", end - begin);
  }
}

int main()
{
  test_cpu();
  return 0;
}
