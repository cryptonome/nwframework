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
#ifndef UTILS__
#define UTILS__

#include "MemoryUtils.h"
#include "ICAssert.h"
#include "Log.h"
#include "NWTypes.h"
#include "NWMath.h"

//********************************************************************
//
//********************************************************************
namespace Utils
{
    bool init (bool _checkMemLeaks = true);
    void done ();

} // Utils

#endif
