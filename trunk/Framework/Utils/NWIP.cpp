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
*
*      Permission is hereby granted, free of charge, to any person obtaining
*      a copy of this software and associated documentation files (the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*      distribute, sublicense, and/or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions:
*
*      The above copyright notice and this permission notice shall be
*      included in all copies or substantial portions of the Software.
*
*      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
*      LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
*      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

