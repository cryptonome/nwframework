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

