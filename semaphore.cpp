#include "semaphore.h"

//TODO: remove before committing
#include <thread>
#include <iostream>       // std::cout
// ------------------------------

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
  value = bf_mp4->Dequeue();

  /* tell anyone waiting on a full buffer that they can wake up. */
  the_notfull_cvar.notify_all();

  return value;
}

void foo(Semaphore* theTest){
  for (int i = 0; i < 15001; ++i){
    theTest->P(i);
    printf("Queueing From 1...\n");
  }
}

void foo2(Semaphore* theTest){
  for (int i = 0; i < 15001; ++i){
    theTest->P(i);
    printf("Queueing From 2...\n");
  }
}

void bar(Semaphore* theTest){
  for (int i = 0; i < 15001; ++i){
    printf("Dequeued Success from 3: %i \n", theTest->V());
  }
}

void bar2(Semaphore* theTest){
  for (int i = 0; i < 15001; ++i){
    printf("Dequeued Success from 4: %i \n", theTest->V());
  }
}

int main()
{
  Semaphore* theTest = new Semaphore(30000);

  std::thread first (foo, theTest);     // spawn new thread that calls foo()
  std::thread third (foo2, theTest);     // spawn new thread that calls foo()
  std::thread second (bar, theTest);  // spawn new thread that calls bar(0)
  std::thread fourth (bar2, theTest);  // spawn new thread that calls bar(0)

  std::cout << "main, threads now execute concurrently...\n";

  // synchronize threads:
  first.join();                // pauses until first finishes
  second.join();               // pauses until second finishes
  third.join();                // pauses until first finishes
  fourth.join();               // pauses until second finishes

  std::cout << "Threads completed.\n";

  delete theTest;
    return 0;
}
