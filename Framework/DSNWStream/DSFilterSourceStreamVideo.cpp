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

#include "DSFilterSourceStreamVideo.h"
#include "DSStreamSourceStreamVideo.h"
#include "DSUtils.h"


//--------------------------------------------------------------------
// GUID
//--------------------------------------------------------------------
// {00744768-DED4-4fc3-B976-533FEE3308AF}
static const GUID CLSID_FilterSourceStreamVideo = 
{ 0x744768, 0xded4, 0x4fc3, { 0xb9, 0x76, 0x53, 0x3f, 0xee, 0x33, 0x8, 0xaf } };


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterSourceStreamVideo* DSFilterSourceStreamVideo::createInstance(INWStreamReader* _stream)
{
    DSFilterSourceStreamVideo* filter = NEW DSFilterSourceStreamVideo();

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
DSFilterSourceStreamVideo::DSFilterSourceStreamVideo() : 
    CSource(NAME("FilterSourceStreamVideo"), 0, CLSID_FilterSourceStreamVideo),
    mInit(false),
    mStream(0)
{
    int a = 0;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSFilterSourceStreamVideo::init(INWStreamReader* _stream)
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
void DSFilterSourceStreamVideo::done()
{
    if (isOk())
    {
        //DS_RELEASE(mStream);
        mInit = false;
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSFilterSourceStreamVideo::createStream(INWStreamReader* _stream)
{
    CAutoLock cAutoLock(&m_cStateLock);

    bool bOK = true;

    HRESULT hr = S_OK;
    ASSERT(mStream == 0);
    mStream = NEW DSStreamSourceStreamVideo(&hr, this, L"", _stream);
    bOK = hr == S_OK;

    return bOK;
}
