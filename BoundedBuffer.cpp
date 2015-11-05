#include "BoundedBuffer.h"

// BoundedBuffer Constructor function
template <class Elem>
BoundedBuffer<Elem>::BoundedBuffer(int MaxSize) :
    MAX_NUM( MaxSize )   // Initialize the constant
{
  // This extra space added will allow us to distinguish between
  // the Beginning and the End locations.
  Data      = new Elem[MAX_NUM + 1];
  Beginning = 0;
  End       = 0;
  ElemCount = 0;
}

// BoundedBuffer Copy Constructor function
template <class Elem>
BoundedBuffer<Elem>::BoundedBuffer(const BoundedBuffer &OtherBoundedBuffer) : MAX_NUM( OtherBoundedBuffer.MAX_NUM )  // Initialize the constant
{
  Beginning = OtherBoundedBuffer.Beginning;
  End       = OtherBoundedBuffer.End;
  ElemCount = OtherBoundedBuffer.ElemCount;

  Data      = new Elem[MAX_NUM + 1];
  for (int i = 0; i < MAX_NUM; i++)
    Data[i] = OtherBoundedBuffer.Data[i];
}

// BoundedBuffer Destructor function
template <class Elem>
BoundedBuffer<Elem>::~BoundedBuffer(void)
{
  delete[] Data;
}

// Enqueue() function
template <class Elem>
void BoundedBuffer<Elem>::Enqueue(const Elem &Item)
{
  // Error Check: Make sure we aren't exceeding our maximum storage space
  assert( ElemCount < MAX_NUM );

  Data[ End++ ] = Item;
  ++ElemCount;

  // Check for wrap-around
  if (End > MAX_NUM)
    End -= (MAX_NUM + 1);
}

// Dequeue() function
template <class Elem>
Elem BoundedBuffer<Elem>::Dequeue(void)
{
  // Error Check: Make sure we aren't dequeueing from an empty queue
  assert( ElemCount > 0 );

  Elem ReturnValue = Data[ Beginning++ ];
  --ElemCount;

  // Check for wrap-around
  if (Beginning > MAX_NUM)
    Beginning -= (MAX_NUM + 1);

  return ReturnValue;
}

// ElemNum() function
template <class Elem>
inline int BoundedBuffer<Elem>::ElemNum(void)
{
  return ElemCount;
}

// isBufferFull() function
template <class Elem>
bool BoundedBuffer<Elem>::isBufferFull(){
  assert( ElemCount < MAX_NUM );
  return ElemCount >= MAX_NUM;
}

// isBufferEmpty() function
template <class Elem>
bool BoundedBuffer<Elem>::isBufferEmpty(){
  assert( ElemCount > 0 );
  return ElemCount <= 0;
}
