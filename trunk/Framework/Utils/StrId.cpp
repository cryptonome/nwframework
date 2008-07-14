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

#include "StrId.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
StrId::StrId() :
    mString()
{
}

StrId::StrId(const char* _string)
{
    mString = _string;
}

StrId::StrId(const std::string _string)
{
    mString = _string;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool operator == (const StrId& _s1, const StrId& _s2)
{
    return strcmp(_s1.c_str(),_s2.c_str()) == 0;
}
