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

#include "MemBufferRef.h"
#include "NWCriticalSection.h"
#include <memory.h>

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
struct sMemBufferData : public RefCount
{
    unsigned char * mBuffer;
    int mSize;

    sMemBufferData(int _size);
    sMemBufferData(unsigned char * _buffer, int _size);
    ~sMemBufferData();

private:
};

sMemBufferData::sMemBufferData(int _size) : RefCount(),
    mBuffer(NULL),
    mSize(_size)
{
    if(_size > 0)
    {
        mBuffer = NEW unsigned char[_size];
    }
}

sMemBufferData::sMemBufferData(unsigned char * _buffer, int _size) : RefCount(),
    mBuffer(NULL),
    mSize(_size)
{
    if(_buffer && _size > 0)
    {
        mBuffer = NEW unsigned char[_size];
        memcpy(mBuffer, _buffer, _size);
    }
}

sMemBufferData::~sMemBufferData()
{
    DISPOSE_ARRAY(mBuffer);
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MemBufferRef::MemBufferRef() :
    mData(NULL)
{
    mCritSec = NWCriticalSection::create();
}

MemBufferRef::MemBufferRef(int _size) :
    mData(NULL)
{
    mCritSec = NWCriticalSection::create();

    if(_size > 0)
    {
        mData = NEW sMemBufferData(_size);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MemBufferRef::MemBufferRef(unsigned char * _buffer, int _size) :
    mData(NULL)
{
    mCritSec = NWCriticalSection::create();

    if(_buffer && _size > 0)
    {
        mData = NEW sMemBufferData(_buffer, _size);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ MemBufferRef::~MemBufferRef()
{
    mCritSec->enter();
    {
        if(mData)
        {
            if(mData->release(false) == 0)
            {
                DISPOSE(mData);
            }
        }
    }
    mCritSec->leave();
    
    NWCriticalSection::destroy(mCritSec);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MemBufferRef::MemBufferRef(MemBufferRef const & _other)
{
    mCritSec = NWCriticalSection::create();

    NWAutoCritSec autoCS(mCritSec);

    mData = _other.mData;
    if(mData)
        mData->addRef();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MemBufferRef& MemBufferRef::operator = (MemBufferRef const & _other)
{
    NWAutoCritSec autoCS(mCritSec);

    if(mData)
        mData->release();

    mData = _other.mData;

    if(mData)
        mData->addRef();

    return *this;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MemBufferRef::CloneBuffer(unsigned char * _buffer, int _size)
{
    if(_buffer && _size > 0)
    {
        if(mData)
            mData->release();

        mData = NEW sMemBufferData(_buffer, _size);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
unsigned char * MemBufferRef::getPtr() const
{
    NWAutoCritSec autoCS(mCritSec);

    return mData ? mData->mBuffer : NULL;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int MemBufferRef::getSize() const
{
    NWAutoCritSec autoCS(mCritSec);

    return mData ? mData->mSize : 0;
}
