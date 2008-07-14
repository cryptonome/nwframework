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
#include "Serializer.h"

//****************************************************************************
// Serializer
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
Serializer::Serializer() :
    mInitd(false),
    mBuffer(NULL),
    mBufferSize(0),
    mCurrentPos(NULL),
    mCurrentSize(0)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
Serializer::~Serializer()
{
    ASSERT(!mInitd);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool Serializer::init(int _bufferSize)
{
    bool retVal = false;

    if(!mInitd)
    {
        mBuffer = NEW unsigned char[_bufferSize];
        mBufferSize = _bufferSize;
        mCurrentPos = mBuffer;
        mCurrentSize = 0;
        mInitd = true;
        retVal = true;
    }

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Serializer::done()
{
    if(mInitd)
    {
        DISPOSE_ARRAY(mBuffer);
        mBufferSize = 0;
        mCurrentPos = NULL;
        mCurrentSize = 0;
        mInitd = false;
    }
}

//****************************************************************************
// SerializerIn
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
SerializerIn::SerializerIn() : Inherited()
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ SerializerIn::~SerializerIn()
{
    ASSERT(!mInitd);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ bool SerializerIn::init(int _bufferSize)
{
    return Inherited::init(_bufferSize);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void SerializerIn::done()
{
    Inherited::done();
}

//****************************************************************************
// ISerializerIn
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/  std::string SerializerIn::getString()
{
    std::string retVal = "";

    int numChars = getInt();
    if(numChars)
    {
        unsigned char * tmpBuff = NEW unsigned char[numChars+1];
        memset(tmpBuff, 0, numChars+1);
        get(tmpBuff, numChars);
        retVal = (char *)tmpBuff;
        DISPOSE_ARRAY(tmpBuff);
    }

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/  bool SerializerIn::getBool()
{
    bool retVal = false;

    get((unsigned char *)&retVal, sizeof(bool));

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/  char SerializerIn::getChar()
{
    char retVal = '\0';

    get((unsigned char *)&retVal, sizeof(char));

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/  unsigned char SerializerIn::getUChar()
{
    char retVal = '\0';

    get((unsigned char *)&retVal, sizeof(unsigned char));

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/  int SerializerIn::getInt()
{
    int retVal = 0;

    get((unsigned char *)&retVal, sizeof(int));

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/  unsigned int SerializerIn::getUInt()
{
    unsigned int retVal = 0;

    get((unsigned char *)&retVal, sizeof(unsigned int));

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/  float SerializerIn::getFloat()
{
    float retVal = 0;

    get((unsigned char *)&retVal, sizeof(float));

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/  double SerializerIn::getDouble()
{
    double retVal = 0;

    get((unsigned char *)&retVal, sizeof(double));

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/  void SerializerIn::getBuffer(unsigned char * _buffer, int _bufferSize)
{
    get(_buffer, _bufferSize);
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void SerializerIn::get(unsigned char * _ptr, int _byteCount)
{
    if(_ptr && _byteCount)
    {
        int size = _byteCount;
        unsigned char * outPtr = _ptr;

        while(size)
        {
            if(mCurrentSize < mBufferSize)
            {
                *outPtr = *mCurrentPos;
                mCurrentPos++;
                outPtr++;
                mCurrentSize++;
                size--;
            }
            else
            {
                memset(mBuffer, 0, mBufferSize);
                loadBlock(mBuffer, mBufferSize);
                mCurrentPos = mBuffer;
                mCurrentSize = 0;
            }
        }
    }
}

//****************************************************************************
// SerializerOut
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
SerializerOut::SerializerOut() : Inherited(),
    mFinalized(false)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ SerializerOut::~SerializerOut()
{
    ASSERT(mFinalized);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ bool SerializerOut::init(int _bufferSize)
{
    bool retVal = Inherited::init(_bufferSize);

    if(retVal)
    {
        mFinalized = false;
    }

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void SerializerOut::done()
{
    if(!mFinalized)
        finalize();

    Inherited::done();
}

//****************************************************************************
// ISerializerOut
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void SerializerOut::addString(std::string _str)
{
    int numChars = (int)_str.size();
    addInt(numChars);

    if(numChars > 0)
    {
        add((unsigned char *)_str.c_str(), numChars);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void SerializerOut::addBool(bool _val)
{
    add((unsigned char *)&_val, sizeof(bool));
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void SerializerOut::addChar(char _val)
{
    add((unsigned char *)&_val, sizeof(char));
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void SerializerOut::addUChar(unsigned char _val)
{
    add((unsigned char *)&_val, sizeof(unsigned char));
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void SerializerOut::addInt(int _val)
{
    add((unsigned char *)&_val, sizeof(int));
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void SerializerOut::addUInt(unsigned int _val)
{
    add((unsigned char *)&_val, sizeof(unsigned int));
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void SerializerOut::addFloat(float _val)
{
    add((unsigned char *)&_val, sizeof(float));
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void SerializerOut::addDouble(double _val)
{
    add((unsigned char *)&_val, sizeof(double));
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void SerializerOut::addBuffer(unsigned char * _buffer, int _bufferSize)
{
    add(_buffer, _bufferSize);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void SerializerOut::finalize()
{
    if(!mFinalized)
    {
        saveBlock(mBuffer, mCurrentSize);
        mFinalized = true;
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void SerializerOut::add(unsigned char * _ptr, int _byteCount)
{
    if(_ptr && _byteCount)
    {
        int size = _byteCount;

        unsigned char * inPtr = _ptr;

        while(size)
        {
            if(mCurrentSize < mBufferSize)
            {
                *mCurrentPos = *inPtr;
                mCurrentPos++;
                inPtr++;
                mCurrentSize++;
                size--;
            }
            else
            {
                saveBlock(mBuffer, mBufferSize);
                memset(mBuffer, 0, mBufferSize);
                mCurrentPos = mBuffer;
                mCurrentSize = 0;
            }
        }
    }
}
