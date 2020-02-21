#ifndef _UTILS_H_
#define _UTILS_H_

const char *tf[2] = {"false", "true"};

#define BITS_PER_LONG 64

#define DISABLE_IRQ 1
#define BIND_CPU 0

#define CPUID 3

static inline uint64_t rdtscp(void)
{
  uint32_t lo, hi;
  __asm__ __volatile__(
      "rdtscp"
      : "=a"(lo), "=d"(hi));
  return (uint64_t)hi << 32 | lo;
}

typedef struct _rand {
  uint64_t a, b;
} rand_t;

static inline void rand_init(rand_t *rand, long seed) {
  rand->a = rdtscp() - seed;
  rand->b = seed;
}

static uint64_t inline rand_gen(rand_t *rand) {
  uint64_t t = rand->a;
  uint64_t const s = rand->b;
  rand->a = s;
  t ^= t << 23;        
  t ^= t >> 17;        
  t ^= s ^ (s >> 26);  
  rand->b = t;
  return t + s;
}

#endif