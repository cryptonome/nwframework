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

