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

#include "DSGraphRender.h"
#include "NWStreamGroup.h"
#include "DSUtils.h"
#include "INWStream.h"
#include "DSFilterSourceStreamDSMediaSample.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSGraphRender::DSGraphRender() : Inherited(),
    mStreamGroupInput(0),
    mBuilt(false)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphRender::init()
{
    bool bOK = true;

    if (!isOk())
    {
        bOK = Inherited::init();

        mBuilt = false;
        mStreamGroupInput = NEW NWStreamGroupRead();
        bOK = mStreamGroupInput->init("Graph Render Input");
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraphRender::done()
{
    if (isOk())
    {
        mStreamGroupInput->disableRead(true);
        Inherited::done();
        DISPOSE(mStreamGroupInput);
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
/*bool DSGraphRender::connectInputGraph(INWGraph* _graph)
{
    bool bOK = _graph->getStreamGroupOutput() != NULL;
    ASSERT(mGraphInput == 0);
    ASSERT(bOK);
    mGraphInput = _graph;

    if ( bOK )
    {
        bOK = createRenderGraph();
    }

    if ( bOK )
        bOK = mGraphInput->inputGraphConnected();

    if ( bOK )
    {
        mGraphInput->start();
    }

    return bOK;
}*/

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphRender::build()
{
    bool ok = true;

    ASSERT(!mBuilt);

    if ( !mBuilt )
    {
        ok = createRenderGraph();
        mBuilt = true;
    }

    return ok;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphRender::postBuild()
{
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphRender::start()
{
    bool ok = true;

    ASSERT(mBuilt);

    if ( !mStarted )
    {
        mStreamGroupInput->disableRead(false);

        ok = Inherited::start();
    }

    return ok;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraphRender::stop()
{
    if ( mStarted )
    {
        mStreamGroupInput->disableRead(true);

        Inherited::stop();
    }
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraphRender::pause()
{
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphRender::createRenderGraph()
{
    bool bOK = true;

    // Create our streams
    /*INWStreamGroup* outputStreamGroup = mGraphInput->getStreamGroupOutput();
    for ( int i = 0 ; i < outputStreamGroup->getNumStreams() ; ++i )
    {
        INWStream* stream = outputStreamGroup->getStream(i);
        mStreamGroupInput->addStream(stream);
    }*/
    for( std::list<INWStreamReader*>::iterator it = mStreamsToBuild.begin() ; it != mStreamsToBuild.end() ; ++it )
    {
        INWStreamReader* stream = *it;
        mStreamGroupInput->addStream(stream);
    }

    // Create the source filters
    std::list<IBaseFilter*> filters;
    for ( int i = 0 ; i < mStreamGroupInput->getNumStreams() ; ++i )
    {
        INWStreamReader* stream = mStreamGroupInput->getStream(i);

        if ( stream->getType() == NWSTREAM_TYPE_MEDIA )
        {
            ASSERT(stream->getSubType() == NWSTREAM_SUBTYPE_MEDIA_DS_MEDIASAMPLE);

            DSFilterSourceStreamDSMediaSample* filter = DSFilterSourceStreamDSMediaSample::createInstance(stream);
            HRESULT hr = mGraphBuilder->AddFilter(filter,L"");

            filter->AddRef();
            filters.push_back(filter);
            ASSERT(hr == S_OK); hr;
        }
        else
        {
            ASSERT(false);
        }
    }

    if ( bOK )
        bOK = createFilters();

    // Render Source filters
    if ( bOK )
    {
        bool videoConnected = false;
        bool audioConnected = false;
        for ( std::list<IBaseFilter*>::iterator it = filters.begin() ; bOK && it != filters.end() ; ++it )
        {
            bool connected = false;

            IBaseFilter* filter = *it;
            IPin* pin = DSUtils::getOutputPin(filter);

            DSFilterSourceStreamDSMediaSample* filterMediaSample = (DSFilterSourceStreamDSMediaSample*)filter;
            bOK = connectSourceFilter(filter, pin, filterMediaSample->getStream());

            DS_RELEASE(pin);
            DS_RELEASE(filter);

        }
    }

    if ( bOK )
        renderGraphCreated();

    if ( bOK )
        DSUtils::enableExternalDebugGraph(mGraphBuilder);

    return bOK;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraphRender::addStream(INWStreamReader* _stream)
{
    mStreamsToBuild.push_back(_stream);
}
