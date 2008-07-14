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
