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

