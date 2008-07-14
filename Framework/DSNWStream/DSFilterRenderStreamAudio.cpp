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
 */
#include "PchDSNWStream.h"

#include "DSFilterRenderStreamAudio.h"
#include "DSUtils.h"
#include "INWStream.h"
#include "NWStreamBlockAudio.h"


// {3B249369-C006-4d59-8DF9-013CB8FB92CA}
static const GUID CLSID_FilterRenderStreamAudio = 
{ 0x3b249369, 0xc006, 0x4d59, { 0x8d, 0xf9, 0x1, 0x3c, 0xb8, 0xfb, 0x92, 0xca } };



//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterRenderStreamAudio* DSFilterRenderStreamAudio::createInstance(INWStreamWriter* _stream)
{
    HRESULT hr = S_OK;
    DSFilterRenderStreamAudio* filter = NEW DSFilterRenderStreamAudio(&hr,_stream);

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
DSFilterRenderStreamAudio::DSFilterRenderStreamAudio(HRESULT * phr, INWStreamWriter* _stream) :
    CBaseRenderer( CLSID_FilterRenderStreamAudio, NAME("DSFilterRenderStreamAudio"), 0, phr ),
    mStream(_stream)
{
    bool bOK = _stream->getType() == NWSTREAM_TYPE_MEDIA && _stream->getSubType() == NWSTREAM_SUBTYPE_MEDIA_AUDIO;
    ASSERT(bOK);

    if ( !bOK && *phr == S_OK )
        *phr = E_FAIL;

    mBitsPerSample = -1;
    mChannels = -1;
    mSamplesPerSec = -1;
    mBytesPerSample = -1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterRenderStreamAudio::~DSFilterRenderStreamAudio()
{ 
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamAudio::CheckMediaType(const CMediaType *pmtIn)
{
    CheckPointer(pmtIn,E_POINTER);

    // the major type must match
    if( *pmtIn->Type() != MEDIATYPE_Audio )
    {
        return E_INVALIDARG;
    }

    // the sub type must match
    if( *pmtIn->Subtype() != MEDIASUBTYPE_PCM )
    {
        return E_INVALIDARG;
    }

    // the format must match
    if( *pmtIn->FormatType() != FORMAT_WaveFormatEx )
    {
        return E_INVALIDARG;
    }

    /*WAVEFORMATEX* waveFormat = (WAVEFORMATEX*)pmtIn->Format();

    if ( waveFormat->nChannels != 2 ||
         waveFormat->nSamplesPerSec != 44100 ||
         waveFormat->wBitsPerSample != 16 )
        return E_INVALIDARG;*/

    return 0;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamAudio::DoRenderSample(IMediaSample *pMediaSample)
{
    NWStreamBlockAudio* streamBlock = NEW NWStreamBlockAudio();
    streamBlock->init();
        
    // Copy the contents of the mediasample to the streamblock
    CheckPointer(pMediaSample,E_POINTER);
    BYTE* audioBuffer = 0;
    pMediaSample->GetPointer(&audioBuffer);
    long len = pMediaSample->GetActualDataLength();
    int samples = len / mBytesPerSample;
    ASSERT(samples*mBytesPerSample == len);
    streamBlock->setAudioBuffer(mBitsPerSample,mChannels,mSamplesPerSec,samples,audioBuffer);

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
HRESULT DSFilterRenderStreamAudio::SetMediaType(const CMediaType *pmt)
{
    ASSERT(mChannels == -1);
    ASSERT(mSamplesPerSec == -1);
    ASSERT(mBitsPerSample == -1);

    // the major type must match
    ASSERT( *pmt->Type() == MEDIATYPE_Audio );
    ASSERT( *pmt->Subtype() == MEDIASUBTYPE_PCM );
    ASSERT( *pmt->FormatType() == FORMAT_WaveFormatEx );

    // Store Size
    WAVEFORMATEX* waveFormat = (WAVEFORMATEX*)pmt->Format();
    mChannels = waveFormat->nChannels;
    mSamplesPerSec = waveFormat->nSamplesPerSec;
    mBitsPerSample = waveFormat->wBitsPerSample;
    mBytesPerSample = (mBitsPerSample/8) * mChannels;

    LOG("DSFilterRenderStreamAudio  SampleRate(%d) BitsPerSample(%d) Channels(%d)",mSamplesPerSec,mBitsPerSample,mChannels);

    NWStreamBlockAudio* streamBlock = NEW NWStreamBlockAudio();
    streamBlock->init();

    // set null buffer
    streamBlock->setAudioBuffer(mBitsPerSample,mChannels,mSamplesPerSec,0,0);

    // Set the time
    streamBlock->setTime(0);

    // Add the streamblock to the stream
    mStream->writeBlock(streamBlock,false);

    return NOERROR;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamAudio::Active()
{
    return CBaseRenderer::Active();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamAudio::Inactive()
{
    return CBaseRenderer::Inactive();
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamAudio::EndOfStream()
{
    // Send end block
    NWStreamBlockAudio* streamBlock = NEW NWStreamBlockAudio();
    streamBlock->init();
    streamBlock->setEnd(true);
    mStream->writeBlock(streamBlock);

    return CBaseRenderer::EndOfStream();
}
