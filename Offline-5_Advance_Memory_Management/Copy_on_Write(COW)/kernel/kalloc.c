// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct spinlock refcntlock; // Lock for reference count
int pg_refcnt[MX_PGIDX]; // Array to store the refcount for every page

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  initlock(&refcntlock, "refcnt");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  acquire(&refcntlock);
  if(--PG_REFCNT(pa) <= 0) // If by reducing, reference count becomes 0, it will be released
  {
    // Fill with junk to catch dangling refs.
    memset(pa, 1, PGSIZE);

    r = (struct run*)pa;

    acquire(&kmem.lock);
    r->next = kmem.freelist;
    kmem.freelist = r;
    release(&kmem.lock);
  }
  release(&refcntlock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r){
    memset((char*)r, 5, PGSIZE); // fill with junk

    PG_REFCNT(r) = 1; // One process using this page frame during allocation, so it's set to 1
  }
  return (void*)r;
}

// Checks whether a certain virtual address is a legal, 
// unallocated COW page or not. In other words, when 
// a page fault occurs on a COW page, can a new physical 
// page be allocated or not
int 
is_unallocated_COW(pagetable_t pagetable, uint64 va)
{
  if(va >= MAXVA){
    return 0;
  }

  pte_t* pte = walk(pagetable, va, 0);

  // If this page does not exist
  if(pte == 0){
    return 0;
  }         
  // If the PTE is not valid   
  if((*pte & PTE_V) == 0){
    return 0;
  }
  // If user can't access the PTE
  if((*pte & PTE_U) == 0){
    return 0;
  }
  // The representative with PTE_COW has not been copied yet, and it is a cow page
  return ((*pte) & PTE_COW);
}

// Allocates physical memory to the COW page
int 
alloc_COW(pagetable_t pagetable, uint64 va)
{
  pte_t *pte;
  uint64 pa, curr_va;
  uint flags;
  char *newpg;

  if ((pte = walk(pagetable, va, 0)) == 0){
    printf("allow_COW: PTE doesn't exist\n");
    return -1;
  }
  // pa is the physical address of the page table of the parent process 
  // originally used by this page frame
  pa = PTE2PA(*pte);
  flags = PTE_FLAGS(*pte);

  if ((newpg = kalloc()) == 0){
    return -1;
  }

  curr_va = PGROUNDDOWN(va); // Current page frame

  flags &= (~PTE_COW); // After copying, it's not a valid COW page
  flags |= PTE_W;    // After copying, we can write

  memmove(newpg, (char *)pa, PGSIZE); // Copy the data of the parent process page frame
  uvmunmap(pagetable, curr_va, 1, 1); // unmap the parent process page frame
  
  if(mappages(pagetable, curr_va, PGSIZE, (uint64)newpg, flags) < 0){
    kfree(newpg);
    return -1;
  }
  return 0;
}

// Increase the reference count of the page frame of the parent process 
// (one more process is sharing this page frame)
void 
add_ref(void* pa){
  acquire(&refcntlock);
  PG_REFCNT(pa)++;
  release(&refcntlock);
}