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

#include "GraphSource.h"
#include "NWStreamGroup.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GraphSource::GraphSource() :
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
bool GraphSource::init()
{
    bool bOK = true;

    if (!isOk())
    {
        mStreamGroupOutput = NEW NWStreamGroupWrite();
        bOK = mStreamGroupOutput->init("Graph Source Output");

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GraphSource::done()
{
    if (isOk())
    {
        stop();

        DISPOSE(mStreamGroupOutput);
        mInit = false;
    }
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
INWStreamGroupWrite* GraphSource::getStreamGroupOutput()
{
    return mStreamGroupOutput;
}

