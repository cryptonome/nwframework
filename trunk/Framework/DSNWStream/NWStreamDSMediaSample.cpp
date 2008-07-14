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
