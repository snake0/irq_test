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

#define BITS_PER_LONG 64

unsigned long cal_sqrt(unsigned long x)
{
	unsigned long op, res, one;
	op = x;
	res = 0;
	one = 1UL << (BITS_PER_LONG - 2);
	while (one > op)
		one >>= 2;

	while (one != 0) {
		if (op >= res + one) {
			op = op - (res + one);
			res = res +  2 * one;
		}
		res /= 2;
		one /= 4;
	}
	return res;
}

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
      t = cal_sqrt(c);
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
