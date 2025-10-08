#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next,
                      int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  for (struct header *p = free_list_ptr; p != NULL; p = p->next) {
    if (p->size >= size) {
      return p->id;
    }
  }
  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  uint64_t best_size = UINT64_MAX;

  for (struct header *p = free_list_ptr; p != NULL; p = p->next) {
    if (p->size >= size && p->size < best_size) {
      best_size = p->size;
      best_fit_id = p->id;
    }
  }
  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  uint64_t worst_size = 0;

  for (struct header *p = free_list_ptr; p != NULL; p = p->next) {
    if (p->size >= size && p->size > worst_size) {
      worst_size = p->size;
      worst_fit_id = p->id;
    }
  }
  return worst_fit_id;
}

int main(void) {
  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  free(free_block5);
  free(free_block4);
  free(free_block3);
  free(free_block2);
  free(free_block1);

  return 0;
}
/*
Part2
Function coalesce(free_list_head, newly_freed_block B):
  //Find insertion point
  prev = NULL
  curr = free_list_head
  while curr != NULL and curr < B:
    prev = curr
    curr = curr.next
  //Insert B between prev and curr
  B.next = curr
  if prev == NULL:
    free_list_head = B
  else:
    prev.next = B
  //Try merge with previous neighbor
  if prev != NULL and (address(prev) + prev.size == address(B)):
    prev.size += B.size
    prev.next = B.next
    B = prev

  //Try merge with next neighbor
  next = B.next
  if next != NULL and (address(B) + B.size == address(next)):
    B.size += next.size
    B.next = next.next

  return free_list_head
*/
