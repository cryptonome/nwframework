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
#ifndef NWSTREAMBLOCKDSMEDIASAMPLE_H_
#define NWSTREAMBLOCKDSMEDIASAMPLE_H_

#include "NWStreamBlockMedia.h"

struct IMediaSample;

//********************************************************************
//
//********************************************************************
class NWStreamBlockDSMediaSample : public NWStreamBlockMedia
{
public:
    NWStreamBlockDSMediaSample  ();
    virtual    ~NWStreamBlockDSMediaSample ()                      { NWStreamBlockDSMediaSample::done(); }

    virtual bool          init                         ();
    virtual void          done                         ();

    void setMediaSample(IMediaSample* _mediaSample);
    IMediaSample* getMediaSample() const;

private:
    typedef NWStreamBlockMedia Inherited;

    IMediaSample* mMediaSample;
};

#endif
