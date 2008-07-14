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

#include "DSStreamSourceStreamDSMediaSample.h"
#include "DSFilterSourceStreamDSMediaSample.h"
#include "INWStreamBlock.h"
#include "NWStream.h"
#include "NWStreamBlockDSMediaSample.h"
#include "wxdebug.h"
#include "NWStreamDSMediaSample.h"
#include "DSUtils.h"


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSStreamSourceStreamDSMediaSample::DSStreamSourceStreamDSMediaSample(HRESULT *phr, DSFilterSourceStreamDSMediaSample *pParent, LPCWSTR pPinName, INWStreamReader* _stream) :
    CSourceStream(NAME("StreamSourceStreamDSMediaSample"),phr, pParent, pPinName)
{
    mMediaSample = 0;

    ASSERT(phr);
    mStream = _stream;
    ASSERT(mStream->getType() == NWSTREAM_TYPE_MEDIA && mStream->getSubType() == NWSTREAM_SUBTYPE_MEDIA_DS_MEDIASAMPLE);

    // Store a list with the valid mediatypes of the DSRender
    NWStreamDSMediaSample* streamMediaSample = (NWStreamDSMediaSample*)mStream->getStream();
    IPin* pin = streamMediaSample->getBeforeRenderPinOut();

    IEnumMediaTypes* enumMediaTypes = NULL;
    if ( pin->EnumMediaTypes(&enumMediaTypes) == S_OK )
    {
        AM_MEDIA_TYPE* mediaType;
        while ( enumMediaTypes->Next(1,&mediaType,NULL) == S_OK )
        {
            AM_MEDIA_TYPE* mediaTypeNew = CreateMediaType(mediaType);
            mMediaTypes.push_back(mediaTypeNew);
        }
        DS_RELEASE(enumMediaTypes);
    }

    mTime = 0;
    mLastTime = 0;

    mEOS = false;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSStreamSourceStreamDSMediaSample::~DSStreamSourceStreamDSMediaSample()
{
    for ( std::vector<AM_MEDIA_TYPE*>::iterator it = mMediaTypes.begin() ; it != mMediaTypes.end() ; ++it )
    {
        AM_MEDIA_TYPE* mediaType = *it;
        FreeMediaType(*mediaType);
    }
    mMediaTypes.clear();
}



//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamDSMediaSample::DecideBufferSize(IMemAllocator* pAlloc, ALLOCATOR_PROPERTIES* pProperties)
{
    CheckPointer(pAlloc,E_POINTER);
    CheckPointer(pProperties,E_POINTER);

    CAutoLock cAutoLock(m_pFilter->pStateLock());
    HRESULT hr = NOERROR;

    pProperties->cBuffers = 1;
    pProperties->cbBuffer = 100;

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
HRESULT DSStreamSourceStreamDSMediaSample::FillBuffer(IMediaSample *pms)
{
    CheckPointer(pms,E_POINTER);

    ASSERT(mMediaSample == pms);

    return NOERROR;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamDSMediaSample::CheckMediaType(const CMediaType *pMediaType)
{
    CheckPointer(pMediaType,E_POINTER);

    bool valid = false;
    for ( std::vector<AM_MEDIA_TYPE*>::iterator it = mMediaTypes.begin() ; !valid && it != mMediaTypes.end() ; ++it )
    {
        AM_MEDIA_TYPE* mediaType = *it;
        CMediaType mt(*mediaType);

        if ( mt.MatchesPartial(pMediaType) == TRUE )
            valid = true;
    }

    return valid ? S_OK : E_INVALIDARG;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamDSMediaSample::GetMediaType(int iPosition, CMediaType* pmt)
{
    CheckPointer(pmt,E_POINTER);

    CAutoLock cAutoLock(m_pFilter->pStateLock());
    if(iPosition < 0)
    {
        return E_INVALIDARG;
    }
    if(iPosition >= (int)mMediaTypes.size())
    {
        return VFW_S_NO_MORE_ITEMS;
    }

    pmt->Set(*mMediaTypes[iPosition]);

    return NOERROR;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamDSMediaSample::SetMediaType(const CMediaType *pMediaType)
{
    HRESULT hr = E_INVALIDARG;

    if ( CheckMediaType(pMediaType) == S_OK )
    {
        //if ( pMediaType->pbFormat != 0 )
        {
            hr = CSourceStream::SetMediaType(pMediaType);

            if(SUCCEEDED(hr))
            {
                NWStreamDSMediaSample* streamMediaSample = (NWStreamDSMediaSample*)mStream->getStream();

                streamMediaSample->setAcceptedMediaType(&m_mt);

                {
                    CMediaType mt(m_mt);
                    ASSERT(mt.MatchesPartial(pMediaType) == TRUE);
                }
            }
        }
    }

    return hr;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamDSMediaSample::Notify(IBaseFilter * pSender, Quality q)
{

    return NOERROR;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamDSMediaSample::OnThreadCreate()
{
    mStream->setStartTimeAbs(mLastTime);

    /*CDisp disp = CDisp(CRefTime((REFERENCE_TIME)mLastTime));
    const char* str = disp;
    LOG("LastTime(%s)",str);*/

    return NO_ERROR;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
HRESULT DSStreamSourceStreamDSMediaSample::GetDeliveryBuffer(IMediaSample ** ppSample,REFERENCE_TIME * pStartTime, REFERENCE_TIME * pEndTime, DWORD dwFlags)
{
    HRESULT hr = NO_ERROR;

    ASSERT(mStream->getType() == NWSTREAM_TYPE_MEDIA && mStream->getSubType() == NWSTREAM_SUBTYPE_MEDIA_DS_MEDIASAMPLE);

    bool dropFrame = true;
    while ( dropFrame && hr == NO_ERROR )
    {
        INWStreamBlock* streamBlock = mStream->readBlock();
        *ppSample = 0;
        dropFrame = false;

        if ( streamBlock != 0 )
        {
            NWStreamBlockDSMediaSample* blockDSMediaSample = (NWStreamBlockDSMediaSample*)streamBlock;

            mMediaSample = blockDSMediaSample->getMediaSample();
            *ppSample = mMediaSample;
            if ( mMediaSample )
            {
                mMediaSample->AddRef();

                // Modify the time of the MediaSample into the
                // time-space of the current graph
                REFERENCE_TIME timeStart(0);
                REFERENCE_TIME timeEnd(0);
                if ( mMediaSample->GetTime(&timeStart,&timeEnd) == S_OK )
                {
                    u64 startTimeAbs = mStream->getStartTimeAbs();

                    u64 timeStartAux = timeStart;
                    if ( timeStartAux >= startTimeAbs )
                        timeStartAux = timeStartAux - startTimeAbs;
                    else
                    {
                        dropFrame = true;
                        timeStartAux = 0;
                    }

                    u64 timeEndAux = timeEnd;
                    if ( timeEndAux >= startTimeAbs )
                        timeEndAux = timeEndAux - startTimeAbs;
                    else
                    {
                        dropFrame = true;
                        timeEndAux = 0;
                    }

                    if ( !dropFrame )
                        mLastTime = timeStart;

                    timeStart = timeStartAux;
                    timeEnd = timeEndAux;

                    if ( mMediaSample->SetTime(&timeStart,&timeEnd) != S_OK )
                    {
                        ASSERT(false);
                    }
                }
                else
                {
                    ASSERT(false);
                }

            }
            else
                hr = E_INVALIDARG;

            DISPOSE(streamBlock);
        }
        else
        {
            hr = E_INVALIDARG;
        }

        if ( dropFrame )
        {
            DS_RELEASE(mMediaSample);
            *ppSample = 0;
        }
    }

    return hr;
}

