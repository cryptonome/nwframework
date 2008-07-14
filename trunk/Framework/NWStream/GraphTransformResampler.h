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
