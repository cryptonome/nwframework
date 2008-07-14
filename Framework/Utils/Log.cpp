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

#include "Log.h"
#include <string>

// Visual Studio
#ifdef _MSC_VER
    #include <windows.h>
#endif


//********************************************************************
//
//********************************************************************
namespace Log
{

  
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Printf(const char* _msg, ...)
{
    va_list args; 
    va_start (args, _msg);
      
    int iLen = _vscprintf(_msg, args) + 1; 

    char* fullMsg = NEW char[iLen]; 
    vsprintf (fullMsg, _msg, args);
    va_end (args);
    std::string msg = fullMsg;
    msg += "\n";
    DISPOSE_ARRAY(fullMsg);

    // TODO: support variable parameters
    #ifdef _MSC_VER
        OutputDebugString(msg.c_str());
    #endif // Visual Studio
}

};
