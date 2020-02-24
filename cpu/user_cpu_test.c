#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#define __USE_GNU
#include <pthread.h>
#include <sched.h>
#include "../utils.h"

#define MAX_PRIME 1000000

unsigned long cal_sqrt(unsigned long x)
{
  unsigned long op, res, one;
  op = x;
  res = 0;
  one = 1UL << (BITS_PER_LONG - 2);
  while (one > op)
    one >>= 2;

  while (one != 0)
  {
    if (op >= res + one)
    {
      op = op - (res + one);
      res = res + 2 * one;
    }
    res /= 2;
    one /= 4;
  }
  return res;
}

cpu_set_t mask;

void test_prime()
{
  unsigned long long c;
  unsigned long long l, t;
  unsigned long long n = 0;

  uint64_t begin, end;
  int i;
  if (BIND_CPU)
  {
    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0)
    {
      printf("Set thread affinity error!\n");
    }
  }

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

int main(int argc, char **argv)
{
  int ret, i;
  pthread_t pid;

  CPU_ZERO(&mask);

  // Set CPU mask
  if (BIND_CPU)
  {
    CPU_SET(CPUID, &mask);
  }

  // Create a thread
  if ((ret = pthread_create(&pid, NULL, (void *)test_prime, NULL)) != 0)
  {
    printf("Create thread error!\n");
  }

  pthread_join(pid, NULL);
  return 0;
}
