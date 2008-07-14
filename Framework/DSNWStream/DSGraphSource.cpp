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

#include "DSGraphSource.h"
#include "NWStreamGroup.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSGraphSource::DSGraphSource() : Inherited(),
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
bool DSGraphSource::init()
{
    bool bOK = true;

    if (!isOk())
    {
        bOK = Inherited::init();

        mBuilt = false;
        mStreamGroupOutput = NEW NWStreamGroupWrite();
        bOK = mStreamGroupOutput->init("GraphSource Output");
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraphSource::done()
{
    if (isOk())
    {
        mStreamGroupOutput->disableWrite(true);
        Inherited::done();
        DISPOSE(mStreamGroupOutput);
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
INWStreamGroupWrite* DSGraphSource::getStreamGroupOutput()
{
    return mStreamGroupOutput;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
/*bool DSGraphSource::inputGraphConnected()
{
    return connectFiltersRender();
}*/
bool DSGraphSource::build()
{
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphSource::postBuild()
{
    bool ok = true;

    ASSERT(!mBuilt);

    if ( !mBuilt )
    {
        ok = connectFiltersRender();
        mBuilt = true;
    }

    return ok;
}