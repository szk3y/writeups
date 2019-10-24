#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>

#define CODE_ADDR 0x112233440000
#define TABLE_ADDR 0x77777000

size_t kMmapSize = 0x10000;

typedef void (*funcp)(void);
typedef ssize_t (*io_funcp)(void*, size_t);

ssize_t println(void* s, size_t size)
{
  puts(s);
  return 0;
}

ssize_t print_byte(void* c, size_t size)
{
  putchar((char)(size_t)c);
  fflush(stdout);
  return 0;
}

ssize_t read_buf(void* buf, size_t size)
{
  return read(0, buf, size);
}

ssize_t do_ls(void* buf, size_t size)
{
  return puts("'ls' is executed");
}

ssize_t do_cat(void* buf, size_t size)
{
  FILE* fp = fopen(buf, "rb");
  if (fp == NULL) {
    perror("do_cat");
    return -1;
  }
  while (1) {
    int ch = fgetc(fp);
    if (ch == EOF) {
      break;
    }
    putchar(ch);
  }
  fclose(fp);
  return 0;
}

void write_to_mem(char* mem, FILE* fp)
{
  int i = 0;
  while (1) {
    int ch = fgetc(fp);
    if (ch == EOF) {
      return;
    }
    mem[i] = (char)ch;
    i++;
  }
}

int main()
{
  void* p = mmap((void*)CODE_ADDR, kMmapSize, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (p == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }
  if (p != (void*)CODE_ADDR) {
    fprintf(stderr, "failed to allocate memory at %lx\n", CODE_ADDR);
    exit(1);
  }
  FILE* fp = fopen("crux", "rb");
  if (fp == NULL) {
    perror("fopen(crux)");
    exit(1);
  }
  write_to_mem(p, fp);
  fclose(fp);

  fp = fopen("table", "rb");
  if (fp == NULL) {
    perror("fopen(table)");
    exit(1);
  }
  write_to_mem(p+0x55, fp);
  fclose(fp);

  io_funcp* table = mmap((void*)TABLE_ADDR, 0x1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (table != (void*)TABLE_ADDR) {
    fprintf(stderr, "failed to allocate memory at %x\n", TABLE_ADDR);
    exit(1);
  }

  table[0] = &print_byte;
  table[1] = &read_buf;
  table[2] = &println;
  table[3] = &do_ls;
  table[4] = &do_cat;

  (*(funcp)p)();

  munmap(p, kMmapSize);
  munmap(table, 0x1000);
  return 0;
}
