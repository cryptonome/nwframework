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
#ifndef DSFILTERRENDERSTREAMVIDEO_H_
#define DSFILTERRENDERSTREAMVIDEO_H_

#include "streams.h"

class INWStreamWriter;

//********************************************************************
//
//********************************************************************
class DSFilterRenderStreamVideo : public CBaseRenderer
{
public:

    static DSFilterRenderStreamVideo* createInstance(INWStreamWriter* _stream);

    DECLARE_IUNKNOWN

private:
    DSFilterRenderStreamVideo  (HRESULT* phr, INWStreamWriter* _stream);
    virtual    ~DSFilterRenderStreamVideo ();

    // CBaseRenderer
    HRESULT CheckMediaType(const CMediaType *pmtIn); // make sure media type is what we want
    HRESULT DoRenderSample(IMediaSample *pMediaSample); // have to override this    
    HRESULT SetMediaType(const CMediaType *pmt); // have to override this
    HRESULT Active(); // override these to receive indication of when we change to Pause/Play (Active) or Stop (Inactive) state.
    HRESULT Inactive(); // override these to receive indication of when we change to Pause/Play (Active) or Stop (Inactive) state.
    HRESULT Notify(IBaseFilter * pSender, Quality q);
    HRESULT EndOfStream();

    INWStreamWriter* mStream;
    int mWidth;
    int mHeight;
};

#endif
