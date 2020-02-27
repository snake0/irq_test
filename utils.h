#ifndef _UTILS_H_
#define _UTILS_H_

const char *tf[2] = {"false", "true"};

#define BITS_PER_LONG 64

#define DISABLE_IRQ 1
#define BIND_CPU 0

#define CPUID 3

typedef long unsigned int size_t;

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

static inline void mem_flush(const void *p, unsigned int allocation_size)
{
  const size_t cache_line = 64;
  const char *cp = (const char *)p;
  size_t i = 0;

  if (!p || allocation_size <= 0)
    return;

  for (i = 0; i < allocation_size; i += cache_line)
  {
    asm volatile("clflush (%0)\n\t"
                 :
                 : "r"(&cp[i])
                 : "memory");
  }

  asm volatile("sfence\n\t"
               :
               :
               : "memory");
}

#endif
