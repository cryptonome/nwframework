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
