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
#ifndef NWIP_H_
#define NWIP_H_

#include <string>

//********************************************************************
//
//********************************************************************
class NWIP
{
public:
    NWIP();
    NWIP(const char* _str);

    bool setFromStr(const char* _str);
    const std::string getAsStr() const;

    // Data
    union
    {
        u8 fields[4];
        struct 
        {
            u8 a;
            u8 b;
            u8 c;
            u8 d;
        };
    };
};

#endif
