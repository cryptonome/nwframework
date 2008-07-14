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
#include "StrUtils.h"

namespace StrUtils
{

//*****************************************************************************
// Visual Studio 
//*****************************************************************************
#ifdef _MSC_VER 

    std::string Utf8ToAnsi(const char* pszText) 
    { 
        //ASSERT(false); // TODO
        return pszText; 
    }
    std::string AnsiToUtf8(const char* pszText) 
    { 
        ASSERT(false); // TODO
        return pszText; 
    }

#endif  // Visual Studio



//*****************************************************************************
// Borland Builder 
//*****************************************************************************
#ifdef __BORLANDC__ 

    std::string Utf8ToAnsi(const char* pszText)
    { 
        return Utf8ToAnsi(pszText).c_str(); 
    }
    
    std::string AnsiToUtf8(const char* pszText) 
    { 
        return AnsiToUtf8(pszText).c_str(); 
    }

#endif // Borland Builder


//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
std::wstring multiCharToUniChar(const char* _str)
{
    int len = (int)strlen(_str) + 1;
    wchar_t *ucString = NEW wchar_t[len];
    mbstowcs(ucString, _str, len);
    std::wstring str = ucString;
    DISPOSE_ARRAY(ucString);
    return str;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
std::string uniCharToMultiChar(const wchar_t* _str)
{
    int len = (int)wcslen(_str) + 1;
    char* mbStr = NEW char[len];
    wcstombs(mbStr,_str,len);
    std::string str = mbStr;
    return str;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
std::string iToA(int _val)
{
    char buffer[256];
    sprintf(buffer,"%d",_val);
    std::string str = buffer;
    return str;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int aToI(const char* _str)
{
    int val = atoi(_str);

    return val;
}


//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
char* stdStringToCharPointer(const std::string& _string)
{
    char* str = 0;

    str = NEW char[strlen(_string.c_str())+1];
    strcpy(str,_string.c_str());

    return str;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool getNextToken(const char* _string, const char* _delimit, std::string& token_, std::string& newString_ )
{
    bool exist = _string[0] != '\0';
    int endPos = 0;

    if ( exist )
    {
        // Find the next demilit char pos (or end of string)
        bool end = false;
        bool delimitCharFound = false;
        while ( !end )
        {
            // Get character from the current pos
            char currentChar = _string[endPos];

            if ( currentChar != '\0' )
            {
                // Current character is a delimit one?
                int delimitPos = 0;
                while ( !delimitCharFound && _delimit[delimitPos] != '\0' )
                {
                    delimitCharFound = currentChar == _delimit[delimitPos];
                    delimitPos++;
                }

                if ( delimitCharFound )
                    end = true;
                else
                    endPos++;
            }
            else
                end = true;
        }

        if ( delimitCharFound )
        {
            char* str = StrUtils::stdStringToCharPointer(_string);
            newString_ = &str[endPos+1];
            str[endPos] = '\0';
            token_ = str;
            DISPOSE_ARRAY(str);
        }
        else
        {
            newString_ = "";
            token_ = _string;
        }
    }

    return exist;
}




} // StrUtils


