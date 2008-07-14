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
#include "PchDSNWStream.h"

#include "DSGraphSourceFile.h"
#include "DSUtils.h"
#include "DSFilterRenderStreamVideo.h"
#include "DSFilterRenderStreamAudio.h"
#include "DSFilterRenderDSMediaSample.h"
#include "NWStreamGroup.h"
#include "NWStreamVideo.h"
#include "NWStreamAudio.h"
#include "NWStreamDSMediaSample.h"
#include <dshow.h>

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSGraphSourceFile::DSGraphSourceFile() : Inherited(),
    mFilterRenderVideo(0),
    mFilterRenderAudio(0),
    mFiltersRenderConnected(false)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphSourceFile::init(const char* _fileName)
{
    bool bOK = true;

    if (!isOk())
    {
        bOK = Inherited::init();

        mFilterRenderVideo = 0;
        mFilterRenderAudio = 0;
        mFiltersRenderConnected = false;
        mFileName = _fileName;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraphSourceFile::done()
{
    if (isOk())
    {
        destroySourceFileGraph();

        Inherited::done();
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraphSourceFile::setFileName(const char* _fileName)
{ 
    mFileName = _fileName;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphSourceFile::createSourceFileGraph()
{
    bool bOK = true;

    destroySourceFileGraph();

    // Render the file
    bOK = DSUtils::renderFile(mGraphBuilder,mFileName.c_str(),DSUtils::E_RENDER_FILE_MODE_FULL);

    // Change the render filters by stream render filters
    if ( bOK )
        bOK = DSUtils::swapFiltersRender(mGraphBuilder,this,false);

    // Set no-clock
    if ( bOK )
        DSUtils::setGraphNoClock(mGraphBuilder);

    if ( bOK )
        DSUtils::enableExternalDebugGraph(mGraphBuilder);

    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraphSourceFile::destroySourceFileGraph()
{
    for ( std::list<FilterRender>::iterator it = mFiltersRender.begin() ; it != mFiltersRender.end() ; ++it )
    {
        FilterRender& fr = *it;
        DS_RELEASE(fr.filter);
        DS_RELEASE(fr.pinToConnect);
    }
    mFiltersRender.clear();
    mFiltersRenderConnected = false;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IBaseFilter* DSGraphSourceFile::swap(IBaseFilter* _filter, IPin* _pin, IPin*& pinNew_)
{
    IBaseFilter* filter = 0;

    AM_MEDIA_TYPE mediaType;
    memset(&mediaType,0,sizeof(mediaType));
    if ( _pin->ConnectionMediaType(&mediaType) == S_OK )
    {
        // Video
        /*if ( DSUtils::areEqual(mediaType.majortype,MEDIATYPE_Video) )
        {
            // Create the stream
            NWStreamVideo* stream = NEW NWStreamVideo();
            bool bOK = stream->init();

            if ( bOK )
            {
                // Create the filter
                ASSERT(mFilterRenderVideo == 0);
                mFilterRenderVideo = DSFilterRenderStreamVideo::createInstance(stream);
                filter = mFilterRenderVideo;
                pinNew_= DSUtils::getInputPin(mFilterRenderVideo);

                // Add the stream to the streamgroup
                mStreamGroupOutput->addStream(stream);
            }
            else
            {
                DISPOSE(stream);
            }
        }
        // Audio
        else if ( DSUtils::areEqual(mediaType.majortype,MEDIATYPE_Audio) )
        {
            // Create the stream
            NWStreamAudio* stream = NEW NWStreamAudio();
            bool bOK = stream->init();

            if ( bOK )
            {
                // Create the filter
                ASSERT(mFilterRenderAudio == 0);
                mFilterRenderAudio = DSFilterRenderStreamAudio::createInstance(stream);
                filter = mFilterRenderAudio;
                pinNew_= DSUtils::getInputPin(mFilterRenderAudio);

                // Add the stream to the streamgroup
                mStreamGroupOutput->addStream(stream);
            }
            else
            {
                DISPOSE(stream);
            }
        }
        // Unknown
        else
        {
            ASSERT(false);
        }*/

            ENWStreamMediaType mediaTypeNW = NWSTREAM_MEDIATYPE_UNKNOWN;

            if ( DSUtils::areEqual(mediaType.majortype,MEDIATYPE_Video) )
                mediaTypeNW = NWSTREAM_MEDIATYPE_VIDEO;
            else if ( DSUtils::areEqual(mediaType.majortype,MEDIATYPE_Audio) )
                mediaTypeNW = NWSTREAM_MEDIATYPE_AUDIO;

            // Create the DS MediaSample stream
            NWStreamDSMediaSample* stream = NEW NWStreamDSMediaSample();
            bool bOK = stream->init(mediaTypeNW);

            if ( bOK )
            {
                // Create the filter
                ASSERT(mFilterRenderAudio == 0);
                DSFilterRenderStreamDSMediaSample* filterRender = DSFilterRenderStreamDSMediaSample::createInstance(stream);
                filter = filterRender;
                pinNew_= DSUtils::getInputPin(filterRender);

                FilterRender fr;
                fr.filter = filterRender;
                fr.filter->AddRef();
                IPin* pinConnected = DSUtils::getConnectedTo(_pin);
                ASSERT(pinConnected);
                fr.pinToConnect = pinConnected;
                fr.pinToConnect->AddRef();
                stream->setBeforeRenderPinOut(fr.pinToConnect);
                mFiltersRender.push_back(fr);
                DS_RELEASE(pinConnected);

                // Add the stream to the streamgroup
                mStreamGroupOutput->addStream(stream);
            }
            else
            {
                DISPOSE(stream);
            }
    }

    return filter;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphSourceFile::build()
{
    bool ok = true;

    ok = createSourceFileGraph();

    return ok;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphSourceFile::connectFiltersRender()
{
    bool bOK = true;

    if ( !mFiltersRenderConnected )
    {
        for ( std::list<FilterRender>::iterator it = mFiltersRender.begin() ; it != mFiltersRender.end() ; ++it )
        {
            FilterRender& fr = *it;
            IPin* pinFilter = DSUtils::getInputPin(fr.filter);
            bOK &= mGraphBuilder->Connect(fr.pinToConnect,pinFilter) == S_OK;

            ASSERT(bOK);
            DS_RELEASE(pinFilter);
        }

        mFiltersRenderConnected = true;
    }

    return bOK;
}

