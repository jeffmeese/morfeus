#include "pool.h"
#include "mem.h"

#include <stdlib.h>

/*****************************************************************************/
/*                                                                           */
/*  poolzero()   Set all of a pool's fields to zero.                         */
/*                                                                           */
/*  This procedure should never be called on a pool that has any memory      */
/*  allocated to it, as that memory would leak.                              */
/*                                                                           */
/*****************************************************************************/

void poolzero(struct memorypool *pool)
{
  pool->firstblock = (void **) NULL;
  pool->nowblock = (void **) NULL;
  pool->nextitem = (void *) NULL;
  pool->deaditemstack = (void *) NULL;
  pool->pathblock = (void **) NULL;
  pool->pathitem = (void *) NULL;
  pool->alignbytes = 0;
  pool->itembytes = 0;
  pool->itemsperblock = 0;
  pool->itemsfirstblock = 0;
  pool->items = 0;
  pool->maxitems = 0;
  pool->unallocateditems = 0;
  pool->pathitemsleft = 0;
}

/*****************************************************************************/
/*                                                                           */
/*  poolrestart()   Deallocate all items in a pool.                          */
/*                                                                           */
/*  The pool is returned to its starting state, except that no memory is     */
/*  freed to the operating system.  Rather, the previously allocated blocks  */
/*  are ready to be reused.                                                  */
/*                                                                           */
/*****************************************************************************/

void poolrestart(struct memorypool *pool)
{
  unsigned ULONGLONG alignptr;

  pool->items = 0;
  pool->maxitems = 0;

  /* Set the currently active block. */
  pool->nowblock = pool->firstblock;
  /* Find the first item in the pool.  Increment by the size of (void *). */
  alignptr = (unsigned ULONGLONG) (pool->nowblock + 1);
  /* Align the item on an `alignbytes'-byte boundary. */
  pool->nextitem = (void *)
    (alignptr + (unsigned ULONGLONG) pool->alignbytes -
     (alignptr % (unsigned ULONGLONG) pool->alignbytes));
  /* There are lots of unallocated items left in this block. */
  pool->unallocateditems = pool->itemsfirstblock;
  /* The stack of deallocated items is empty. */
  pool->deaditemstack = (void *) NULL;
}

/*****************************************************************************/
/*                                                                           */
/*  poolinit()   Initialize a pool of memory for allocation of items.        */
/*                                                                           */
/*  This routine initializes the machinery for allocating items.  A `pool'   */
/*  is created whose records have size at least `bytecount'.  Items will be  */
/*  allocated in `itemcount'-item blocks.  Each item is assumed to be a      */
/*  collection of words, and either pointers or floating-point values are    */
/*  assumed to be the "primary" word type.  (The "primary" word type is used */
/*  to determine alignment of items.)  If `alignment' isn't zero, all items  */
/*  will be `alignment'-byte aligned in memory.  `alignment' must be either  */
/*  a multiple or a factor of the primary word size; powers of two are safe. */
/*  `alignment' is normally used to create a few unused bits at the bottom   */
/*  of each item's pointer, in which information may be stored.              */
/*                                                                           */
/*  Don't change this routine unless you understand it.                      */
/*                                                                           */
/*****************************************************************************/

void poolinit(struct memorypool *pool, int bytecount, int itemcount, int firstitemcount, int alignment)
{
  /* Find the proper alignment, which must be at least as large as:   */
  /*   - The parameter `alignment'.                                   */
  /*   - sizeof(void *), so the stack of dead items can be maintained */
  /*       without unaligned accesses.                                */
  if (alignment > sizeof(void *)) {
    pool->alignbytes = alignment;
  } else {
    pool->alignbytes = sizeof(void *);
  }
  pool->itembytes = ((bytecount - 1) / pool->alignbytes + 1) *
                    pool->alignbytes;
  pool->itemsperblock = itemcount;
  if (firstitemcount == 0) {
    pool->itemsfirstblock = itemcount;
  } else {
    pool->itemsfirstblock = firstitemcount;
  }

  /* Allocate a block of items.  Space for `itemsfirstblock' items and one  */
  /*   pointer (to point to the next block) are allocated, as well as space */
  /*   to ensure alignment of the items.                                    */
  pool->firstblock = (void **)
    trimalloc(pool->itemsfirstblock * pool->itembytes + (int) sizeof(void *) +
              pool->alignbytes);
  /* Set the next block pointer to NULL. */
  *(pool->firstblock) = (void *) NULL;
  poolrestart(pool);
}

/*****************************************************************************/
/*                                                                           */
/*  pooldeinit()   Free to the operating system all memory taken by a pool.  */
/*                                                                           */
/*****************************************************************************/

void pooldeinit(struct memorypool *pool)
{
  while (pool->firstblock != (void **) NULL) {
    pool->nowblock = (void **) *(pool->firstblock);
    trifree((void *) pool->firstblock);
    pool->firstblock = pool->nowblock;
  }
}

/*****************************************************************************/
/*                                                                           */
/*  poolalloc()   Allocate space for an item.                                */
/*                                                                           */
/*****************************************************************************/

