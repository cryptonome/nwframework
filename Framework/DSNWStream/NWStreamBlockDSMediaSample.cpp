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

