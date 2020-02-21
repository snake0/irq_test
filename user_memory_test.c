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

void test_memory()
{
  
}

int main()
{
  test_memory();
  return 0;
}