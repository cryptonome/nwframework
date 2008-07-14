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
#ifndef GRAPHSOURCERANDOM_H_
#define GRAPHSOURCERANDOM_H_

#include "GraphSource.h"
#include "MultiTask.h"

class NWStreamVideo;
class NWStreamAudio;

//********************************************************************
//
//********************************************************************
class GraphSourceRandom : public GraphSource, public MultiTask::ITaskProcessor
{
public:
    GraphSourceRandom  ();
    virtual    ~GraphSourceRandom ()                      { GraphSourceRandom::done(); }

    struct InitData
    {
        struct Video
        {
            Video() : mWidth(320), mHeight(240), mFrameRate(30), mBitsPerPixel(24) { }

            int mWidth;
            int mHeight;
            int mFrameRate;
            int mBitsPerPixel;
        };

        struct Audio
        {
            Audio() : mSampleRate(44100), mBitsPerSample(16), mChannels(2), mSamplesPerBlock(10000) { }

            int mSampleRate;
            int mBitsPerSample;
            int mChannels;
            int mSamplesPerBlock;
        };

        Video video;
        Audio audio;
    };

    virtual bool          init                (const InitData& _data);
    virtual void          done                ();

private:
    typedef GraphSource Inherited;

    // INWGraph
    virtual bool start();
    virtual void stop();

    // MultiTask::ITaskProcessor
    virtual bool multiTaskProcess(MultiTask* _multiTask, int _task);


    void generateNewFrameVideo();
    void generateNewFrameAudio();

    bool createStreams();
    void destroyStreams();

    void sendStreamProperties();

    NWStreamVideo* mStreamVideo;
    NWStreamAudio* mStreamAudio;

    InitData mData;

    u64 mTimeAudio;
    u64 mTimeVideo;

    MultiTask* mMultiTask;
};

#endif
