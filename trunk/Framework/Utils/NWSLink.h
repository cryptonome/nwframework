/* 
 * Copyright 2007-2008 InCrew Software, All Rights Reserved
 *
 * This file may be used or modified without the need for a license.
 *
 * Redistribution of this file in either its original form, or in an
 * updated form may be done under the terms of the GNU LIBRARY GENERAL
 * PUBLIC LICENSE.  If this license is unacceptable to you then you
 * may not redistribute this work.
 * 
 * See the file COPYING.GPL for details.
 */
#ifndef NW_SLINK_H
#define NW_SLINK_H

//****************************************************************************
//
//****************************************************************************
template <class T> class SPLink
{
public:
    inline SPLink();
    //inline virtual ~SPLink();

    inline T * getHead();

    inline T * getPrev();
    inline void setPrev(T * _prev);

private:
    T * volatile mPrev; // volatile pointer to non volatile data

    SPLink operator = (SPLink const & _other);
};

template <class T> inline SPLink<T>::SPLink() :
    mPrev(NULL)
{
}

//template <class T> inline SPLink<T>::~SPLink()
//{
//}

template <class T> inline T * SPLink<T>::getHead()
{
    T * head = (T *)this;

    while(head->getPrev())
    {
        head = head->getPrev();
    }

    return head;
}

template <class T> inline T * SPLink<T>::getPrev()
{
    return mPrev;
}

template <class T> inline void SPLink<T>::setPrev(T * _prev)
{
    mPrev = _prev;
}

//****************************************************************************
//
//****************************************************************************
template <class T> class SNLink
{
public:
    inline SNLink();
    //inline virtual ~SNLink();

    inline T * getTail();

    inline T * getNext();
    inline void setNext(T * _prev);

private:
    T * volatile mNext; // volatile pointer to non volatile data

    SNLink operator = (SNLink const & _other);
};

template <class T> inline SNLink<T>::SNLink() :
    mNext(NULL)
{
}

//template <class T> inline SNLink<T>::~SNLink()
//{
//}

template <class T> inline T * SNLink<T>::getTail()
{
    T * tail = (T *)this;

    while(tail->getPrev())
    {
        tail = tail->getPrev();
    }

    return tail;
}

template <class T> inline T * SNLink<T>::getNext()
{
    return mNext;
}

template <class T> inline void SNLink<T>::setNext(T * _next)
{
    mNext = _next;
}

//****************************************************************************
//
//****************************************************************************
template <class T> class DLink
{
public:
    inline DLink();
    //inline virtual ~DLink();

    inline T * getHead();
    inline T * getTail();

    inline T * getPrev();
    inline T * getNext();

    inline void linkAsPrevOf(T * _other);
    inline void linkAsNextOf(T * _other);

    inline void unlink();

private:
    T * volatile mPrev; // volatile pointer to non volatile data
    T * volatile mNext; // volatile pointer to non volatile data

    // zzz pending : link update lock

    inline void setNext(T * _next);
    inline void setPrev(T * _prev);

    DLink operator = (DLink const & _other);
};

template <class T> inline DLink<T>::DLink() : 
    mPrev(NULL),
    mNext(NULL)
{
}

//template <class T> inline DLink<T>::~DLink()
//{
//    unlink();
//}

template <class T> inline T * DLink<T>::getHead()
{
    T * head = (T *)this;
    
    while(head->getPrev())
    {
        head = head->getPrev();
    }

    return head;
}

template <class T> inline T * DLink<T>::getTail()
{
    T * tail = (T *)this;

    while(tail->getNext())
    {
        tail = tail->getNext();
    }

    return tail;
}

template <class T> inline T * DLink<T>::getPrev()
{
    return mPrev;
}

template <class T> inline void DLink<T>::setPrev(T * _prev)
{
    mPrev = _prev;
}

template <class T> inline T * DLink<T>::getNext()
{
    return mNext;
}

template <class T> inline void DLink<T>::setNext(T * _next)
{
    mNext = _next;
}

template <class T> inline void DLink<T>::linkAsPrevOf(T * _other)
{
    if(_other)
    {
        setNext(_other);
        setPrev(_other->getPrev());
        _other->setPrev((T*)this);
    }
}

template <class T> inline void DLink<T>::linkAsNextOf(T * _other)
{
    if(_other)
    {
        setPrev(_other);
        setNext(_other->getNext());
        _other->setNext((T*)this);
    }
}

template <class T> inline void DLink<T>::unlink()
{
    T * prev = getPrev();
    T * next = getNext();
    
    if(prev)
    {
        prev->setNext(next);
        setPrev(NULL);
    }

    if(next)
    {
        next->setPrev(prev);
        setNext(NULL);
    }
}

#endif // NW_SLINK_H
