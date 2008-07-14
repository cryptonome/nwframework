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
#ifndef NWSTREAMBLOCKAUDIO_H_
#define NWSTREAMBLOCKAUDIO_H_

#include "NWStreamBlockMedia.h"

//********************************************************************
//
//********************************************************************
class NWStreamBlockAudio : public NWStreamBlockMedia
{
public:
    NWStreamBlockAudio  ();
    virtual    ~NWStreamBlockAudio ()                      { NWStreamBlockAudio::done(); }

    virtual bool          init                 ();
    virtual void          done                 ();

    void setAudioBuffer(int _bitsPerSample, int _channels, int _samplesPerSec, int _samples, unsigned char* _buffer, bool _copy = true);

    int getBitsPerSample() const { return mBitsPerSample; }
    int getChannels() const { return mChannels; }
    int getSamplesPerSec() const { return mSamplesPerSec; }
    int getSamples() const { return mSamples; }
    const unsigned char* getBuffer() const { return mBuffer; }

private:
    typedef NWStreamBlockMedia Inherited;

    int mBitsPerSample;
    int mChannels;
    int mSamplesPerSec;
    int mSamples;
    unsigned char* mBuffer;
};


#endif
