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

#include "DSGraphTransform.h"
#include "NWStreamGroup.h"

#include <windows.h>

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSGraphTransform::DSGraphTransform() : Inherited(),
    mGraphInput(0),
    mStreamGroupInput(0),
    mStreamGroupOutput(0),
    mBuilt(false)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphTransform::init()
{
    bool bOK = true;

    if (!isOk())
    {
        bOK = Inherited::init();

        mGraphInput = 0;
        mStreamGroupInput = 0;
        mStreamGroupOutput = 0;

        mStreamGroupInput = NEW NWStreamGroupRead();
        bOK = mStreamGroupInput->init("Graph Transform Input");

        if ( bOK )
        {
            mStreamGroupOutput = NEW NWStreamGroupWrite();
            bOK = mStreamGroupOutput->init("Graph Transform Output");
        }
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraphTransform::done()
{
    if (isOk())
    {
        mStreamGroupOutput->disableWrite(true);
        mStreamGroupInput->disableRead(true);
        Inherited::done();
        DISPOSE(mStreamGroupInput);
        DISPOSE(mStreamGroupOutput);
    }
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
/*bool DSGraphTransform::connectInputGraph(INWGraph* _graph)
{
    bool bOK = _graph->getStreamGroupOutput() != NULL;
    ASSERT(mGraphInput == 0);
    ASSERT(bOK);
    mGraphInput = _graph;

    if ( bOK )
    {
        bOK = createTransformGraph();
    }

    if ( bOK )
        bOK = mGraphInput->inputGraphConnected();

    if ( bOK )
    {
        mGraphInput->start();
    }

    return bOK;
}*/


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
/*bool DSGraphTransform::inputGraphConnected()
{
    return connectFiltersRender();
}*/


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
INWStreamGroupWrite* DSGraphTransform::getStreamGroupOutput()
{
    return mStreamGroupOutput;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphTransform::build()
{
    bool ok = true;

    ok = createTransformGraph();

    return ok;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphTransform::postBuild()
{
    bool ok = true;

    ok = connectFiltersRender();

    return ok;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphTransform::start()
{
    bool ok = true;

    if ( !mStarted )
    {
        mStreamGroupInput->disableRead(false);

        ok = Inherited::start();
    }

    return ok;
}
