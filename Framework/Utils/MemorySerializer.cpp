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
