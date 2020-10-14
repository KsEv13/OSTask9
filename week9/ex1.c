#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int memory = 0, rpages = 0, apages = 0;
struct inpage{
    int page_num; uint8_t age;
};
struct inpage *pages = NULL;

void update_ages(void) {
  for (int i=0;i<rpages && i<memory;i++) {
    pages[i].age >>= 1;
  }
  for (int i=memory;i < rpages;i++) {
    pages[i].age=0x80|(pages[i].age>>1);
  }
}

int page_index(size_t page) {
    for (int i=0;i<rpages;i++) {
      if (page==pages[i].page_num) {
        return i;
      }
    }
    if (rpages >= apages) {
      pages = (struct inpage *)realloc(pages, (apages*=2)*sizeof(struct 
inpage));
    }
    pages[rpages].page_num = page;
    pages[rpages].age = 0;
    return rpages++;
}

int page_request(size_t page) {
  int output = 1;
  int idx = page_index(page);
  if (idx >= memory) {
    int idx_min = 0;
    for (int i=1;i<memory;i++) {
      if (pages[i].age < pages[idx_min].age) {
        idx_min = i;
      }
    }
    struct inpage p = pages[idx_min];
    pages[idx_min] = pages[idx];
    pages[idx] = p;
    output = 0;
  }
  update_ages();
  return output;
}

int main(void) {
  int page, hits = 0,misses = 0;
  FILE *myfile = fopen("input.txt", "r");

  scanf("%d", &memory);
  apages = memory;
  pages = (struct inpage *)malloc(apages*sizeof(struct inpage));
  rpages = 0;
  while (fscanf(myfile,"%d",&page) == 1) {
    if (!page_request(page)) {
        misses++;
    }
    else {
        hits++;
    }
  }
  printf("The number of hits:%d\tThe number of misses:%d\n", hits, 
misses);
  return 0;
}

