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

#include "DSFilterRenderStreamVideo.h"
#include "DSUtils.h"
#include "INWStream.h"
#include "NWStreamBlockVideo.h"


// {19412A2E-9D07-4a2e-A721-B482DB4B32AD}
static const GUID CLSID_FilterRenderStreamVideo = 
{ 0x19412a2e, 0x9d07, 0x4a2e, { 0xa7, 0x21, 0xb4, 0x82, 0xdb, 0x4b, 0x32, 0xad } };


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterRenderStreamVideo* DSFilterRenderStreamVideo::createInstance(INWStreamWriter* _stream)
{
    HRESULT hr = S_OK;
    DSFilterRenderStreamVideo* filter = NEW DSFilterRenderStreamVideo(&hr,_stream);

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
DSFilterRenderStreamVideo::DSFilterRenderStreamVideo(HRESULT * phr, INWStreamWriter* _stream) :
    CBaseRenderer( CLSID_FilterRenderStreamVideo, NAME("CLSID_FilterRenderStreamVideo"), 0, phr ),
    mStream(_stream)
{
    bool bOK = _stream->getType() == NWSTREAM_TYPE_MEDIA && _stream->getSubType() == NWSTREAM_SUBTYPE_MEDIA_VIDEO;
    ASSERT(bOK);

    if ( !bOK && *phr == S_OK )
        *phr = E_FAIL;

    mHeight = -1;
    mWidth = -1;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterRenderStreamVideo::~DSFilterRenderStreamVideo()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamVideo::CheckMediaType(const CMediaType *pmtIn)
{
    CheckPointer(pmtIn,E_POINTER);

    // the major type must match
    if( *pmtIn->Type() != MEDIATYPE_Video )
    {
        return E_INVALIDARG;
    }

    // the sub type must match
    if( *pmtIn->Subtype() != MEDIASUBTYPE_RGB24 )
    {
        return E_INVALIDARG;
    }

    // the format must match
    if( *pmtIn->FormatType() != FORMAT_VideoInfo )
    {
        return E_INVALIDARG;
    }

    // TODO: we could do more here to ensure the image is right-side up
    // by looking at the bitmap info header in the VIDEOINFO struct

    return 0;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamVideo::DoRenderSample(IMediaSample *pMediaSample)
{
    LOG("DSFilterRenderStreamVideo::DoRenderSample (%d)",rand());

    NWStreamBlockVideo* streamBlock = NEW NWStreamBlockVideo();
    streamBlock->init();

    // Copy the contents of the mediasample to the streamblock
    CheckPointer(pMediaSample,E_POINTER);
    BYTE* bits = 0;
    pMediaSample->GetPointer(&bits);
    long len = pMediaSample->GetActualDataLength();
    unsigned char* frameBufferSrc = (unsigned char*)bits;
    int stride = len/mHeight;
    ASSERT( (stride*mHeight) == len );
    streamBlock->setFrameBufferData(mWidth,mHeight,stride,frameBufferSrc);

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
HRESULT DSFilterRenderStreamVideo::SetMediaType(const CMediaType *pmt)
{
    ASSERT(mHeight == -1);
    ASSERT(mWidth == -1);

    // the major type must match
    ASSERT( *pmt->Type() == MEDIATYPE_Video );
    ASSERT( *pmt->Subtype() == MEDIASUBTYPE_RGB24 );
    ASSERT( *pmt->FormatType() == FORMAT_VideoInfo );

    // Store Size
    VIDEOINFOHEADER* pVIH = (VIDEOINFOHEADER*)pmt->Format();
    mHeight = pVIH->bmiHeader.biHeight;
    mWidth = pVIH->bmiHeader.biWidth;

    NWStreamBlockVideo* streamBlock = NEW NWStreamBlockVideo();
    streamBlock->init();

    // set null frame
    streamBlock->setFrameBufferData(mWidth,mHeight,-1,0);

    // Set the time
    streamBlock->setTime(0);

    // Add the streamblock to the stream
    mStream->writeBlock(streamBlock,false);

    return NOERROR;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamVideo::Active()
{
    return CBaseRenderer::Active();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamVideo::Inactive()
{
    return CBaseRenderer::Inactive();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamVideo::Notify(IBaseFilter * pSender, Quality q)
{
    ASSERT(false);
    return NO_ERROR;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSFilterRenderStreamVideo::EndOfStream()
{
    // Send end block
    NWStreamBlockVideo* streamBlock = NEW NWStreamBlockVideo();
    streamBlock->init();
    streamBlock->setEnd(true);
    mStream->writeBlock(streamBlock);

    return CBaseRenderer::EndOfStream();
}
