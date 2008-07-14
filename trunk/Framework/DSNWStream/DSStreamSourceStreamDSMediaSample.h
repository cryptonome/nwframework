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
