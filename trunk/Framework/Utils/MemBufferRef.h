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
