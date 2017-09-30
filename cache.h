#ifndef CACHE_H
#define CACHE_H

#include <QQueue>

template <class T>
class Cache : public QQueue<T>
{
public:
    void setMaxSize(int value);
    inline void enqueue(const T &t);
protected:
    int m_maxSize {10};

};

template<class T>
void Cache<T>::setMaxSize(int value)
{
    m_maxSize = value;
}

template<class T>
void Cache<T>::enqueue(const T &t)
{
    if(QQueue<T>::size()+1 > m_maxSize)
        QQueue<T>::dequeue();
    QQueue<T>::enqueue(t);
}

#endif // CACHE_H
