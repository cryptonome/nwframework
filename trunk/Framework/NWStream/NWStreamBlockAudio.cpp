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

