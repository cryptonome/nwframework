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
#ifndef STRID_H_
#define STRID_H_

#include <string>

//********************************************************************
//
//********************************************************************
class StrId
{
public:
    StrId();
    StrId(const char* _string);
    StrId(std::string _string);

    const char* c_str() const { return mString.c_str(); }

private:
    std::string mString;
};


bool operator == (const StrId& _s1, const StrId& _s2);

#endif
