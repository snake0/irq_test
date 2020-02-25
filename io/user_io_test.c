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

int bind_cpu = 0, cpu_id = 0, file_num = 0;
#define BLOCK_SIZE (1 << 8)
cpu_set_t mask;
char *filename;

void test_io()
{
  int ret;
  int fd;
  char read_buf[BLOCK_SIZE];
  char write_buf[BLOCK_SIZE];
  uint64_t begin, end;
  off_t pos;
  memset(write_buf, 'c', BLOCK_SIZE);

  if (bind_cpu)
  {
    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0)
    {
      printf("Set thread affinity error!\n");
    }
  }

  // test read
  for (int i = 0; i < 10; ++i)
  {
    if ((fd = open(filename, O_RDONLY | O_SYNC)) == -1)
    {
      printf("File open error!\n");
    }
    begin = rdtscp();
    for (int j = 0; j < 100; ++j)
      ret = pread(fd, read_buf, BLOCK_SIZE, 0);
    end = rdtscp();
    printf("[READ BIND=%d] %lu\n", bind_cpu, end - begin);
    close(fd);
  }

  // test sequential write
  pos = 0;
  for (int i = 0; i < 10; ++i)
  {
    // Open file
    if ((fd = open("./temp.txt", O_WRONLY | O_TRUNC | O_SYNC, S_IRWXG)) == -1)
    {
      printf("File open error!\n");
    }

    begin = rdtscp();
    for (int j = 0; j < 100; ++j)
    {
      ret = pwrite(fd, write_buf, BLOCK_SIZE, pos);
      pos + BLOCK_SIZE;
    }
    end = rdtscp();

    printf("[WRITE BIND=%d] %lu\n", bind_cpu, end - begin);
    close(fd);
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
    pthread_join(pid, NULL);
  }
  else
  {
    test_io();
  }

  return 0;
}
