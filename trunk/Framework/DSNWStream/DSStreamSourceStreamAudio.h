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
#ifndef DSSTREAMSOURCESTREAMAUDIO_H_
#define DSSTREAMSOURCESTREAMAUDIO_H_

#include "streams.h"

class DSFilterSourceStreamAudio;
class INWStreamReader;
class INWStreamBlock;

//********************************************************************
//
//********************************************************************
class DSStreamSourceStreamAudio : public CSourceStream
{
public:
    DSStreamSourceStreamAudio  (HRESULT *phr, DSFilterSourceStreamAudio *pParent, LPCWSTR pPinName,INWStreamReader* _stream);
    ~DSStreamSourceStreamAudio ();

    // CBaseOutputPin
    HRESULT DecideBufferSize(IMemAllocator* _memAlloc, ALLOCATOR_PROPERTIES* _properties);

    // CSourceStream
    virtual HRESULT FillBuffer(IMediaSample* _samp);
    virtual HRESULT GetMediaType(int _position, CMediaType* _pmt);
    virtual HRESULT CheckMediaType(const CMediaType *pMediaType);
    //virtual HRESULT CompleteConnect(IPin *pReceivePin);
    virtual HRESULT OnThreadCreate();

    // CBasePin
    virtual HRESULT __stdcall Notify(IBaseFilter * pSender, Quality q);

    // CBaseOutputPin
    HRESULT GetDeliveryBuffer(IMediaSample ** ppSample,REFERENCE_TIME * pStartTime, REFERENCE_TIME * pEndTime, DWORD dwFlags);

private:
    void fillNextFrame(unsigned char* _buffer, int _buffersize, u64& time_);
    void fillNextFrameProcedural(unsigned char* _buffer, int _buffersize, u64& time_);
    void fillNextFrameFromStream(unsigned char* _buffer, int _buffersize, u64& time_);
    int processNewSamplesFromStream(unsigned char* _buffer, int _samples, u64& time_);

    INWStreamReader* mStream;

    int mBitsPerSample;
    int mChannels;
    int mSamplesPerSec;

    u64 mTime;
    u64 mLastTime;

    INWStreamBlock* mStreamBlock;
    const unsigned char* mSBBuffer;
    int mSBAvailableSamples;

    bool mEOS;
};

#endif
