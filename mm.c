/*
 * This malloc function uses immediate coalescing and first fit to free and allocate memory
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Os caras",
    /* First member's full name */
    "Jeremy Teela",
    /* First member's email address */
    "2ndteela@gmail.com",
    /* Second member's full name (leave blank if none) */
    "Kolby Nottingham",
    /* Second member's email address (leave blank if none) */
    "kolbytn@gmail.com"
};

typedef struct {
    unsigned int size;
    unsigned int prev_size;
} block_hdr;

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define HEADER_SIZE 8

#define GET(p)       (*(unsigned int *)(p))            //line:vm:mm:get
#define PUT(p, val)  (*(unsigned int *)(p) = (val))    //line:vm:mm:put

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)                   //line:vm:mm:getsize
#define GET_ALLOC(p) (GET(p) & 0x1)      

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
  mem_sbrk((2 << 10) + (HEADER_SIZE * 2));

  block_hdr* first = mem_heap_lo();
  first->size = (2 << 10) + HEADER_SIZE;
  first->prev_size = 1;

  block_hdr* end = first + (first->size / 8);
  end->size = 1;
  end->prev_size = first->size;

  return 0;
}

/*
 * Allocates first fit or expands heap and returns pointer
 */
void *mm_malloc(size_t size) {
  size = ALIGN(size);
  block_hdr* curr = mem_heap_lo();
  block_hdr* prev;
  block_hdr* bestFit = 0;

  while (curr->size > 1) {
    if(curr->size > size && curr->size % 2 == 0) {
      if(bestFit == 0) {
	 bestFit = curr;
	}
      else if (bestFit -> size > curr -> size) {
	 bestFit = curr;
	}
    }
    prev = curr;
    curr += curr->size / 8;
  }

  if(bestFit == 0) {
    mem_sbrk(size + HEADER_SIZE);
    curr->size = size + HEADER_SIZE + 1;
    prev = curr;
    curr += curr->size / 8;
    curr->size = 1;
    curr->prev_size = prev->size;
  }
  else {
    bestFit->size += 1;
    prev = bestFit;
    bestFit += bestFit->size / 8;
    bestFit->prev_size = prev->size;
  }
  return prev + 1;
}

/*
 * Frees block and attempts to Coalesce with adjacent blocks
 */
void mm_free(void *ptr)
{
   block_hdr* curr = ptr - 8;
   if(curr->size % 2 == 1)
      curr->size -= 1;
   if((curr + (curr->size / 8))->size % 2 == 0) {
      curr->size = curr->size + (curr + (curr->size / 8))->size;
   }
   if(curr->prev_size % 2 == 0) {
      (curr - (curr->prev_size / 8))->size += curr->size;
   }
   (curr + (curr->size / 8))->prev_size = curr->size;
}

/*
 * Implemented using mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{

    size_t oldSize;
    void *newptr;

    if(size == 0) {
      mm_free(ptr);
      return 0;
    } 

    if(ptr == NULL) {
      return mm_malloc(size);
    }

    newptr = mm_malloc(size);

    oldSize = size + 8;
    if(size < oldSize) oldSize = size;
    memcpy(newptr, ptr, oldSize);
    mm_free(ptr);

    return newptr;
}
