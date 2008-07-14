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

#include "NWStreamVideo.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamVideo::NWStreamVideo() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamVideo::init()
{
    bool bOK = true;

    if (!isOk())
    {

        bOK = Inherited::init(NWSTREAM_SUBTYPE_MEDIA_VIDEO, NWSTREAM_MEDIATYPE_VIDEO);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamVideo::done()
{
    if (isOk())
    {

        Inherited::done();
    }
}
