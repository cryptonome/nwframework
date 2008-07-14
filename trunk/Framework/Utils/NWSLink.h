/*       
*       This file is part of NWFramework.
*       Copyright (c) InCrew Software and Others.
*       (See the AUTHORS file in the root of this distribution.)
*
*       NWFramework is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 2 of the License, or
*       (at your option) any later version.
*
*       NWFramework is distributed in the hope that it will be useful,
*       but WITHOUT ANY WARRANTY; without even the implied warranty of
*       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*       GNU General Public License for more details.
* 
*       You should have received a copy of the GNU General Public License
*       along with NWFramework; if not, write to the Free Software
*       Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*
*      Permission is hereby granted, free of charge, to any person obtaining
*      a copy of this software and associated documentation files (the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*      distribute, sublicense, and/or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions:
*
*      The above copyright notice and this permission notice shall be
*      included in all copies or substantial portions of the Software.
*
*      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
*      LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
*      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
