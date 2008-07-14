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
#include "PchNWStream.h"

#include "NWStream.h"
#include "NWStreamBlockQueue.h"
#include "INWStreamBlock.h"
#include "SystemUtils.h"
#include "NWStreamGroup.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamWriter::NWStreamWriter() :
    mInit(false),
    mQueue(0),
    mDisabled(false),
    mWriteThreadId(0xffffffff)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamWriter::init(ENWStreamType _type, ENWStreamSubType _subType)
{
    bool bOK = true;

    if (!isOk())
    {
        mType = _type;
        mSubType = _subType;
        mDisabled = false;
        mStreamGroupWrite = 0;
        //mStreamGroupRead = 0;
        mWriteThreadId = 0xffffffff;

        mQueue = NEW NWStreamBlockQueue();
        bOK = mQueue->init(this);

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamWriter::done()
{
    if (isOk())
    {
        mQueue->done();
        DISPOSE(mQueue);

        mInit = false;
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
ENWStreamType NWStreamWriter::getType() const
{
    return mType;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
ENWStreamSubType NWStreamWriter::getSubType() const
{
    return mSubType;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamWriter::writeBlock(INWStreamBlock* _block, bool _checkThread)
{
#ifdef _DEBUG
    if ( _checkThread )
        checkWriteThreadId();
#endif

    ASSERT(_block->getType() == getType() && _block->getSubType() == getSubType());

    mQueue->writeBlock(_block);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamWriter::setStreamGroupWrite(NWStreamGroupWrite* _streamGroup)
{
    mStreamGroupWrite = _streamGroup;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamWriter::checkWriteThreadId()
{
    unsigned int threadId = SystemUtils::getCurrentThreadId();
    if (mWriteThreadId == 0xffffffff )
        mWriteThreadId = threadId;
    else
    {
        ASSERT(mWriteThreadId == threadId);
        mWriteThreadId = threadId;
    }

    int streams = mStreamGroupWrite->getNumStreams();
    for ( int i = 0 ; i < streams ; ++i )
    {
        NWStreamWriter* stream = (NWStreamWriter*)mStreamGroupWrite->getStream(i);
        if ( stream != this )
        {
            ASSERT(mWriteThreadId != stream->mWriteThreadId);
        }
    }
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamWriter::disableWrite(bool _disable)
{ 
    mDisabled = _disable; 
    mQueue->disableWrite(_disable);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
INWStreamReader* NWStreamWriter::createReader()
{
    NWStreamReader* reader = NEW NWStreamReader();
    
    if ( !reader->init(this) )
        DISPOSE(reader);

    return reader;

}







//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamReader::NWStreamReader() :
    mInit(false),
    mDisabled(false),
    mReadThreadId(0xffffffff)
{
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamReader::init(NWStreamWriter* _stream)
{
    bool bOK = true;

    if (!isOk())
    {
        mStream = _stream;
        mDisabled = false;
        mStreamGroupRead = 0;
        mReadThreadId = 0xffffffff;

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamReader::done()
{
    if (isOk())
    {
        mInit = false;
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamReader::setStreamGroupRead(NWStreamGroupRead* _streamGroup)
{
    mStreamGroupRead = _streamGroup;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
INWStreamBlock* NWStreamReader::readBlock(bool _checkThread)
{
#ifdef _DEBUG
    if ( _checkThread )
        checkReadThreadId();
#endif

    return mStream->getQueue()->readBlock();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamReader::disableRead(bool _disable)
{ 
    mDisabled = _disable; 
    mStream->getQueue()->disableRead(_disable);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
ENWStreamType NWStreamReader::getType() const
{
    return mStream->getType();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
ENWStreamSubType NWStreamReader::getSubType() const
{
    return mStream->getSubType();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
u64 NWStreamReader::getStartTimeAbs()
{
    return mStreamGroupRead->getStartTimeAbs();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamReader::setStartTimeAbs(u64 _time)
{
    mStreamGroupRead->setStartTimeAbs(_time);
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamReader::checkReadThreadId()
{
    unsigned int threadId = SystemUtils::getCurrentThreadId();
    if (mReadThreadId == 0xffffffff )
        mReadThreadId = threadId;
    else
    {
        ASSERT(mReadThreadId == threadId);
        mReadThreadId = threadId;
    }

    int streams = mStreamGroupRead->getNumStreams();
    for ( int i = 0 ; i < streams ; ++i )
    {
        NWStreamReader* stream = (NWStreamReader*)mStreamGroupRead->getStream(i);
        if ( stream != this )
        {
            ASSERT(mReadThreadId != stream->mReadThreadId);
        }
    }
}
