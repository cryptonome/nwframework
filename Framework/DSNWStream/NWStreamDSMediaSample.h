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
#ifndef NWSTREAMDSMEDIASAMPLE_H_
#define NWSTREAMDSMEDIASAMPLE_H_

#include "NWStreamMedia.h"
#include "streams.h"

struct IPin;

//***********************************************************************************
//
//   ----------------     ------------              ------------     ---------------
//   | BeforeRender |o   o| DSRender | <- Stream -> | DSSource |o   o| AfterSource |
//   ----------------     ------------              ------------     ---------------
//
// Stream Writer = DSRender
// Stream Reader = DSSource
//***********************************************************************************


//********************************************************************
//
//********************************************************************
class NWStreamDSMediaSample : public NWStreamMedia
{
public:
    NWStreamDSMediaSample  ();
    virtual    ~NWStreamDSMediaSample ()                      { NWStreamDSMediaSample::done(); }

    virtual bool          init                    (ENWStreamMediaType _mediaType);
    virtual void          done                    ();

    virtual void setBeforeRenderPinOut(IPin* _pin) { mBeforeRenderPinOut = _pin; }
    virtual IPin* getBeforeRenderPinOut() const { return mBeforeRenderPinOut; }

    virtual void setAcceptedMediaType(const AM_MEDIA_TYPE* _mediaType);
    virtual const AM_MEDIA_TYPE* getAcceptedMediaType() const;
    
private:
    typedef NWStreamMedia Inherited;

    IPin* mBeforeRenderPinOut;
    AM_MEDIA_TYPE* mMediaType;
};

#endif
