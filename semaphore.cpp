#include "semaphore.h"

Semaphore::Semaphore(int _sizeOfBuffer){
  bf_mp4 = new BoundedBuffer<RequestPackage>(_sizeOfBuffer);
  bIsDone = false;
}

Semaphore::~Semaphore(){
  delete bf_mp4;
}

/* -- SEMAPHORE OPERATIONS */

void Semaphore::P(RequestPackage _content){
  std::unique_lock<std::mutex> lock(the_mutex);
  while ( bf_mp4->isBufferFull() ){
    /* simultaneously wait and release the mutex */
    the_notfull_cvar.wait(lock);
    /* the mutex is reaquired at this point */
  }

  /* buffer has space and we own the mutex: insert the item */
  bf_mp4->Enqueue(_content);
  // printf("You queued something\n");
  /* tell anyone waiting on an empty buffer that they can wake up. */
  the_notempty_cvar.notify_all();
}

RequestPackage Semaphore::V(){
  std::unique_lock<std::mutex> lock(the_mutex);
  RequestPackage value;

  while ( bf_mp4->isBufferEmpty() ){
    /* simultaneously wait and release the mutex */
    the_notempty_cvar.wait(lock);
    /* the mutex is reaquired at this point */
  }
  // printf("You dequeued something\n");
  /* buffer has something in it and we own the mutex: get the item */
  value = bf_mp4->Dequeue();

  /* tell anyone waiting on a full buffer that they can wake up. */
  the_notfull_cvar.notify_all();

  return value;
}

bool Semaphore::isDone(){
  return bIsDone;
}

void Semaphore::setDone(bool areWeDone){
  bIsDone = areWeDone;
}
