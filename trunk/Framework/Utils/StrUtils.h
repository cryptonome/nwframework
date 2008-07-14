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
#ifndef STR_UTILS_H__
#define STR_UTILS_H__

#include <string>

//********************************************************************
//
//********************************************************************
namespace StrUtils
{
    // Ansi/Uf8 conversions
    std::string Utf8ToAnsi(const char* pszText);
    std::string AnsiToUtf8(const char* pszText);

    std::wstring multiCharToUniChar(const char* mbString);
    std::string uniCharToMultiChar(const wchar_t* _str);

    std::string iToA(int _val);
    int aToI(const char* _str);

    char* stdStringToCharPointer(const std::string& _string);

    bool getNextToken(const char* _string, const char* _delimit, std::string& token_, std::string& newString_ );
};

#endif //STR_UTILS_H__
