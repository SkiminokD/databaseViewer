#ifndef CACHE_H
#define CACHE_H

#include <QList>

template <class T>
class Cache : public QList<QPair<T,int>>
{
public:
    void setMaxSize(int value);
    inline void append(const QPair<T,int> &t);
protected:
    int m_maxSize {10};

    using QList<QPair<T,int>>::insert;
    using QList<QPair<T,int>>::operator +;
    using QList<QPair<T,int>>::operator +=;
    using QList<QPair<T,int>>::push_back;
    using QList<QPair<T,int>>::push_front;
};

template<class T>
void Cache<T>::setMaxSize(int value)
{
    m_maxSize = value;
}

template<class T>
void Cache<T>::append(const QPair<T,int> &t)
{
    if(QList<QPair<T,int>>::isEmpty())
    {
        QList<QPair<T,int>>::append(t);
        return;
    }

    int distanceFront = abs(t.second - QList<QPair<T,int>>::first().second);
    int distanceBack = abs(t.second - QList<QPair<T,int>>::last().second);

    if(QList<QPair<T,int>>::size()+1 > m_maxSize)
    {
        auto remove = distanceFront > distanceBack ? &QList<QPair<T,int>>::takeFirst :
                                                     &QList<QPair<T,int>>::takeLast;
        (this->*remove)();
    }
    auto append = distanceFront > distanceBack ? &QList<QPair<T,int>>::push_back :
                                                 &QList<QPair<T,int>>::push_front;
    (this->*append)(t);
}

#endif // CACHE_H
