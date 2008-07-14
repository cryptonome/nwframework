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
