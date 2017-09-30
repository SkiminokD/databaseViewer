#ifndef CACHE_H
#define CACHE_H

#include <QList>

template <class T>
class Cache : public QList<T>
{
public:
    void setMaxSize(int value);
    inline void append(const T &t);
protected:
    int m_maxSize {10};

    using QList<T>::insert;
    using QList<T>::operator +;
    using QList<T>::operator +=;
    using QList<T>::push_back;
    using QList<T>::push_front;
};

template<class T>
void Cache<T>::setMaxSize(int value)
{
    m_maxSize = value;
}

template<class T>
void Cache<T>::append(const T &t)
{
    if(QList<T>::size()+1 > m_maxSize)
        QList<T>::takeFirst();
    QList<T>::append(t);
}

#endif // CACHE_H
