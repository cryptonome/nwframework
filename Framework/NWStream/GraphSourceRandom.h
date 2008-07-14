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
