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
#include "NWIP.h"
#include "StrUtils.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWIP::NWIP()
{
    a = b = c = d = 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWIP::NWIP(const char* _str)
{
    setFromStr(_str);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWIP::setFromStr(const char* _str)
{
    bool valid = true;

    // Parse the string
    std::string str = _str;
    std::string strNew;
    std::string token;
    u8 tempFields[4] = { 0 };
    int currentField = 0; 
    while ( valid && StrUtils::getNextToken(str.c_str(),".",token,strNew) )
    {
        valid = currentField < 4;

        if ( valid )
        {
            int val = StrUtils::aToI(token.c_str());
            valid = val >= 0 && val <= 255;
            if ( valid )
                tempFields[currentField] = (u8)val;
        }

        str = strNew;
        currentField++;
    }

    if ( valid && currentField != 4)
        valid = false;

    ASSERT(valid);

    // Store the new values
    if ( valid )
    {
        for ( int i = 0 ; i < 4 ; ++i )
            fields[i] = tempFields[i];
    }

    return valid;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
const std::string NWIP::getAsStr() const
{
    char str[256];
    sprintf(str,"%u.%u.%u.%u",a,b,c,d);
    return str;
}

