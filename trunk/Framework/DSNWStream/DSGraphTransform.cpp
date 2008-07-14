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
