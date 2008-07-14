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

#include "GraphSourceRandom.h"
#include "NWThread.h"
#include "NWEvent.h"
#include "NWStreamVideo.h"
#include "NWStreamAudio.h"
#include "NWStreamBlockVideo.h"
#include "NWStreamBlockAudio.h"
#include "NWStreamGroup.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GraphSourceRandom::GraphSourceRandom() : Inherited(),
    mStreamVideo(0),
    mStreamAudio(0),
    mTimeAudio(0),
    mTimeVideo(0),
    mMultiTask(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GraphSourceRandom::init(const InitData& _data)
{
    bool bOK = true;

    if (!isOk())
    {
        bOK = Inherited::init();

        mStreamAudio = 0;
        mStreamVideo = 0;
        mTimeAudio = 0;
        mTimeVideo = 0;
        mData = _data;
        mMultiTask = 0;

        if ( bOK )
            bOK = createStreams();

        if ( bOK )
        {
            mMultiTask = NEW MultiTask();
            bOK = mMultiTask->init(2,this);
        }
    }
    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GraphSourceRandom::done()
{
    if (isOk())
    {
        mStreamVideo->disableWrite(true);
        mStreamAudio->disableWrite(true);

        Inherited::done();

        DISPOSE(mMultiTask);
    }
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GraphSourceRandom::start()
{
    bool bOK = true;

    bOK = mMultiTask->start();

    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GraphSourceRandom::stop()
{
    mMultiTask->stop();
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GraphSourceRandom::generateNewFrameVideo()
{
    // Fill the framebuffer
    InitData::Video& videoData = mData.video;
    int bufferSize = videoData.mWidth * videoData.mHeight * (videoData.mBitsPerPixel/8);
    unsigned char* frameBuffer = NEW unsigned char[bufferSize];
    for ( int i = 0 ; i < bufferSize ; ++i )
        frameBuffer[i] = rand();

    // Add the block to the stream
    NWStreamBlockVideo* videoBlock = NEW NWStreamBlockVideo();
    videoBlock->init();
    videoBlock->setFrameBufferData(videoData.mWidth, videoData.mHeight, videoData.mWidth*3, frameBuffer, false);
    videoBlock->setTime(mTimeVideo);
    mTimeVideo += ((u64)1000 * (u64)10000) / (u64)videoData.mFrameRate;
    mStreamVideo->writeBlock(videoBlock);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GraphSourceRandom::generateNewFrameAudio()
{
    InitData::Audio& audioData = mData.audio;
    int bytesPerSample = audioData.mChannels * (audioData.mBitsPerSample/8);
    int bufferSize = bytesPerSample * audioData.mSamplesPerBlock;
    unsigned char* buffer = NEW unsigned char[bufferSize];
    for ( int i = 0 ; i < bufferSize ; ++i )
        buffer[i] = rand();

    // Add the block to the stream
    NWStreamBlockAudio* audioBlock = NEW NWStreamBlockAudio();
    audioBlock->init();
    audioBlock->setAudioBuffer(audioData.mBitsPerSample, audioData.mChannels, audioData.mSampleRate, audioData.mSamplesPerBlock, buffer, false);
    audioBlock->setTime(mTimeAudio);
    mTimeAudio += ((u64)audioData.mSamplesPerBlock * (u64)1000 * (u64)10000) / (u64)audioData.mSampleRate;
    mStreamAudio->writeBlock(audioBlock);
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GraphSourceRandom::sendStreamProperties()
{
    // Video
    InitData::Video& videoData = mData.video;
    NWStreamBlockVideo* videoBlock = NEW NWStreamBlockVideo();
    videoBlock->init();
    videoBlock->setFrameBufferData(videoData.mWidth, videoData.mHeight, -1, 0);
    mStreamVideo->writeBlock(videoBlock,false);

    // Audio
    InitData::Audio& audioData = mData.audio;
    NWStreamBlockAudio* audioBlock = NEW NWStreamBlockAudio();
    audioBlock->init();
    audioBlock->setAudioBuffer(audioData.mBitsPerSample, audioData.mChannels, audioData.mSampleRate, audioData.mSamplesPerBlock, 0);
    mStreamAudio->writeBlock(audioBlock,false);
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GraphSourceRandom::createStreams()
{
    bool bOK = true;

    if ( bOK )
    {
        mStreamVideo = NEW NWStreamVideo();
        bOK = mStreamVideo->init();
        if ( bOK )
            mStreamGroupOutput->addStream(mStreamVideo);
        else
            DISPOSE(mStreamVideo);
    }

    if ( bOK )
    {
        mStreamAudio = NEW NWStreamAudio();
        bOK = mStreamAudio->init();
        if ( bOK )
            mStreamGroupOutput->addStream(mStreamAudio);
        else
            DISPOSE(mStreamAudio);
    }

    if ( bOK )
    {
        sendStreamProperties();
    }

    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GraphSourceRandom::destroyStreams()
{
    DISPOSE(mStreamVideo);
    DISPOSE(mStreamAudio);
}



//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GraphSourceRandom::multiTaskProcess(MultiTask* _multiTask, int _task)
{
    bool exit = false;

    switch (_task)
    {
        case 0:
            generateNewFrameVideo();
            break;
        case 1:
            generateNewFrameAudio();
            break;
        default:
            ASSERT(false);
    }

    return exit;
}

