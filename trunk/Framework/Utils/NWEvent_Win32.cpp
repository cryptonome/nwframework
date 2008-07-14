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

#include "NWEvent_Win32.h"

//****************************************************************************
// Instanciation
//****************************************************************************
/*static*/ NWEvent * NWEvent::create(bool _manualReset/*=false*/, bool _initialState/*=false*/, const char * _name/*=0*/)
{
    return NEW NWEventW32(_manualReset, _initialState, _name);
}

/*static*/ void NWEvent::destroy(NWEvent* & _cs)
{
    DISPOSE(_cs);
}

//****************************************************************************
//
//****************************************************************************
NWEventW32::NWEventW32(bool _manualReset/*=false*/, bool _initialState/*=false*/, const char * _name/*=0*/)
{
    mEventHandle = CreateEvent(NULL, _manualReset, _initialState, _name);
}

NWEventW32::~NWEventW32()
{
    CloseHandle(mEventHandle);
}

//****************************************************************************
//
//****************************************************************************
void NWEventW32::signal()
{
    SetEvent(mEventHandle);
}

void NWEventW32::reset()
{
    ResetEvent(mEventHandle);
}

//****************************************************************************
//
//****************************************************************************
bool NWEventW32::isSignaled()
{
    bool bRet = false;

    DWORD result = WaitForSingleObject(mEventHandle, 0);
    
    ASSERT(result != WAIT_ABANDONED);

    if(result == WAIT_OBJECT_0)
    {
        bRet = true;
    }

    return bRet;
}

bool NWEventW32::waitForSignal(unsigned int _msTimeout/*=NWE_INFINITE*/)
{
    bool bRet = false;

    DWORD result = WaitForSingleObject(mEventHandle, _msTimeout == NWE_INFINITE ? INFINITE : _msTimeout);
    
    ASSERT(result != WAIT_ABANDONED);

    if(result == WAIT_OBJECT_0)
    {
        bRet = true;
    }

    return bRet;
}

//****************************************************************************
//
//****************************************************************************
const char * NWEventW32::getName()
{
    return mEventName.c_str();
}
