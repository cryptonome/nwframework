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
