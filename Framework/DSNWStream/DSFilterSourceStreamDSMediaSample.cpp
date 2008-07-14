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

#include "DSFilterSourceStreamDSMediaSample.h"
#include "DSStreamSourceStreamDSMediaSample.h"
#include "DSUtils.h"


//--------------------------------------------------------------------
// GUID
//--------------------------------------------------------------------
// {5500E796-51C0-4cb2-B31C-1318FA6EC62B}
static const GUID CLSID_FilterSourceStreamDSMediaSample = 
{ 0x5500e796, 0x51c0, 0x4cb2, { 0xb3, 0x1c, 0x13, 0x18, 0xfa, 0x6e, 0xc6, 0x2b } };


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterSourceStreamDSMediaSample* DSFilterSourceStreamDSMediaSample::createInstance(INWStreamReader* _stream)
{
    DSFilterSourceStreamDSMediaSample* filter = NEW DSFilterSourceStreamDSMediaSample();

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
DSFilterSourceStreamDSMediaSample::DSFilterSourceStreamDSMediaSample() : 
    CSource(NAME("FilterSourceStreamDSMediaSample"), 0, CLSID_FilterSourceStreamDSMediaSample),
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
bool DSFilterSourceStreamDSMediaSample::init(INWStreamReader* _stream)
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
void DSFilterSourceStreamDSMediaSample::done()
{
    if (isOk())
    {
        mInit = false;
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSFilterSourceStreamDSMediaSample::createStream(INWStreamReader* _stream)
{
    CAutoLock cAutoLock(&m_cStateLock);

    bool bOK = true;

    HRESULT hr = S_OK;
    ASSERT(mStream == 0);
    mStream = NEW DSStreamSourceStreamDSMediaSample(&hr, this, L"", _stream);
    bOK = hr == S_OK;

    return bOK;
}

