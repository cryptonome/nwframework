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

#include "DSFilterRenderDSMediaSample.h"

#include "DSUtils.h"
#include "INWStream.h"
#include "NWStreamBlockDSMediaSample.h"
#include "NWStreamDSMediaSample.h"


// {C2F884EF-8D62-4a1f-BEFA-A26CEB28BE0D}
static const GUID CLSID_FilterRenderStreamDSMediaSample = 
{ 0xc2f884ef, 0x8d62, 0x4a1f, { 0xbe, 0xfa, 0xa2, 0x6c, 0xeb, 0x28, 0xbe, 0xd } };


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterRenderStreamDSMediaSample* DSFilterRenderStreamDSMediaSample::createInstance(INWStreamWriter* _stream)
{
    HRESULT hr = S_OK;
    DSFilterRenderStreamDSMediaSample* filter = NEW DSFilterRenderStreamDSMediaSample(&hr,_stream);

    if ( hr != S_OK )
        DS_RELEASE(filter);

    return filter;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterRenderStreamDSMediaSample::DSFilterRenderStreamDSMediaSample(HRESULT * phr, INWStreamWriter* _stream) :
    CBaseRenderer( CLSID_FilterRenderStreamDSMediaSample, NAME("DSFilterRenderStreamDSMediaSample"), 0, phr ),
    mStream(_stream)
{
    bool bOK = _stream->getType() == NWSTREAM_TYPE_MEDIA && _stream->getSubType() == NWSTREAM_SUBTYPE_MEDIA_DS_MEDIASAMPLE;
    ASSERT(bOK);

    if ( !bOK && *phr == S_OK )
        *phr = E_FAIL;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterRenderStreamDSMediaSample::~DSFilterRenderStreamDSMediaSample()
{
    int a = 0;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamDSMediaSample::CheckMediaType(const CMediaType *pmtIn)
{
    CheckPointer(pmtIn,E_POINTER);

    HRESULT hr = E_INVALIDARG;

    if ( this->m_State != State_Running && this->m_State != State_Paused )
    {
        NWStreamDSMediaSample* streamDS = (NWStreamDSMediaSample*)mStream;

        if ( streamDS->getAcceptedMediaType() )
        {
            CMediaType mt(*streamDS->getAcceptedMediaType());

            if ( mt.MatchesPartial(pmtIn) == TRUE )
                hr = S_OK;
        }
        else
        {
            ASSERT(false);
            hr = S_FALSE;
        }
    }
    else
    {
        hr = S_OK;
    }

    return hr;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamDSMediaSample::DoRenderSample(IMediaSample *pMediaSample)
{
    NWStreamBlockDSMediaSample* streamBlock = NEW NWStreamBlockDSMediaSample();
    streamBlock->init();
        
    // Copy the contents of the mediasample to the streamblock
    CheckPointer(pMediaSample,E_POINTER);
    streamBlock->setMediaSample(pMediaSample);

    // Set the time
    REFERENCE_TIME timeStart(0);
    REFERENCE_TIME timeEnd(0);
    if ( pMediaSample->GetTime(&timeStart,&timeEnd) == S_OK )
    {
        u64 time = timeStart;
        streamBlock->setTime(time);
    }
    else
    {
        ASSERT(false);
    }

    // Add the streamblock to the stream
    mStream->writeBlock(streamBlock);

    return NOERROR;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamDSMediaSample::SetMediaType(const CMediaType *pmt)
{
    /*NWStreamBlockDSMediaSample* streamBlock = NEW NWStreamBlockDSMediaSample();
    streamBlock->init();

    // Set the time
    streamBlock->setTime(0);

    // Add the streamblock to the stream
    mStream->writeBlock(streamBlock,false);*/

    HRESULT hr = CheckMediaType(pmt);
    return hr;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamDSMediaSample::Active()
{
    return CBaseRenderer::Active();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamDSMediaSample::Inactive()
{
    return CBaseRenderer::Inactive();
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamDSMediaSample::EndOfStream()
{
    // Send end block
    NWStreamBlockDSMediaSample* streamBlock = NEW NWStreamBlockDSMediaSample();
    streamBlock->init();
    streamBlock->setEnd(true);
    mStream->writeBlock(streamBlock);

    return CBaseRenderer::EndOfStream();
}

