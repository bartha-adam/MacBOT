#ifndef QUEUE_H
#define QUEUE_H

template <class T>
class Queue
{
    //TODO: improve permformance by implementing circular buffer
private:
    T* queue;
    unsigned int queueSize;
    unsigned int queueMaxSize;
public:
    Queue(unsigned short size)
    {
        queue = new T[size];
        queueMaxSize = size;
    }

    ~Queue()
    {
        delete[] queue;
    }

    bool IsEmpty()
    {
        return queueSize == 0;
    }

    bool IsFull()
    {
        return queueSize == queueMaxSize;
    }

    unsigned int count()
    {
        return queueSize;
    }

    void Clear()
    {
        for (unsigned int i = 0; i < queueSize; ++i)
          queue[i] = "";
        queueSize = 0;
    }

    bool Push(const T& element)
    {
        if (IsFull())
          return false;
        queue[queueSize] = element;
        queueSize++;
        return true;
    }

    bool Pop(T& element)
    {
        if (IsEmpty())
            return false;
        element = queue[0];
        for (unsigned int i = 0; i < queueSize - 1; ++i)
            queue[i] = queue[i + 1];
        --queueSize;
        return true;
    }
};

#endif //QUEUE_H
