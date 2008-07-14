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
