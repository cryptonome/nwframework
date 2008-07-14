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
#include "PchNWStream.h"

#include "NWStreamBlockVideo.h"
#include <memory.h>

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamBlockVideo::NWStreamBlockVideo() : Inherited(),
    mFrameBuffer(0),
    mWidth(0),
    mHeight(0),
    mStride(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamBlockVideo::init()
{
    bool bOK = true;

    if (!isOk())
    {
        mFrameBuffer = 0;
        mWidth = 0;
        mHeight = 0;
        mStride = 0;
        bOK = Inherited::init(NWSTREAM_SUBTYPE_MEDIA_VIDEO);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockVideo::done()
{
    if (isOk())
    {
        DISPOSE_ARRAY(mFrameBuffer);
        Inherited::done();
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockVideo::setFrameBufferData(int _width, int _height, int _stride, unsigned char* _frameBuffer, bool _copy)
{
    if ( _frameBuffer && _copy )
    {
        int frameBufferSize = _height*_stride;
        ASSERT(_stride >= (_height*3));
        mFrameBuffer = NEW unsigned char[frameBufferSize];
        memcpy(mFrameBuffer,_frameBuffer,frameBufferSize);
    }
    else
    {
        mFrameBuffer = _frameBuffer;
    }
    mWidth = _width;
    mHeight = _height;
    mStride = _stride;
}