void *poolalloc(struct memorypool *pool)
{
  void *newitem;
  void **newblock;
  unsigned ULONGLONG alignptr;

  /* First check the linked list of dead items.  If the list is not   */
  /*   empty, allocate an item from the list rather than a fresh one. */
  if (pool->deaditemstack != (void *) NULL) {
    newitem = pool->deaditemstack;               /* Take first item in list. */
    pool->deaditemstack = * (void **) pool->deaditemstack;
  } else {
    /* Check if there are any free items left in the current block. */
    if (pool->unallocateditems == 0) {
      /* Check if another block must be allocated. */
      if (*(pool->nowblock) == (void *) NULL) {
        /* Allocate a new block of items, pointed to by the previous block. */
        newblock = (void **) trimalloc(pool->itemsperblock * pool->itembytes +
                                       (int) sizeof(void *) +
                                       pool->alignbytes);
        *(pool->nowblock) = (void *) newblock;
        /* The next block pointer is NULL. */
        *newblock = (void *) NULL;
      }

      /* Move to the new block. */
      pool->nowblock = (void **) *(pool->nowblock);
      /* Find the first item in the block.    */
      /*   Increment by the size of (void *). */
      alignptr = (unsigned ULONGLONG) (pool->nowblock + 1);
      /* Align the item on an `alignbytes'-byte boundary. */
      pool->nextitem = (void *)
        (alignptr + (unsigned ULONGLONG) pool->alignbytes -
         (alignptr % (unsigned ULONGLONG) pool->alignbytes));
      /* There are lots of unallocated items left in this block. */
      pool->unallocateditems = pool->itemsperblock;
    }

    /* Allocate a new item. */
    newitem = pool->nextitem;
    /* Advance `nextitem' pointer to next free item in block. */
    pool->nextitem = (void *) ((char *) pool->nextitem + pool->itembytes);
    pool->unallocateditems--;
    pool->maxitems++;
  }
  pool->items++;
  return newitem;
}

/*****************************************************************************/
/*                                                                           */
/*  pooldealloc()   Deallocate space for an item.                            */
/*                                                                           */
/*  The deallocated space is stored in a queue for later reuse.              */
/*                                                                           */
/*****************************************************************************/

void pooldealloc(struct memorypool *pool, void *dyingitem)
{
  /* Push freshly killed item onto stack. */
  *((void **) dyingitem) = pool->deaditemstack;
  pool->deaditemstack = dyingitem;
  pool->items--;
}

/*****************************************************************************/
/*                                                                           */
/*  traversalinit()   Prepare to traverse the entire list of items.          */
/*                                                                           */
/*  This routine is used in conjunction with traverse().                     */
/*                                                                           */
/*****************************************************************************/

void traversalinit(struct memorypool *pool)
{
  unsigned ULONGLONG alignptr;

  /* Begin the traversal in the first block. */
  pool->pathblock = pool->firstblock;
  /* Find the first item in the block.  Increment by the size of (void *). */
  alignptr = (unsigned ULONGLONG) (pool->pathblock + 1);
  /* Align with item on an `alignbytes'-byte boundary. */
  pool->pathitem = (void *)
    (alignptr + (unsigned ULONGLONG) pool->alignbytes -
     (alignptr % (unsigned ULONGLONG) pool->alignbytes));
  /* Set the number of items left in the current block. */
  pool->pathitemsleft = pool->itemsfirstblock;
}

/*****************************************************************************/
/*                                                                           */
/*  traverse()   Find the next item in the list.                             */
/*                                                                           */
/*  This routine is used in conjunction with traversalinit().  Be forewarned */
/*  that this routine successively returns all items in the list, including  */
/*  deallocated ones on the deaditemqueue.  It's up to you to figure out     */
/*  which ones are actually dead.  Why?  I don't want to allocate extra      */
/*  space just to demarcate dead items.  It can usually be done more         */
/*  space-efficiently by a routine that knows something about the structure  */
/*  of the item.                                                             */
/*                                                                           */
/*****************************************************************************/

void *traverse(struct memorypool *pool)
{
  void *newitem;
  unsigned ULONGLONG alignptr;

  /* Stop upon exhausting the list of items. */
  if (pool->pathitem == pool->nextitem) {
    return (void *) NULL;
  }

  /* Check whether any untraversed items remain in the current block. */
  if (pool->pathitemsleft == 0) {
    /* Find the next block. */
    pool->pathblock = (void **) *(pool->pathblock);
    /* Find the first item in the block.  Increment by the size of (void *). */
    alignptr = (unsigned ULONGLONG) (pool->pathblock + 1);
    /* Align with item on an `alignbytes'-byte boundary. */
    pool->pathitem = (void *)
      (alignptr + (unsigned ULONGLONG) pool->alignbytes -
       (alignptr % (unsigned ULONGLONG) pool->alignbytes));
    /* Set the number of items left in the current block. */
    pool->pathitemsleft = pool->itemsperblock;
  }

  newitem = pool->pathitem;
  /* Find the next item in the block. */
  pool->pathitem = (void *) ((char *) pool->pathitem + pool->itembytes);
  pool->pathitemsleft--;
  return newitem;
}
