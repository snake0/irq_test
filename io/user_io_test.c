#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#define __USE_GNU
#include <pthread.h>
#include <sched.h>
#include "../utils.h"

int bind_cpu = 0, cpu_id = 0, file_num = 0;
#define BLOCK_SIZE (1 << 9)
cpu_set_t mask;
char *filename;

void test_io()
{
  FILE *f;
  char read_buf[BLOCK_SIZE];
  char write_buf[BLOCK_SIZE];
  uint64_t begin, end;
  memset(write_buf, 'c', sBLOCK_SIZE);

  if (bind_cpu){
    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0)
    {
      printf("Set thread affinity error!\n");
    }
  }

  if((f=open(filename, O_RDWR | O_RSYNC)) == NULL)
  {
    printf("File open error!\n");
  }

  // test read
  for (int i = 0; i < 10; ++i)
  {
    begin = rdtscp();
    fread(f, read_buf, BLOCK_SIZE);
    end = rdtscp();
    printf("[READ BIND=%d] %llu\n", bind_cpu, end - begin);
  }

  // test read
  for (int i = 0; i < 10; ++i)
  {
    begin = rdtscp();
    fwrite(f, write_buf, BLOCK_SIZE);
    fsync();
    end = rdtscp();
    printf("[READ BIND=%d] %llu\n", bind_cpu, end - begin);
  }

}

int main(int argc, char **argv)
{
  int ret = 0;
  pthread_t pid;

  CPU_ZERO(&mask);
  bind_cpu = atoi(argv[2]);
  filename = argv[1];

  if (bind_cpu)
  {
    cpu_id = atoi(argv[3]);
    CPU_SET(cpu_id, &mask);
     if ((ret = pthread_create(&pid, NULL, (void *)test_io, NULL)) != 0)
    {
      printf("Create thread error!\n");
    }
  }
  else
  {
    test_io();
  }

  return 0;
}