#include "semaphore.h"

Semaphore::Semaphore(int _sizeOfBuffer){
  bf_mp4 = new BoundedBuffer<int>(_sizeOfBuffer);
}

Semaphore::~Semaphore(){
  delete bf_mp4;
}

/* -- SEMAPHORE OPERATIONS */

int Semaphore::P(int _content){
  std::unique_lock<std::mutex> lock(the_mutex);
  while ( bf_mp4->isBufferFull() ){
    /* simultaneously wait and release the mutex */
    the_notfull_cvar.wait(lock);
    /* the mutex is reaquired at this point */
  }

  /* buffer has space and we own the mutex: insert the item */
  bf_mp4->Enqueue(_content);

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
  int value = bf_mp4->Dequeue();

  /* tell anyone waiting on a full buffer that they can wake up. */
  the_notfull_cvar.notify_all();

  return value;
}

// int main()
// {
//   Semaphore* theTest = new Semaphore(30000);
//
//
//   theTest->P(44);
//   theTest->V();
//
//   delete theTest;
// }
