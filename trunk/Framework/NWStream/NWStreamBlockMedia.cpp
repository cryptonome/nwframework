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

#include "NWStreamBlockMedia.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamBlockMedia::NWStreamBlockMedia() : Inherited(),
    mEnd(false),
    mTime(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamBlockMedia::init(ENWStreamSubType _subType)
{
    bool bOK = true;

    if (!isOk())
    {
        mEnd = false;
        mTime = 0;
        bOK = Inherited::init(NWSTREAM_TYPE_MEDIA,_subType);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockMedia::done()
{
    if (isOk())
    {

        Inherited::done();
    }
}



//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockMedia::setTime(u64 _time)
{
    mTime = _time;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
u64 NWStreamBlockMedia::getTime() const
{
    return mTime;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockMedia::setEnd(bool _end)
{
    mEnd = _end;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamBlockMedia::IsEnd() const
{
    return mEnd;
}

