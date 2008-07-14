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
#ifndef DSSTREAMSOURCESTREAMVIDEO_H_
#define DSSTREAMSOURCESTREAMVIDEO_H_

#include "streams.h"

class DSFilterSourceStreamVideo;
class INWStreamReader;

//********************************************************************
//
//********************************************************************
class DSStreamSourceStreamVideo : public CSourceStream
{
public:
    DSStreamSourceStreamVideo  (HRESULT *phr, DSFilterSourceStreamVideo *pParent, LPCWSTR pPinName,INWStreamReader* _stream);
    ~DSStreamSourceStreamVideo ();

    // CBaseOutputPin
    HRESULT DecideBufferSize(IMemAllocator* _memAlloc, ALLOCATOR_PROPERTIES* _properties);

    // CSourceStream
    virtual HRESULT FillBuffer(IMediaSample* _samp);
    virtual HRESULT GetMediaType(int _position, CMediaType* _pmt);
    virtual HRESULT CheckMediaType(const CMediaType *pMediaType);
    virtual HRESULT OnThreadCreate();

    // CBasePin
    virtual HRESULT __stdcall Notify(IBaseFilter * pSender, Quality q);

private:
    void getSize();
    void getSizeFromStream();
    void getSizeProcedural();

    bool fillNextFrame(unsigned char* _buffer, int _buffersize, u64& time_);
    bool fillNextFrameFromStream(unsigned char* _buffer, int _buffersize, u64& time_);
    bool fillNextFrameProcedural(unsigned char* _buffer, int _buffersize, u64& time_);

    INWStreamReader* mStream;
    int mWidth;
    int mHeight;

    u64 mTime;
    u64 mLastTime;
};

#endif
