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
