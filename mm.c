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
    "Kolby Nottingham",
    /* Second member's email address (leave blank if none) */
    "kolbytn@gmail.com"
};

typedef struct {
    unsigned int size;
    unsigned int alloc;
} block_hdr;

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define HEADER_SIZE 8

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
  mem_sbrk((2 << 10) + (HEADER_SIZE * 2));

  block_hdr* first = mem_heap_lo();
  first->size = (2 << 10) + HEADER_SIZE;
  first->alloc = 0;

  block_hdr* end = first + (first->size / 8);
  end->size = 0;
  end->alloc = 1;

  return 0;
}

/*
 * mm_malloc - Look for best fit block, adn allocate more memory if does not exist
 * Note: I havn't tested this yet and I'm not familiar enough w/ C to know if it will run
 */
void *mm_malloc(size_t size) {
  size = ALIGN(size);
  block_hdr* curr = mem_heap_lo();

  while (curr->size > 0) {
    if(curr->size > size && curr->alloc == 0) {
      break;
    }
    curr += curr->size / 8;
  }

  if(curr->size == 0) {
    mem_sbrk(size + HEADER_SIZE);
    curr += (size + HEADER_SIZE) / 8;
    curr->size = 0;
    curr->alloc = 1;
    curr -= (size + HEADER_SIZE) / 8;
    curr->size = size + HEADER_SIZE;
  }
  curr->alloc = 1;
  return curr + 1;
}

/*
 * Checks whether the next and previous blocks are free and combines them by
 * adjusting the size of the blocks. Then sets the alloc byte
 *  Note: I havn't tested this yet and I'm not familiar enough w/ C to know if it will run
 */
void mm_free(void *ptr)
{
   block_hdr* curr = ptr;
   curr->alloc = 0;
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
