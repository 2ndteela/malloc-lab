/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
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
    "Kolberto Knottingham",
    /* Second member's email address (leave blank if none) */
    "kolbytn@gmail.com"
};

typedef long unsigned int mem_addr;

typedef struct {
    unsigned short prevSize;
    unsigned short prevAlloc;
    unsigned short size;
    unsigned short alloc;
} block_hdr;

mem_addr heapTop = 0;

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define HEADER_SIZE 8

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    block_hdr first;
    block_hdr end;
    first.prevSize = 0;
    first.prevAlloc = 1;
    first.size = 500;
    first.alloc = 0;
    end.prevSize = 500;
    end.prevAlloc = 0;
    end.size = 0;
    end.alloc = 1;
    return 0;
}

/*
 * mm_malloc - Look for best fit block, adn allocate more memory if does not exist
 * Note: I havn't tested this yet and I'm not familiar enough w/ C to know if it will run
 */
void *mm_malloc(size_t size)
{
  block_hdr* prev;
  block_hdr* curr = mem_heap_lo();
  block_hdr* best = NULL;
  unsigned short first = 0;

  size = ALIGN(size);
  while(curr->size != 0) {                         //Traverse entire heap
    if(curr->size > size && curr->alloc == 0) {
      if(first == 0){                              //identify first fit
        first = curr->size;
        best = curr;
      }
      else if(curr->size < best->size)             //identify best fit
        best = curr;
    }
    prev = curr;
    curr += curr->size;
  }

  if(best != NULL) {
    if(best->size - size >= 16){                    //if left over block is >= 16
      prev = best;                                  //split block
      curr = best + size + HEADER_SIZE;
      curr->prevSize = prev->size;
      curr->prevAlloc = 1;
      curr->size = prev->size - size + HEADER_SIZE;
      curr->alloc = 0;
    }
    curr = best;                                     //select best fit
    prev = best - best->prevSize;
  }
  else                                               //if no best fit select end header
    mem_sbrk(size + HEADER_SIZE);                    //and increase heap size

  curr->prevSize = prev->size;                       //initialize new block
  curr->prevAlloc = 1;
  curr->size = size + HEADER_SIZE;
  curr->alloc = 1;
}

  /*
  int newsize = ALIGN(size + SIZE_T_SIZE);
  void *p = mem_sbrk(newsize);
  if (p == (void *)-1)
return NULL;
  else {
      *(size_t *)p = size;
      return (void *)((char *)p + SIZE_T_SIZE);
  }*/

/*
 * Checks whether the next and previous blocks are free and combines them by
 * adjusting the size of the blocks. Then sets the alloc byte
 *  Note: I havn't tested this yet and I'm not familiar enough w/ C to know if it will run
 */
void mm_free(void *ptr)
{
  block_hdr* curr = ptr;
  if(!(curr + curr->size)->alloc)             //if next is free
    curr->size += (curr + curr->size)->size;   //add next size to current size
  if(!curr->prevAlloc) {                     //if previous is free
    curr -= curr->prevSize;                   //point at previous
    curr->size += (curr + curr->size)->size;   //add next size to current size
  }
  curr->alloc = 0;                           //set current to free
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;

    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}
