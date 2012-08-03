//
//  CircularBuffer.h
//  emptyExample
//
//  Created by Broadcast on 03/08/2012.
//
//

#ifndef emptyExample_CircularBuffer_h
#define emptyExample_CircularBuffer_h

template <typename T>
class CircularBuffer {
public:
  CircularBuffer();
  CircularBuffer(int size);
  ~CircularBuffer();
  
  void write(T val);
  T read();
  
private:
  T *data;
  int data_size;
  int writeIndex, readIndex;
  
  
};

template <typename T>
CircularBuffer<T>::CircularBuffer():
data(NULL),
data_size(0),
writeIndex(0),
readIndex(-1) {}

template <typename T>
CircularBuffer<T>::CircularBuffer(int size):
data_size(size),
writeIndex(0),
readIndex(-1) {
  assert(size > 0);
  data = new T[data_size];
}

template <typename T>
CircularBuffer<T>::~CircularBuffer() {
  if (data != NULL) {
    delete[] data;
  }
}


template <typename T>
void CircularBuffer<T>::write(T val) {
  
  
  data[writeIndex] = val;

  if (readIndex < 0) {
    readIndex = writeIndex;
  }
  
  writeIndex++;
  writeIndex %= data_size;
}

template <typename T>
T CircularBuffer<T>::read() {
  
  if (readIndex < 0) {
    //buffer is empty
    cout << "buffer empty" << endl;
    return 0;
  }
  
  
  T val = data[readIndex];
  readIndex++;
  readIndex %= data_size;
  

  
  return val;
}
#endif
