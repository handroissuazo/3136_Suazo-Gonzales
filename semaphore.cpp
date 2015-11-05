#include "semaphore.h"

Semaphore::Semaphore(int _sizeOfBuffer){
  bf_mp4 = new BoundedBuffer<int>(_sizeOfBuffer);
}

Semaphore::~Semaphore(){
  delete bf_mp4;
}

/* -- SEMAPHORE OPERATIONS */

int Semaphore::P(){
  std::unique_lock<std::mutex> lock(the_mutex);
  while ( bf_mp4->isBufferFull() ){
    /* simultaneously wait and release the mutex */
    the_notfull_cvar.wait(lock);
    /* the mutex is reaquired at this point */
  }

  /* buffer has space and we own the mutex: insert the item */
  /*...implementation here...*/
  /* tell anyone waiting on an empty buffer that they can wake up. */
  the_notempty_cvar.notify_all();
}

int Semaphore::V(){
  std::unique_lock<std::mutex> lock(the_mutex);
  int value = 0;

  while ( bf_mp4->isBufferEmpty() ){
    /* simultaneously wait and release the mutex */
    the_notempty_cvar.wait(lock);
    /* the mutex is reaquired at this point */
  }

  /* buffer has something in it and we own the mutex: get the item */
  /*...implement here...*/

  /* tell anyone waiting on a full buffer that they can wake up. */
  the_notfull_cvar.notify_all();

  return value;
}

int main() {
  Semaphore* curr = new Semaphore(200000);
  delete curr;
}
