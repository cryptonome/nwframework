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

#include "NWStreamBlockAudio.h"
#include <memory.h>

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamBlockAudio::NWStreamBlockAudio() : Inherited(),
    mBitsPerSample(0),
    mChannels(0),
    mSamplesPerSec(0),
    mBuffer(0),
    mSamples(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamBlockAudio::init()
{
    bool bOK = true;

    if (!isOk())
    {
        mBitsPerSample = 0;
        mChannels = 0;
        mSamplesPerSec = 0;
        mBuffer = 0;
        mSamples = 0;
        bOK = Inherited::init(NWSTREAM_SUBTYPE_MEDIA_AUDIO);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockAudio::done()
{
    if (isOk())
    {
        DISPOSE_ARRAY(mBuffer);
        Inherited::done();
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockAudio::setAudioBuffer(int _bitsPerSample, int _channels, int _samplesPerSec, int _samples, unsigned char* _buffer, bool _copy)
{
    mBitsPerSample = _bitsPerSample;
    mChannels = _channels;
    mSamplesPerSec = _samplesPerSec;
    mSamples = _samples;

    if ( _buffer && _copy )
    {
        int sizeBuffer = (mBitsPerSample/8) * mChannels * mSamples;
        mBuffer = NEW unsigned char[sizeBuffer];
        memcpy(mBuffer,_buffer,sizeBuffer);
    }
    else
    {
        mBuffer = _buffer;
    }
}

