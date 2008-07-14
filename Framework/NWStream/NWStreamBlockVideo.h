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
#ifndef NWSTREAMBLOCKVIDEO_H_
#define NWSTREAMBLOCKVIDEO_H_

#include "NWStreamBlockMedia.h"

//********************************************************************
//
//********************************************************************
class NWStreamBlockVideo : public NWStreamBlockMedia
{
public:
    NWStreamBlockVideo  ();
    virtual    ~NWStreamBlockVideo ()                      { NWStreamBlockVideo::done(); }

    virtual bool          init                 ();
    virtual void          done                 ();

    void setFrameBufferData(int _width, int _height, int _stride, unsigned char* _frameBuffer, bool _copy = true);
    const unsigned char* getFrameBuffer() const { return mFrameBuffer; }

    int getWidth() const { return mWidth; }
    int getHeight() const { return mHeight; }
    int getStride() const { return mStride; }

private:
    typedef NWStreamBlockMedia Inherited;

    unsigned char* mFrameBuffer;
    int mWidth;
    int mHeight;
    int mStride;
};

#endif
