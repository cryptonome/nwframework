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

#include "DSStreamSourceStreamAudio.h"
#include "DSFilterSourceStreamAudio.h"
#include "INWStreamBlock.h"
#include "NWStream.h"
#include "NWStreamBlockAudio.h"
#include "wxdebug.h"


const int WaveBufferSize = 16*1024;     // Size of each allocated buffer
                                        // Originally used to be 2K, but at
                                        // 44khz/16bit/stereo you would get
                                        // audio breaks with a transform in the
                                        // middle.

const int BITS_PER_BYTE = 8;


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSStreamSourceStreamAudio::DSStreamSourceStreamAudio(HRESULT *phr, DSFilterSourceStreamAudio *pParent, LPCWSTR pPinName, INWStreamReader* _stream) :
    CSourceStream(NAME("StreamSourceStreamAudio"),phr, pParent, pPinName)
{
    ASSERT(phr);
    mStream = _stream;

    mBitsPerSample = -1;
    mChannels = -1;
    mSamplesPerSec = -1;
    mTime = 0;
    mLastTime = 0;
    mSBAvailableSamples = 0;
    mStreamBlock = 0;
    mSBBuffer = 0;

    mEOS = false;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSStreamSourceStreamAudio::~DSStreamSourceStreamAudio()
{
    DISPOSE(mStreamBlock);
}



//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamAudio::DecideBufferSize(IMemAllocator* pAlloc, ALLOCATOR_PROPERTIES* pProperties)
{
    CheckPointer(pAlloc,E_POINTER);
    CheckPointer(pProperties,E_POINTER);

    CAutoLock cAutoLock(m_pFilter->pStateLock());
    HRESULT hr = NOERROR;

    WAVEFORMATEX *pwfexCurrent = (WAVEFORMATEX*)m_mt.Format();

    if(WAVE_FORMAT_PCM == pwfexCurrent->wFormatTag)
    {
        pProperties->cbBuffer = WaveBufferSize;
    }
    else
    {
        return E_FAIL;
    }
    /*else
    {
        // This filter only supports two formats: PCM and ADPCM. 
        ASSERT(WAVE_FORMAT_ADPCM == pwfexCurrent->wFormatTag);

        pProperties->cbBuffer = pwfexCurrent->nBlockAlign;

        MMRESULT mmr = acmStreamSize(m_hPCMToMSADPCMConversionStream,
                                     pwfexCurrent->nBlockAlign,
                                     &m_dwTempPCMBufferSize,
                                     ACM_STREAMSIZEF_DESTINATION);

        // acmStreamSize() returns 0 if no error occurs.
        if(0 != mmr)
        {
            return E_FAIL;
        }
    }*/

    int nBitsPerSample = pwfexCurrent->wBitsPerSample;
    int nSamplesPerSec = pwfexCurrent->nSamplesPerSec;
    int nChannels = pwfexCurrent->nChannels;

    pProperties->cBuffers = (nChannels * nSamplesPerSec * nBitsPerSample) / 
                            (pProperties->cbBuffer * BITS_PER_BYTE);

    // Get 1/2 second worth of buffers
    pProperties->cBuffers /= 2;
    if(pProperties->cBuffers < 1)
        pProperties->cBuffers = 1 ;

    // Ask the allocator to reserve us some sample memory, NOTE the function
    // can succeed (that is return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted

    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties,&Actual);
    if(FAILED(hr))
    {
        ASSERT(false);
        return hr;
    }

    // Is this allocator unsuitable

    if(Actual.cbBuffer < pProperties->cbBuffer)
    {
        return E_FAIL;
    }

    return NOERROR;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSStreamSourceStreamAudio::fillNextFrame(unsigned char* _buffer, int _buffersize, u64& time_)
{
    if ( mStream )
        fillNextFrameFromStream(_buffer,_buffersize,time_);
    else
        fillNextFrameProcedural(_buffer,_buffersize,time_);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSStreamSourceStreamAudio::fillNextFrameFromStream(unsigned char* _buffer, int _buffersize, u64& time_)
{
    ASSERT(mStream->getType() == NWSTREAM_TYPE_MEDIA && mStream->getSubType() == NWSTREAM_SUBTYPE_MEDIA_AUDIO);

    time_ = 0;

    while ( time_ == 0 )
    {
        int bytesPerSample = mChannels * (mBitsPerSample/8);
        int samples = _buffersize/bytesPerSample;
        ASSERT(samples > 0);
        ASSERT(_buffersize == (samples*bytesPerSample));
        unsigned char* buffer = _buffer;
        u64 time = 0;
        bool firstIteration = true;
        while ( samples )
        {
            u64 timeAux = 0;
            int samplesProcessed = processNewSamplesFromStream(buffer, samples, timeAux);
            samples -= samplesProcessed;
            ASSERT(samples >= 0);
            buffer += samplesProcessed*bytesPerSample;

            if ( firstIteration )
            {
                time = timeAux;
                firstIteration = false;
            }
        }
        
        // Update time
        /*time_ = mTime;
        samples = _buffersize / bytesPerSample;
        mTime += ((u64)(samples) * (u64)(10000000)) / (u64)(mSamplesPerSec);*/
        time = time + 1;
        mTime = mStream->getStartTimeAbs();
        mLastTime = time;
        if ( time >= mTime )
            time_ = time - mTime;
        else
            time_ = 0;
    }
    time_ = time_ - 1;
    
    // Log time
    /*CDisp disp = CDisp(CRefTime((REFERENCE_TIME)time_));
    const char* str = disp;
    LOG("DSStreamSourceStreamAudio time(%s)", str);*/
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int DSStreamSourceStreamAudio::processNewSamplesFromStream(unsigned char* _buffer, int _samples, u64& time_)
{
    int samplesProcessed = 0;

    // Este trozo esta comentado para poder hacer el cambio del INWStream al INWStreamReader
    ASSERT(false);
    // Read a new block if needed
    /*NWStreamBlockAudio* audioBlock = (NWStreamBlockAudio*)mStreamBlock;
    if ( audioBlock == 0 )
    {
        mStreamBlock = mStream->readBlock();

        if ( mStreamBlock )
        {
            ASSERT(mStreamBlock->getType() == NWSTREAM_TYPE_MEDIA && mStreamBlock->getSubType() == NWSTREAM_SUBTYPE_MEDIA_AUDIO);
            audioBlock = (NWStreamBlockAudio*)mStreamBlock;

            if ( !audioBlock->IsEnd() )
            {
                ASSERT(mBitsPerSample == audioBlock->getBitsPerSample());
                ASSERT(mChannels == audioBlock->getChannels());
                ASSERT(mSamplesPerSec == audioBlock->getSamplesPerSec());
            }
            mSBAvailableSamples = audioBlock->getSamples();
            mSBBuffer = audioBlock->getBuffer();
        }
        else
        {
            mSBAvailableSamples = 0;
            mSBBuffer = 0;
        }
    }
    

    if ( audioBlock && !audioBlock->IsEnd() )
    {
        // Calc time
        int samplesOffset = audioBlock->getSamples() - mSBAvailableSamples;
        u64 timeOffset = ((u64)(samplesOffset) * (u64)(10000000)) / (u64)(mSamplesPerSec);
        time_ = audioBlock->getTime() + timeOffset;

        
        ASSERT(_samples > 0 && mSBAvailableSamples > 0 );
        samplesProcessed = (_samples < mSBAvailableSamples) ? _samples : mSBAvailableSamples;
        mSBAvailableSamples -= samplesProcessed;
        
        int bytesPerSample = mChannels * (mBitsPerSample/8);
        int bytesToCopy = samplesProcessed*bytesPerSample;    
        
        memcpy(_buffer,mSBBuffer,bytesToCopy);
        mSBBuffer += bytesToCopy;

        if ( mSBAvailableSamples == 0 )
        {
            DISPOSE(mStreamBlock);
            mStreamBlock = 0;
            mSBAvailableSamples = 0;
            mSBBuffer = 0;
        }
    }
    else
    {
        samplesProcessed = _samples;
        int bytesPerSample = mChannels * (mBitsPerSample/8);
        int bytesToCopy = samplesProcessed*bytesPerSample;    
        memset(_buffer,0,bytesToCopy);
        time_ = mLastTime;
        if ( audioBlock && audioBlock->IsEnd() )
            mEOS = true;
    }*/

    return samplesProcessed;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSStreamSourceStreamAudio::fillNextFrameProcedural(unsigned char* _buffer, int _buffersize, u64& time_)
{
    //memset(_buffer,0,_buffersize);    
    for ( int i = 0 ; i < _buffersize ; ++i )
        _buffer[i] = rand();

    // Update time
    time_ = mTime;
    int bytesPerSample = mChannels * (mBitsPerSample/8);
    int samples = _buffersize / bytesPerSample;
    mTime += ((u64)(samples) * (u64)(10000000)) / (u64)(mSamplesPerSec);
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamAudio::FillBuffer(IMediaSample *pms)
{
    CheckPointer(pms,E_POINTER);

    if ( !mEOS )
    {
        BYTE *pData = 0;
        pms->GetPointer(&pData);
        long lDataLen = pms->GetSize();

        ZeroMemory(pData, lDataLen);
        {
            u64 time = 0;
            fillNextFrame(pData,lDataLen,time);

            // Set time
            REFERENCE_TIME timeStart = (REFERENCE_TIME)time;
            REFERENCE_TIME timeEnd = timeStart;
            pms->SetTime(&timeStart,&timeEnd);
        }

        pms->SetSyncPoint(TRUE);

        HRESULT hr = pms->SetActualDataLength(pms->GetSize());
        if (FAILED(hr))
            return hr;
    }
    else
    {
        return S_FALSE;
    }

    // Set the sample's properties.
    /*hr = pms->SetPreroll(FALSE);
    if (FAILED(hr)) {
        return hr;
    }

    hr = pms->SetMediaType(NULL);
    if (FAILED(hr)) {
        return hr;
    }
   
    hr = pms->SetDiscontinuity(!m_fFirstSampleDelivered);
    if (FAILED(hr)) {
        return hr;
    }
    
    hr = pms->SetSyncPoint(!m_fFirstSampleDelivered);
    if (FAILED(hr)) {
        return hr;
    }*/


    return NOERROR;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
/*HRESULT DSStreamSourceStreamAudio::CompleteConnect(IPin *pReceivePin)
{
    WAVEFORMATEX *pwfexCurrent = (WAVEFORMATEX*)m_mt.Format();

    return S_OK;
}*/

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamAudio::CheckMediaType(const CMediaType *pMediaType)
{
    CheckPointer(pMediaType,E_POINTER);


    // we only want fixed size Audio
    //
    if( *(pMediaType->Type()) != MEDIATYPE_Audio )
    {
        return E_INVALIDARG;
    }
    if( !pMediaType->IsFixedSize( ) ) 
    {
        return E_INVALIDARG;
    }
    if( *pMediaType->Subtype( ) != MEDIASUBTYPE_PCM )
    {
        return E_INVALIDARG;
    }
    if( *pMediaType->FormatType( ) != FORMAT_WaveFormatEx )
    {
        return E_INVALIDARG;
    }

    //// Get the format area of the media type
    ////
    //WAVEFORMATEX* waveFormat = (WAVEFORMATEX*)pMediaType->Format();

    //if (waveFormat == NULL)
    //{
    //    return E_INVALIDARG;
    //}

    //if( waveFormat-> )
    //{
    //    return E_INVALIDARG;
    //}

    return S_OK;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamAudio::GetMediaType(int iPosition, CMediaType* pmt)
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

    // Este trozo esta comentado para poder hacer el cambio del INWStream al INWStreamReader
    ASSERT(false);
    // Extract the properties from the first queued frame
    /*if ( mBitsPerSample == -1 && mChannels == -1 && mSamplesPerSec == -1 )
    {
        if ( mStream )
        {
            INWStreamBlock* streamBlock = mStream->readBlock(false);
            ASSERT(streamBlock->getType() == NWSTREAM_TYPE_MEDIA && streamBlock->getSubType() == NWSTREAM_SUBTYPE_MEDIA_AUDIO);
            NWStreamBlockAudio* audioBlock = (NWStreamBlockAudio*)streamBlock;

            ASSERT(audioBlock->getBuffer() == 0);

            mBitsPerSample = audioBlock->getBitsPerSample();
            mChannels = audioBlock->getChannels();
            mSamplesPerSec = audioBlock->getSamplesPerSec();

            DISPOSE(streamBlock);
        }
        else
        {
            mBitsPerSample = 16;
            mChannels = 2;
            mSamplesPerSec = 22050;
        }
    }*/

    WAVEFORMATEX* waveFormat = (WAVEFORMATEX *)pmt->AllocFormatBuffer(sizeof(WAVEFORMATEX));
    if(NULL == waveFormat)
    {
        return E_OUTOFMEMORY;
    }

    memset(waveFormat, 0, sizeof(waveFormat));
    waveFormat->cbSize = 0;//sizeof(*waveFormat);
    waveFormat->nChannels = mChannels;
    waveFormat->nSamplesPerSec = mSamplesPerSec;
    waveFormat->wBitsPerSample = mBitsPerSample;
    waveFormat->nBlockAlign = waveFormat->nChannels * (waveFormat->wBitsPerSample/8);
    waveFormat->nAvgBytesPerSec = waveFormat->nSamplesPerSec * waveFormat->nBlockAlign;

    ASSERT(waveFormat->nChannels <= 2); // Maybe we need other formattag for more than 2 channels streams
    waveFormat->wFormatTag = WAVE_FORMAT_PCM;

    /*pmt->SetType(&MEDIATYPE_Audio);
    pmt->SetFormatType(&FORMAT_WaveFormatEx);
    pmt->SetFormat((BYTE*)&waveFormat, sizeof(waveFormat));
    pmt->SetSubtype(&MEDIASUBTYPE_PCM);
    pmt->SetSampleSize(waveFormat->nBlockAlign);*/

    HRESULT hr = CreateAudioMediaType(waveFormat, pmt, FALSE);
    return hr;
    //return NOERROR;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamAudio::Notify(IBaseFilter * pSender, Quality q)
{

    return NOERROR;
}

u64 g_chanka = 0;

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamAudio::OnThreadCreate()
{
    /*if ( mLastTime > g_chanka )
        g_chanka = mLastTime;
    mLastTime = g_chanka;*/
    
    //mTime = mLastTime;
    mStream->setStartTimeAbs(mLastTime);

    CDisp disp = CDisp(CRefTime((REFERENCE_TIME)mLastTime));
    const char* str = disp;
    LOG("AudioLastTime(%s)",str);

    return NO_ERROR;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamAudio::GetDeliveryBuffer(IMediaSample ** ppSample,REFERENCE_TIME * pStartTime, REFERENCE_TIME * pEndTime, DWORD dwFlags)
{
    return CSourceStream::GetDeliveryBuffer(ppSample,pStartTime,pEndTime,dwFlags);
}
