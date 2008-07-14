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
#ifndef GRAPHTRANSFORMRESAMPLER_H_
#define GRAPHTRANSFORMRESAMPLER_H_

#include "GraphTransform.h"

//********************************************************************
//
//********************************************************************
class GraphTransformResampler : public GraphTransform
{
public:
    GraphTransformResampler  ();
    virtual    ~GraphTransformResampler ()                      { GraphTransformResampler::done(); }

    struct InitData
    {
        struct Video
        {
            Video() : mResample(true), mWidth(320), mHeight(240) { }

            bool mResample;
            int mWidth;
            int mHeight;
        };

        struct Audio
        {
            Audio() : mResample(true), mSampleRate(44100), mBitsPerSample(16), mChannels(2) { }

            bool mResample;
            int mSampleRate;
            int mBitsPerSample;
            int mChannels;
        };

        Video video;
        Audio audio;
    };

    virtual bool          init                      (const InitData& _data);
    virtual void          done                      ();


private:
    typedef GraphTransform Inherited;

    // INWGraph
    virtual bool start();
    virtual void stop();


    InitData mData;
};

#endif
