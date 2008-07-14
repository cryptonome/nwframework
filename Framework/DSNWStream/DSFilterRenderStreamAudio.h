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
#ifndef DSFILTERRENDERSTREAMAUDIO_H_
#define DSFILTERRENDERSTREAMAUDIO_H_


#include "streams.h"

class INWStreamWriter;

//********************************************************************
//
//********************************************************************
class DSFilterRenderStreamAudio : public CBaseRenderer
{
public:

    static DSFilterRenderStreamAudio* createInstance(INWStreamWriter* _stream);

    DECLARE_IUNKNOWN

private:
    DSFilterRenderStreamAudio  (HRESULT* phr, INWStreamWriter* _stream);
    virtual    ~DSFilterRenderStreamAudio ();

    // CBaseRenderer
    HRESULT CheckMediaType(const CMediaType *pmtIn); // make sure media type is what we want
    HRESULT DoRenderSample(IMediaSample *pMediaSample); // have to override this    
    HRESULT SetMediaType(const CMediaType *pmt); // have to override this
    HRESULT Active(); // override these to receive indication of when we change to Pause/Play (Active) or Stop (Inactive) state.
    HRESULT Inactive(); // override these to receive indication of when we change to Pause/Play (Active) or Stop (Inactive) state.
    HRESULT EndOfStream();

    INWStreamWriter* mStream;

    int mBitsPerSample;
    int mChannels;
    int mSamplesPerSec;
    int mBytesPerSample;
};


#endif
