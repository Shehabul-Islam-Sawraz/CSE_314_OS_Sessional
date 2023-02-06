#include "bathroom_lock.h"

void InitalizeMenWomenLock(struct bathroom_lock * lock)
{
  //	Write the code for initializing your men-women lock.
  lock->men = 0;
  lock->women = 0;
  zem_init(&lock->menlock, 1);
  zem_init(&lock->womenlock, 1);
}

void MenLock(struct bathroom_lock * lock)
{
  //	Write the code for acquiring men-women lock by the mener.
  zem_down(&lock->menlock);
  
  lock->men++;
  if(lock->men == 1)
      zem_down(&lock->womenlock);

  zem_up(&lock->menlock);
}

void MenUnlock(struct bathroom_lock * lock)
{
  //	Write the code for releasing men-women lock by the mener.
  zem_down(&lock->menlock);
  
  lock->men--;
  if(lock->men == 0)
      zem_up(&lock->womenlock);

  zem_up(&lock->menlock);
}

void WomenLock(struct bathroom_lock * lock)
{
  //	Write the code for acquiring men-women lock by the womenr.
  zem_down(&lock->womenlock);
  
  lock->women++;
  if(lock->women == 1)
      zem_down(&lock->menlock);

  zem_up(&lock->womenlock);
}

void WomenUnlock(struct bathroom_lock * lock)
{
  //	Write the code for releasing men-women lock by the womenr.
  zem_down(&lock->womenlock);
  
  lock->women--;
  if(lock->women == 0)
      zem_up(&lock->menlock);

  zem_up(&lock->womenlock);
}
