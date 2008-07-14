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

#include "DSStreamSourceStreamVideo.h"
#include "DSFilterSourceStreamVideo.h"
#include "INWStreamBlock.h"
#include "NWStream.h"
#include "NWStreamBlockVideo.h"


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSStreamSourceStreamVideo::DSStreamSourceStreamVideo(HRESULT *phr, DSFilterSourceStreamVideo *pParent, LPCWSTR pPinName, INWStreamReader* _stream) :
    CSourceStream(NAME("StreamSourceStreamVideo"),phr, pParent, pPinName)
{
    ASSERT(phr);
    mStream = _stream;
    mWidth = -1;
    mHeight = -1;
    mTime = 0;
    mLastTime = 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSStreamSourceStreamVideo::~DSStreamSourceStreamVideo()
{
}



//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamVideo::DecideBufferSize(IMemAllocator* pAlloc, ALLOCATOR_PROPERTIES* pProperties)
{
    CheckPointer(pAlloc,E_POINTER);
    CheckPointer(pProperties,E_POINTER);

    CAutoLock cAutoLock(m_pFilter->pStateLock());
    HRESULT hr = NOERROR;

    VIDEOINFO *pvi = (VIDEOINFO *) m_mt.Format();
    pProperties->cBuffers = 1;
    pProperties->cbBuffer = pvi->bmiHeader.biSizeImage;

    ASSERT(pProperties->cbBuffer);

    // Ask the allocator to reserve us some sample memory, NOTE the function
    // can succeed (that is return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted

    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties,&Actual);
    if(FAILED(hr))
    {
        return hr;
    }

    // Is this allocator unsuitable

    if(Actual.cbBuffer < pProperties->cbBuffer)
    {
        return E_FAIL;
    }

    // Make sure that we have only 1 buffer (we erase the ball in the
    // old buffer to save having to zero a 200k+ buffer every time
    // we draw a frame)

    ASSERT(Actual.cBuffers == 1);
    return NOERROR;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamVideo::FillBuffer(IMediaSample *pms)
{
    CheckPointer(pms,E_POINTER);

    BYTE *pData = 0;
    pms->GetPointer(&pData);
    long lDataLen = pms->GetSize();

    ZeroMemory(pData, lDataLen);
    {
        u64 time = 0;
        if ( fillNextFrame(pData,lDataLen,time) )
        {
            // Set time
            REFERENCE_TIME timeStart = (REFERENCE_TIME)time;
            REFERENCE_TIME timeEnd = timeStart;
            pms->SetTime(&timeStart,&timeEnd);
        }
        else
            return S_FALSE;
    }

    pms->SetSyncPoint(TRUE);
    return NOERROR;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamVideo::CheckMediaType(const CMediaType *pMediaType)
{
    CheckPointer(pMediaType,E_POINTER);


    // we only want fixed size video
    //
    if( *(pMediaType->Type()) != MEDIATYPE_Video )
    {
        return E_INVALIDARG;
    }
    if( !pMediaType->IsFixedSize( ) ) 
    {
        return E_INVALIDARG;
    }
    if( *pMediaType->Subtype( ) != MEDIASUBTYPE_RGB24 )
    {
        return E_INVALIDARG;
    }
    if( *pMediaType->FormatType( ) != FORMAT_VideoInfo )
    {
        return E_INVALIDARG;
    }

    // Get the format area of the media type
    //
    VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pMediaType->Format();

    if (pvi == NULL)
    {
        return E_INVALIDARG;
    }

    if( pvi->bmiHeader.biHeight < 0 )
    {
        return E_INVALIDARG;
    }

    return S_OK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamVideo::GetMediaType(int iPosition, CMediaType* pmt)
{
    CheckPointer(pmt,E_POINTER);

    CAutoLock cAutoLock(m_pFilter->pStateLock());
    if(iPosition < 0)
    {
        return E_INVALIDARG;
    }
    if(iPosition > 0)
    {
        return VFW_S_NO_MORE_ITEMS;
    }

    // Extract the size from the first queued frame (which is an
    // special frame to send the size)
    if ( mWidth == -1 && mHeight == -1 )
    {
        getSize();
    }

    VIDEOINFOHEADER vih;
    memset( &vih, 0, sizeof( vih ) );
    vih.bmiHeader.biCompression = BI_RGB;
    vih.bmiHeader.biBitCount    = 24;
    vih.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
    vih.bmiHeader.biWidth        = mWidth;
    vih.bmiHeader.biHeight       = mHeight;
    vih.bmiHeader.biPlanes       = 1;
    vih.bmiHeader.biSizeImage    = GetBitmapSize(&vih.bmiHeader);
    vih.bmiHeader.biClrImportant = 0;
    vih.AvgTimePerFrame = 0;//UNITS * 1 / 15; // TODO

    pmt->SetType(&MEDIATYPE_Video);
    pmt->SetFormatType(&FORMAT_VideoInfo);
    pmt->SetFormat( (BYTE*) &vih, sizeof( vih ) );
    pmt->SetSubtype(&MEDIASUBTYPE_RGB24);
    pmt->SetSampleSize(vih.bmiHeader.biSizeImage);

    return NOERROR;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamVideo::Notify(IBaseFilter * pSender, Quality q)
{

    return NOERROR;
}



//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSStreamSourceStreamVideo::getSize()
{
    if ( mStream )
        getSizeFromStream();
    else
        getSizeProcedural();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSStreamSourceStreamVideo::getSizeFromStream()
{
    ASSERT(false);
    /*INWStreamBlock* streamBlock = mStream->readBlock(false);
    ASSERT(streamBlock->getType() == NWSTREAM_TYPE_MEDIA && streamBlock->getSubType() == NWSTREAM_SUBTYPE_MEDIA_VIDEO);
    NWStreamBlockVideo* videoBlock = (NWStreamBlockVideo*)streamBlock;

    ASSERT(videoBlock->getFrameBuffer() == 0);

    mWidth = videoBlock->getWidth();
    mHeight = videoBlock->getHeight();

    DISPOSE(streamBlock);*/
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSStreamSourceStreamVideo::getSizeProcedural()
{
    mWidth = 320;
    mHeight = 240;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSStreamSourceStreamVideo::fillNextFrame(unsigned char* _buffer, int _buffersize, u64& time_)
{
    if ( mStream )
        return fillNextFrameFromStream(_buffer,_buffersize,time_);
    else
        return fillNextFrameProcedural(_buffer,_buffersize,time_);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSStreamSourceStreamVideo::fillNextFrameFromStream(unsigned char* _buffer, int _buffersize, u64& time_)
{
    bool EOS = false;
    time_ = 0;

    // Este trozo esta comentado para poder hacer el cambio del INWStream al INWStreamReader
    ASSERT(false);
    /*while ( time_ == 0 )
    {
        ASSERT(mStream->getType() == NWSTREAM_TYPE_MEDIA && mStream->getSubType() == NWSTREAM_SUBTYPE_MEDIA_VIDEO);
        INWStreamBlock* streamBlock = mStream->readBlock();
        NWStreamBlockMedia* mediaBlock = (NWStreamBlockMedia*)streamBlock;

        if ( streamBlock && !mediaBlock->IsEnd() )
        {
            ASSERT(streamBlock->getType() == NWSTREAM_TYPE_MEDIA && streamBlock->getSubType() == NWSTREAM_SUBTYPE_MEDIA_VIDEO);
            NWStreamBlockVideo* videoBlock = (NWStreamBlockVideo*)streamBlock;

            // Set frame buffer
            const unsigned char* frameBuffer = videoBlock->getFrameBuffer();
            ASSERT(frameBuffer);
            ASSERT(videoBlock->getWidth() == mWidth);
            ASSERT(videoBlock->getHeight() == mHeight);
            int frameBufferSize = videoBlock->getHeight()*videoBlock->getStride();
            ASSERT(frameBufferSize == _buffersize);
            memcpy(_buffer,frameBuffer,frameBufferSize);

            u64 time = videoBlock->getTime()+1;
            mLastTime = time-1;
            mTime = mStream->getStartTimeAbs();
            if ( time >= mTime )
                time_ = time - mTime;
            else
                time_ = 0;

        }
        else
        {
            memset(_buffer,0,_buffersize);
            time_ = mLastTime;
            if ( mediaBlock && mediaBlock->IsEnd() )
                EOS = true;
        }
        DISPOSE(streamBlock);
    }
    time_ = time_ - 1;*/



    // Log time
    /*CDisp disp = CDisp(CRefTime((REFERENCE_TIME)time_));
    const char* str = disp;
    LOG("DSStreamSourceStreamVideo time(%s)", str);*/

    return !EOS;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSStreamSourceStreamVideo::fillNextFrameProcedural(unsigned char* _buffer, int _buffersize, u64& time_)
{
    for ( int i = 0 ; i < _buffersize ; ++i )
    {
        _buffer[i] = rand();
    }

    time_ = mTime;
    //LOG("New frame (%d)\n",time_);
    mTime += 666666; // 15 fps

    return true;
}

extern u64 g_chanka;

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamVideo::OnThreadCreate()
{
    /*if ( mLastTime > g_chanka )
        g_chanka = mLastTime;*/
    /*mLastTime = g_chanka;

    mTime = mLastTime;*/
    //mStream->setStartTimeAbs(mLastTime);


    /*CDisp disp = CDisp(CRefTime((REFERENCE_TIME)mTime));
    const char* str = disp;
    LOG("VideoLastTime(%s)",str);*/

    return NO_ERROR;
}

