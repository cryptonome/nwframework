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
#ifndef LOG_H__
#define LOG_H__


//*****************************************************************************
// Debug
//*****************************************************************************
//#ifdef _DEBUG
    
    #define LOG Log::Printf


//*****************************************************************************
// Release
//*****************************************************************************
//#else

    //#define LOG

//#endif

//********************************************************************
//
//********************************************************************
namespace Log
{
    void Printf(const char* _msg, ...);
};

#endif //LOG_H__
