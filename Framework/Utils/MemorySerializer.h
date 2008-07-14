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
#ifndef _MEMORY_SERIALIZER_H_
#define _MEMORY_SERIALIZER_H_

#include "Serializer.h"

struct SerializerBlockList;
class MemBufferRef;

//****************************************************************************
//
//****************************************************************************
class MemorySerializerIn : public SerializerIn
{
public:
    MemorySerializerIn();
    virtual ~MemorySerializerIn();

    void setBuffer(unsigned char * _buffer, int _bufferSize);

private:
    typedef SerializerIn Inherited;

    virtual int loadBlock(unsigned char * _ptr, int _count);
};

//****************************************************************************
//
//****************************************************************************
class MemorySerializerOut : public SerializerOut
{
public:
    MemorySerializerOut();
    virtual ~MemorySerializerOut();

    virtual void finalize();

    unsigned char * getBufferPtr(int & _outBufferSize);
    MemBufferRef getMemBuffer();

private:
    typedef SerializerOut Inherited;

    bool mFinalized;
    SerializerBlockList * mBlockList;

    virtual void saveBlock(unsigned char * _ptr, int _count);
};

#endif // _MEMORY_SERIALIZER_H_

