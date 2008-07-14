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

