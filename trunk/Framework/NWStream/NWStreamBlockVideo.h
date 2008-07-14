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
