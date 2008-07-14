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
#include "PchUtils.h"
#include "Utils.h"

//********************************************************************
//
//********************************************************************
namespace Utils
{
    bool mCheckMemLeaks = true;

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool init(bool _checkMemLeaks)
{
    bool bOK = true;

    mCheckMemLeaks = _checkMemLeaks;

    return bOK;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void done()
{
    if ( mCheckMemLeaks )
        MemoryUtils::CheckMemoryLeaks();
}

} // Utils
