#define _GNU_SOURCE
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 256
#endif

struct header {
  uint64_t size;
  struct header *next;
};

static void handle_error(const char *what) {
  perror(what);
  _exit(1);
}

static void print_out(const char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len =
      snprintf(buf, BUF_SIZE, format,
               (data_size == sizeof(uint64_t)) ? *(uint64_t *)data
                                               : (uintptr_t)(*(void **)data));
  if (len < 0)
    handle_error("snprintf");
  if (write(STDOUT_FILENO, buf, (size_t)len) < 0)
    handle_error("write");
}

int main(void) {
  void *base = sbrk(256);
  if (base == (void *)-1)
    handle_error("sbrk");

  const uint64_t BLOCK_SIZE = 128;
  struct header *first = (struct header *)base;
  struct header *second = (struct header *)((char *)base + BLOCK_SIZE);

  first->size = BLOCK_SIZE;
  first->next = NULL;

  second->size = BLOCK_SIZE;
  second->next = first;

  size_t header_sz = sizeof(struct header);
  if (header_sz > BLOCK_SIZE) {
    handle_error("header larger than block");
  }
  size_t data_sz = (size_t)(BLOCK_SIZE - header_sz);

  uint8_t *first_data = (uint8_t *)first + header_sz;
  uint8_t *second_data = (uint8_t *)second + header_sz;

  for (size_t i = 0; i < data_sz; ++i)
    first_data[i] = 0;
  for (size_t i = 0; i < data_sz; ++i)
    second_data[i] = 1;

  print_out("first block:       %p\n", (void *)&first, sizeof(first));
  print_out("second block:      %p\n", (void *)&second, sizeof(second));

  print_out("first block size:  %lu\n", (void *)&first->size,
            sizeof(first->size));
  print_out("first block next:  %p\n", (void *)&first->next,
            sizeof(first->next));
  print_out("second block size: %lu\n", (void *)&second->size,
            sizeof(second->size));
  print_out("second block next: %p\n", (void *)&second->next,
            sizeof(second->next));

  const char zero_line[] = "0\n";
  const char one_line[] = "1\n";

  for (size_t i = 0; i < data_sz; ++i)
    if (write(STDOUT_FILENO, zero_line, sizeof(zero_line) - 1) < 0)
      handle_error("write");

  for (size_t i = 0; i < data_sz; ++i)
    if (write(STDOUT_FILENO, one_line, sizeof(one_line) - 1) < 0)
      handle_error("write");

  return 0;
}
