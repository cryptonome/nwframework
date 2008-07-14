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
#ifndef DSSTREAMSOURCESTREAMDSMEDIASAMPLE_H_
#define DSSTREAMSOURCESTREAMDSMEDIASAMPLE_H_

#include "streams.h"
#include <vector>

class DSFilterSourceStreamDSMediaSample;
class INWStreamReader;
class INWStreamBlock;

//********************************************************************
//
//********************************************************************
class DSStreamSourceStreamDSMediaSample : public CSourceStream
{
public:
    DSStreamSourceStreamDSMediaSample  (HRESULT *phr, DSFilterSourceStreamDSMediaSample *pParent, LPCWSTR pPinName,INWStreamReader* _stream);
    ~DSStreamSourceStreamDSMediaSample ();

    // CBaseOutputPin
    HRESULT DecideBufferSize(IMemAllocator* _memAlloc, ALLOCATOR_PROPERTIES* _properties);

    // CSourceStream
    virtual HRESULT FillBuffer(IMediaSample* _samp);
    virtual HRESULT GetMediaType(int _position, CMediaType* _pmt);
    virtual HRESULT CheckMediaType(const CMediaType *pMediaType);
    virtual HRESULT SetMediaType(const CMediaType *pMediaType);
    //virtual HRESULT CompleteConnect(IPin *pReceivePin);
    virtual HRESULT OnThreadCreate();

    // CBasePin
    virtual HRESULT __stdcall Notify(IBaseFilter * pSender, Quality q);

    // CBaseOutputPin
    HRESULT GetDeliveryBuffer(IMediaSample ** ppSample,REFERENCE_TIME * pStartTime, REFERENCE_TIME * pEndTime, DWORD dwFlags);

    INWStreamReader* getStream() const { return mStream; }

private:
    INWStreamReader* mStream;

    u64 mTime;
    u64 mLastTime;

    bool mEOS;

    std::vector<AM_MEDIA_TYPE*> mMediaTypes;

    IMediaSample* mMediaSample;
};


#endif
