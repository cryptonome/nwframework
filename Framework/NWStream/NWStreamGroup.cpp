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

#include "NWStreamGroup.h"
#include "INWStream.h"
#include "NWStream.h"
#include "NWCriticalSection.h"
#include "NWStreamMedia.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamGroupWrite::NWStreamGroupWrite() :
    mInit(false),
    mWrite(true),
    mStartTimeAbs(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamGroupWrite::init(const char* _name)
{
    bool bOK = true;

    if (!isOk())
    {
        mName = _name;
        mWrite = true;
        mInit = true;
        mStartTimeAbs = 0;
        mCSStartTimeAbs = NWCriticalSection::create();
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamGroupWrite::done()
{
    if (isOk())
    {
        if ( mWrite )
        {
            int streams = getNumStreams();
            for ( int i = 0 ; i < streams ; ++i )
            {
                INWStreamWriter* itStream = getStream(i);
                DISPOSE(itStream);
            }
        }
        mStreams.clear();

        NWCriticalSection::destroy(mCSStartTimeAbs);
        mInit = false;
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int NWStreamGroupWrite::getNumStreams() const
{
    return (int)mStreams.size();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
INWStreamWriter* NWStreamGroupWrite::getStream(int _index)
{
    ASSERT(_index >= 0 && _index < getNumStreams());
    return mStreams[_index];
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
INWStreamWriter* NWStreamGroupWrite::getStreamByType(ENWStreamType _type, ENWStreamSubType _subType)
{
    INWStreamWriter* stream = 0;

    int streams = getNumStreams();
    for ( int i = 0 ; stream == 0 && i < streams ; ++i )
    {
        INWStreamWriter* itStream = getStream(i);
        if ( itStream->getType() == _type && itStream->getSubType() == _subType )
            stream = itStream;
    }

    return stream;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
INWStreamWriter* NWStreamGroupWrite::getStreamByMediaType(ENWStreamMediaType _mediaType)
{
    INWStreamWriter* stream = 0;

    int streams = getNumStreams();
    for ( int i = 0 ; stream == 0 && i < streams ; ++i )
    {
        INWStreamWriter* itStream = getStream(i);
        if ( itStream->getType() == NWSTREAM_TYPE_MEDIA )
        {
            NWStreamMedia* streamMedia = (NWStreamMedia*)itStream;
            if ( streamMedia->getMediaType() == _mediaType )
                stream = itStream;
        }
    }

    return stream;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamGroupWrite::addStream(INWStreamWriter* _stream)
{
    //ASSERT(getStreamByType(_stream->getType(),_stream->getSubType()) == 0);

    mStreams.push_back(_stream);
    if ( mWrite )
        ((NWStreamWriter*)_stream)->setStreamGroupWrite(this);
    /*else
        ((NWStream*)_stream)->setStreamGroupRead(this);*/
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamGroupWrite::disableWrite(bool _disable)
{
    LOG("NWStreamGroupWrite::disable (0x%x) (%d)",this,_disable?1:0);
    int streams = getNumStreams();
    for ( int i = 0 ; i < streams ; ++i )
    {
        INWStreamWriter* itStream = getStream(i);
        itStream->disableWrite(_disable);
    }
}





//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamGroupRead::NWStreamGroupRead() :
    mInit(false),
    mStartTimeAbs(0)
{
}



//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamGroupRead::init(const char* _name)
{
    bool bOK = true;

    if (!isOk())
    {
        mName = _name;
        mInit = true;
        mStartTimeAbs = 0;
        mCSStartTimeAbs = NWCriticalSection::create();
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamGroupRead::done()
{
    if (isOk())
    {
        int streams = getNumStreams();
        for ( int i = 0 ; i < streams ; ++i )
        {
            INWStreamReader* itStream = getStream(i);
            DISPOSE(itStream);
        }
        mStreams.clear();

        NWCriticalSection::destroy(mCSStartTimeAbs);
        mInit = false;
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int NWStreamGroupRead::getNumStreams() const
{
    return (int)mStreams.size();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
INWStreamReader* NWStreamGroupRead::getStream(int _index)
{
    ASSERT(_index >= 0 && _index < getNumStreams());
    return mStreams[_index];
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
INWStreamReader* NWStreamGroupRead::getStreamByMediaType(ENWStreamMediaType _mediaType)
{
    INWStreamReader* stream = 0;

    int streams = getNumStreams();
    for ( int i = 0 ; stream == 0 && i < streams ; ++i )
    {
        INWStreamReader* itStream = getStream(i);
        INWStreamWriter* streamWriter = itStream->getStream();
        if ( streamWriter->getType() == NWSTREAM_TYPE_MEDIA )
        {
            NWStreamMedia* streamMedia = (NWStreamMedia*)streamWriter;
            if ( streamMedia->getMediaType() == _mediaType )
                stream = itStream;
        }
    }

    return stream;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamGroupRead::disableRead(bool _disable)
{
    LOG("NWStreamGroupRead::disable (0x%x) (%d)",this,_disable?1:0);
    int streams = getNumStreams();
    for ( int i = 0 ; i < streams ; ++i )
    {
        INWStreamReader* itStream = getStream(i);
        itStream->disableRead(_disable);
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamGroupRead::addStream(INWStreamReader* _stream)
{
    //ASSERT(getStreamByType(_stream->getType(),_stream->getSubType()) == 0);

    mStreams.push_back(_stream);
    ((NWStreamReader*)_stream)->setStreamGroupRead(this);
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
u64 NWStreamGroupRead::getStartTimeAbs() const
{
    return mStartTimeAbs;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamGroupRead::setStartTimeAbs(u64 _time)
{
    mCSStartTimeAbs->enter();
    if ( _time > mStartTimeAbs )
        mStartTimeAbs = _time;
    mCSStartTimeAbs->leave();
}

