#ifndef __BoundedBuffer_H__
#define __BoundedBuffer_H__

#include <assert.h>    // For error-checking purposes

//-------------------------------------------------
// Main structure of BoundedBuffer Class:
//-------------------------------------------------

template <class Elem>
class BoundedBuffer
{
  public:
    BoundedBuffer(int MaxSize=30000);
    BoundedBuffer(const BoundedBuffer<Elem> &OtherBoundedBuffer);
    ~BoundedBuffer(void);

    void       Enqueue(const Elem &Item);    // Adds Item to BoundedBuffer end
    Elem       Dequeue(void);                // Returns Item from BoundedBuffer
    inline int ElemNum(void);                // Returns Number of Elements
    bool isBufferFull();
    bool isBufferEmpty();

  protected:
    Elem     *Data;      // The actual Data array
    const int MAX_NUM;   // The actual spaces will be one more than this
    int       Beginning, // Numbered location of the start and end
              End;

    // Instead of calculating the number of elements, using this variable
    // is much more convenient.
    int       ElemCount;
};


#endif
