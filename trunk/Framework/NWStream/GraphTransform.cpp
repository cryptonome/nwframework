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
#include "PchNWStream.h"

#include "GraphTransform.h"
#include "NWStreamGroup.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GraphTransform::GraphTransform() :
    mInit(false),
    mStreamGroupOutput(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GraphTransform::init()
{
    bool bOK = true;

    if (!isOk())
    {
        mStreamGroupOutput = NEW NWStreamGroupWrite();
        bOK = mStreamGroupOutput->init("Graph Transform Output");

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GraphTransform::done()
{
    if (isOk())
    {
        DISPOSE(mStreamGroupOutput);
        mInit = false;
    }
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GraphTransform::connectInputGraph(INWGraph* _graph)
{
    bool bOK = true;


    return bOK;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
INWStreamGroupWrite* GraphTransform::getStreamGroupOutput()
{
    return mStreamGroupOutput;
}
