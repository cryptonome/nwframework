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
