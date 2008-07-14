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
