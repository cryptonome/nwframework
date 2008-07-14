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

#include "DSGraph.h"
#include "NWStreamGroup.h"
#include "DSUtils.h"
#include "streams.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSGraph::DSGraph() :
    mInit(false),
    mGraphBuilder(0),
    mMediaControl(0),
    mMediaEvent(0),
    mMediaPosition(0),
    mStarted(false)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraph::init()
{
    bool bOK = true;

    if (!isOk())
    {
        mGraphBuilder = 0;
        mMediaControl = 0;
        mMediaEvent = 0;
        mMediaPosition = 0;
        mStarted = false;

        if ( bOK )
        {
            bOK = createBaseGraphComponents();
            ASSERT(bOK);
        }

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraph::done()
{
    if (isOk())
    {
        stop();
        destroyBaseGraphComponents();

        mInit = false;
    }
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
/*bool DSGraph::connectInputGraph(INWGraph* _graph)
{
    // Base DSGraph can't connect to other graphs, only
    // transform and render DSGraphs are able to do it
    ASSERT(false);
    return false;
}*/

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraph::createBaseGraphComponents()
{
    bool bOK = true;

    // Create the filter graph
    if ( bOK )
    {
        ASSERT(mGraphBuilder == 0);
        mGraphBuilder = DSUtils::createGraphBuilder();
        bOK = mGraphBuilder != 0;
    }

    // Media Control
    if ( bOK )
    {
        mMediaControl = DSUtils::createMediaControl(mGraphBuilder);
        bOK = mMediaControl != 0;
    }

    // Media Event
    if ( bOK )
    {
        mMediaEvent = DSUtils::createMediaEvent(mGraphBuilder);
        bOK = mMediaEvent != 0;
    }

    // Media Position
    if ( bOK )
    {
        mMediaPosition = DSUtils::createMediaPosition(mGraphBuilder);
        bOK = mMediaPosition != 0;
    }


    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraph::destroyBaseGraphComponents()
{
    DS_RELEASE(mMediaPosition);
    DS_RELEASE(mMediaEvent);
    DS_RELEASE(mMediaControl);
    DS_RELEASE(mGraphBuilder);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraph::start()
{
    bool bOK = true;

    if ( !mStarted )
    {
        HRESULT hr = mMediaControl->Run();
        mStarted = true;
    }

    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraph::stop()
{
    if ( mStarted )
    {
        mMediaControl->Stop();
        mStarted = false;
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraph::waitEnd()
{
    if ( mStarted )
    {
        long event = 0;
        mMediaEvent->WaitForCompletion(INFINITE, &event);
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
double DSGraph::getDuration() const
{
    double duration = 0;

    if ( mMediaPosition )
    {
        REFTIME refTime = 0;
        if ( mMediaPosition->get_Duration(&refTime) == S_OK )
            duration = refTime;
        else
        {
            ASSERT(false);
        }
    }

    return duration;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
double DSGraph::getCurrentPosition() const
{
    double position = 0;

    if ( mMediaPosition )
    {
        REFTIME refTime = 0;
        if ( mMediaPosition->get_CurrentPosition(&refTime) == S_OK )
            position = refTime;
        else
        {
            ASSERT(false);
        }
    }

    return position;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraph::setCurrentPosition(double _pos)
{
    REFTIME refTime = _pos;
    if ( mMediaPosition->put_CurrentPosition(refTime) != S_OK )
    {
        ASSERT(false);
    }
}


