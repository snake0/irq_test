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

#define BLOCK_SIZE (1 << 8)
#define OPER_COUNT 1000

cpu_set_t mask;
char *filename = "/home/snake0/test.0";

void test_io()
{
  int ret;
  int fd;
  char read_buf[BLOCK_SIZE];
  char write_buf[BLOCK_SIZE];
  uint64_t begin, end;
  off_t pos;
  memset(write_buf, 'c', BLOCK_SIZE);

  if (BIND_CPU)
  {
    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0)
    {
      printf("Set thread affinity error!\n");
    }
  }

  // init file for read
  if ((fd = open(filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR)) == -1)
  {
    printf("File prepare error! exit.\n");
    return;
  }

  ret = write(fd, write_buf, BLOCK_SIZE);
  ret = write(fd, write_buf, BLOCK_SIZE);
  close(fd);

  // test read
  for (int i = 0; i < 10; ++i)
  {
    if ((fd = open(filename, O_RDONLY | O_SYNC)) == -1)
    {
      printf("File open error! exit.\n");
      return;
    }
    begin = rdtscp();
    for (int j = 0; j < OPER_COUNT; ++j)
      ret = pread(fd, read_buf, BLOCK_SIZE, 0);
    end = rdtscp();
    printf("[READ BIND=%d] %lu\n", BIND_CPU, end - begin);
    close(fd);
  }

  // test sequential write
  pos = 0;
  for (int i = 0; i < 10; ++i)
  {
    // Open file
    if ((fd = open(filename, O_WRONLY | O_TRUNC | O_SYNC, S_IRWXG)) == -1)
    {
      printf("File open error! exit.\n");
      return;
    }

    begin = rdtscp();
    for (int j = 0; j < OPER_COUNT >> 4; ++j)
    {
      ret = pwrite(fd, write_buf, BLOCK_SIZE, pos);
      pos + BLOCK_SIZE;
    }
    end = rdtscp();

    printf("[WRITE BIND=%d] %lu\n", BIND_CPU, end - begin);
    close(fd);
  }
  unlink("test.0");
}

int main(int argc, char **argv)
{
  int ret = 0;
  pthread_t pid;

  CPU_ZERO(&mask);
  // BIND_CPU = atoi(argv[2]);
  // filename = argv[1];

  if (BIND_CPU)
  {
    CPU_SET(CPUID, &mask);
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
