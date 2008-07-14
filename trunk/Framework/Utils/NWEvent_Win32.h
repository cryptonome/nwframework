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
#ifndef _INCREW_EVENT_WIN32_H_
#define _INCREW_EVENT_WIN32_H_

#include "NWEvent.h"

#include <string>
#include <windows.h>

#include <string>

//****************************************************************************
//
//****************************************************************************
typedef HANDLE NWEventHandle;

class NWEventW32 : public NWEvent
{
public:
    enum eNWEventTimeout
    {
        NWE_INFINITE = -1
    };

    NWEventW32(bool _manualReset=false, bool _initialState=false, const char * name=0);
    virtual ~NWEventW32();

    virtual void signal();
    virtual void reset();

    virtual bool isSignaled();
    
    virtual bool waitForSignal(unsigned int _msTimeout=NWE_INFINITE);

    virtual const char * getName();

    inline NWEventHandle getHandle();

private:
    std::string mEventName;
    NWEventHandle mEventHandle;
};

inline NWEventHandle NWEventW32::getHandle()
{
    return mEventHandle;
}

#endif // _INCREW_EVENT_WIN32_H_
