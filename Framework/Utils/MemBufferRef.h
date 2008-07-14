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
#ifndef _MEM_BUFFER_REF_H_
#define _MEM_BUFFER_REF_H_

#include "RefCount.h"

struct sMemBufferData;
class NWCriticalSection;

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class MemBufferRef
{
public:
    MemBufferRef();
    explicit MemBufferRef(int _size);
    MemBufferRef(unsigned char * _buffer, int _size);
    virtual ~MemBufferRef();

    MemBufferRef(MemBufferRef const & _other);
    MemBufferRef& operator = (MemBufferRef const & _other);

    void CloneBuffer(unsigned char * _buffer, int _size);

    unsigned char * getPtr() const;
    int getSize() const;

private:
    sMemBufferData * mData;
    NWCriticalSection * mCritSec;
};

#endif _MEM_BUFFER_REF_H_
