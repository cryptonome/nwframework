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
#include "PchDSNWStream.h"

#include "DSFilterSourceStreamAudio.h"
#include "DSStreamSourceStreamAudio.h"
#include "DSUtils.h"


//--------------------------------------------------------------------
// GUID
//--------------------------------------------------------------------
// {74AB5943-0ED6-4ba2-BAD7-EA8BDCE0BE31}
static const GUID CLSID_FilterSourceStreamAudio = 
{ 0x74ab5943, 0xed6, 0x4ba2, { 0xba, 0xd7, 0xea, 0x8b, 0xdc, 0xe0, 0xbe, 0x31 } };


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterSourceStreamAudio* DSFilterSourceStreamAudio::createInstance(INWStreamReader* _stream)
{
    DSFilterSourceStreamAudio* filter = NEW DSFilterSourceStreamAudio();

    if ( !filter->init(_stream) )
        DS_RELEASE(filter);

    return filter;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterSourceStreamAudio::DSFilterSourceStreamAudio() : 
    CSource(NAME("FilterSourceStreamAudio"), 0, CLSID_FilterSourceStreamAudio),
    mInit(false),
    mStream(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSFilterSourceStreamAudio::init(INWStreamReader* _stream)
{
    bool bOK = true;

    if (!isOk())
    {
        bOK = createStream(_stream);

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSFilterSourceStreamAudio::done()
{
    if (isOk())
    {
        mInit = false;
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSFilterSourceStreamAudio::createStream(INWStreamReader* _stream)
{
    CAutoLock cAutoLock(&m_cStateLock);

    bool bOK = true;

    HRESULT hr = S_OK;
    ASSERT(mStream == 0);
    mStream = NEW DSStreamSourceStreamAudio(&hr, this, L"", _stream);
    bOK = hr == S_OK;

    return bOK;
}

