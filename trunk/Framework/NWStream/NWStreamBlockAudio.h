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
