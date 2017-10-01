#ifndef CACHE_H
#define CACHE_H

#include <QMap>

template <class T>
class Cache : public QMap<int,T>
{
public:
    void setMaxSize(int value);
    inline void append(const int &key, const T &t);
    inline void removeAt(const int &key);
protected:
    int m_maxSize {10};

    using QMap<int,T>::insert;
    using QMap<int,T>::insertMulti;
    using QMap<int,T>::unite;
};

template<class T>
void Cache<T>::setMaxSize(int value)
{
    m_maxSize = value;
}

template<class T>
void Cache<T>::append(const int &key, const T &t)
{
    if(QMap<int,T>::isEmpty())
    {
        QMap<int,T>::insert(key,t);
        return;
    }

    if(QMap<int,T>::size()+1 > m_maxSize)
    {
        int distanceFront = abs(key - QMap<int,T>::firstKey());
        int distanceBack = abs(key - QMap<int,T>::lastKey());
        auto remove = distanceFront > distanceBack ? QMap<int,T>::firstKey() :
                                                     QMap<int,T>::lastKey();
        QMap<int,T>::remove(remove);
    }
    QMap<int,T>::insert(key,t);
}

template<class T>
void Cache<T>::removeAt(const int &key)
{
    if(!QMap<int,T>::contains(key))
        return;
    for(auto it = QMap<int,T>::keys().end(); (*it)!=key; --it)
        QMap<int,T>::remove(*it);
    QMap<int,T>::remove(key);
}

#endif // CACHE_H
