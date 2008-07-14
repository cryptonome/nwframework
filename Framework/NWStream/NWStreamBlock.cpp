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

#include "NWStreamBlock.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamBlock::NWStreamBlock() :
    mInit(false)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamBlock::init(ENWStreamType _type, ENWStreamSubType _subType)
{
    bool bOK = true;

    if (!isOk())
    {
        mType = _type;
        mSubType = _subType;

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlock::done()
{
    if (isOk())
    {

        mInit = false;
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
ENWStreamType NWStreamBlock::getType() const
{
    return mType;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
ENWStreamSubType NWStreamBlock::getSubType() const
{
    return mSubType;
}

