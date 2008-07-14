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
#include "PchUtils.h"

#include "MemorySerializer.h"
#include "MemBufferRef.h"

#include <vector>

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MemorySerializerIn::MemorySerializerIn() : Inherited()
{
    //Inherited::init(2048);
}

/*virtual*/ MemorySerializerIn::~MemorySerializerIn()
{
    mBufferSize = 0;
    mCurrentPos = NULL;
    mCurrentSize = 0;
    mInitd = false;
    //Inherited::done();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MemorySerializerIn::setBuffer(unsigned char * _buffer, int _bufferSize)
{
    mBuffer = _buffer;
    mBufferSize = _bufferSize;
    mCurrentPos = mBuffer;
    mCurrentSize = 0;
    mInitd = true;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ int MemorySerializerIn::loadBlock(unsigned char * _ptr, int _count)
{
    return 0;
}

//****************************************************************************
// Serializer Out
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
struct SerializerBlock
{
    unsigned char * mBuff;
    int mBuffSize;

    SerializerBlock();
    SerializerBlock(int _size);
    ~SerializerBlock();
};

SerializerBlock::SerializerBlock() :
    mBuff(NULL),
    mBuffSize(0)
{
}

SerializerBlock::SerializerBlock(int _size)
{
    mBuff = NEW unsigned char[_size];
    mBuffSize = _size;
}

SerializerBlock::~SerializerBlock()
{
    if(mBuff)
        DISPOSE_ARRAY(mBuff);
    mBuffSize = 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
struct SerializerBlockList
{
    std::vector<SerializerBlock> mList;

    SerializerBlockList();
};

SerializerBlockList::SerializerBlockList()
{
    mList.reserve(16);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MemorySerializerOut::MemorySerializerOut() : Inherited(),
    mFinalized(false),
    mBlockList(NULL)
{
    mBlockList = new SerializerBlockList();

    Inherited::init(2048);
}

/*virtual*/ MemorySerializerOut::~MemorySerializerOut()
{
    if(!mFinalized)
        finalize();

    DISPOSE(mBlockList);

    Inherited::done();
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void MemorySerializerOut::finalize()
{
    if(!mFinalized)
    {
        Inherited::finalize();
    }
}

unsigned char * MemorySerializerOut::getBufferPtr(int & _outBufferSize)
{
    unsigned char * retVal = NULL;    
    
    finalize();

    _outBufferSize = 0;

    int totalSize = 0;

    int numBlocks = (int)mBlockList->mList.size();
    for(int i=0; i<numBlocks; i++)
    {
        totalSize += mBlockList->mList[i].mBuffSize;
    }

    if(totalSize > 0)
    {
        unsigned char * totalBuffer = NEW unsigned char[totalSize];

        unsigned char * current = totalBuffer;
        for(int i=0; i<numBlocks; i++)
        {
            memcpy(current, mBlockList->mList[i].mBuff, mBlockList->mList[i].mBuffSize);
            current += mBlockList->mList[i].mBuffSize;
        }

        _outBufferSize = totalSize;

        retVal = totalBuffer;
    }

    return retVal;
}

MemBufferRef MemorySerializerOut::getMemBuffer()
{
    int size = 0;
    unsigned char * buff = getBufferPtr(size);
    
    MemBufferRef memBuff(buff, size);

    DISPOSE_ARRAY(buff);

    return memBuff;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void MemorySerializerOut::saveBlock(unsigned char * _ptr, int _count)
{
    SerializerBlock tmpBlock;

    mBlockList->mList.push_back(tmpBlock);

    SerializerBlock * block = &mBlockList->mList[mBlockList->mList.size()-1];

    if(block)
    {
        block->mBuff = NEW unsigned char[_count];
        memcpy(block->mBuff, _ptr, _count);
        block->mBuffSize = _count;
    }
}
