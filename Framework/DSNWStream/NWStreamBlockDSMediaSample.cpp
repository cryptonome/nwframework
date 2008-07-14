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
#include "PchDSNWStream.h"

#include "NWStreamBlockDSMediaSample.h"
#include "DSUtils.h"
#include "streams.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamBlockDSMediaSample::NWStreamBlockDSMediaSample() : Inherited(),
    mMediaSample(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamBlockDSMediaSample::init()
{
    bool bOK = true;

    if (!isOk())
    {
        mMediaSample = 0;

        bOK = Inherited::init(NWSTREAM_SUBTYPE_MEDIA_DS_MEDIASAMPLE);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockDSMediaSample::done()
{
    if (isOk())
    {
        DS_RELEASE(mMediaSample);

        Inherited::done();
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockDSMediaSample::setMediaSample(IMediaSample* _mediaSample)
{
    DS_RELEASE(mMediaSample);
    mMediaSample = _mediaSample;
    mMediaSample->AddRef();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IMediaSample* NWStreamBlockDSMediaSample::getMediaSample() const
{
    return mMediaSample;
}

