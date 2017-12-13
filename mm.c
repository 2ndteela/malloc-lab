/*
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
    unsigned int this_size;
    unsigned int prev_size;
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
    const int INITAIL_VAL = 2 << 10;
    block_hdr* first;
    first.this_size -> INITAIL_VAL;
    first.prev_size -> NULL;
    
    mem_addr* next_block = first++;
    mem_addr* last_block = next_block++;
    next_block -> NULL;
    last_block -> NULL; 

    heapTop = INITAIL_VAL;
    return 0;
}

/*
 * mm_malloc - Look for best fit block, adn allocate more memory if does not exist
 * Note: I havn't tested this yet and I'm not familiar enough w/ C to know if it will run
 */
void *mm_malloc(size_t size)
{
  
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
