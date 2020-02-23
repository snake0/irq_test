#ifndef _UTILS_H_
#define _UTILS_H_

const char *tf[2] = {"false", "true"};

#define BITS_PER_LONG 64

#define DISABLE_IRQ 0
#define BIND_CPU 0

#define CPUID 3

static inline unsigned long long rdtscp(void)
{
  unsigned long long lo, hi;
  __asm__ __volatile__(
      "rdtscp"
      : "=a"(lo), "=d"(hi));
  return (unsigned long long)hi << 32 | lo;
}

typedef struct _rand {
  unsigned long long a, b;
} rand_t;

static inline void rand_init(rand_t *rand) {
  rand->a = rdtscp() - 1283;
  rand->b = 1276;
}

static unsigned long long inline rand_gen(rand_t *rand) {
  unsigned long long t = rand->a;
  unsigned long long const s = rand->b;
  rand->a = s;
  t ^= t << 23;        
  t ^= t >> 17;        
  t ^= s ^ (s >> 26);  
  rand->b = t;
  return t + s;
}

#endif
