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

#include "NWStreamDSMediaSample.h"


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamDSMediaSample::NWStreamDSMediaSample() : Inherited(),
    mBeforeRenderPinOut(0),
    mMediaType(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamDSMediaSample::init(ENWStreamMediaType _mediaType)
{
    bool bOK = true;

    if (!isOk())
    {
        mBeforeRenderPinOut = 0;
        mMediaType = 0;

        bOK = Inherited::init(NWSTREAM_SUBTYPE_MEDIA_DS_MEDIASAMPLE,_mediaType);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamDSMediaSample::done()
{
    if (isOk())
    {
        if ( mMediaType )
        {
            FreeMediaType(*mMediaType);
            mMediaType = 0;
        }
        Inherited::done();
    }
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamDSMediaSample::setAcceptedMediaType(const AM_MEDIA_TYPE* _mediaType)
{
    if ( mMediaType )
        FreeMediaType(*mMediaType);

    mMediaType = CreateMediaType(_mediaType);

    if ( mMediaType )
    {
        if ( mMediaType->majortype == MEDIATYPE_Video )
            ASSERT(getMediaType() == NWSTREAM_MEDIATYPE_VIDEO);
        else if ( mMediaType->majortype == MEDIATYPE_Audio )
            ASSERT(getMediaType() == NWSTREAM_MEDIATYPE_AUDIO);
        else 
            ASSERT(getMediaType() == NWSTREAM_MEDIATYPE_UNKNOWN);
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
const AM_MEDIA_TYPE* NWStreamDSMediaSample::getAcceptedMediaType() const
{ 
    return mMediaType;
}
