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

#include "NWMultipleEvents.h"

#include "NWEvent_Win32.h"

//****************************************************************************
//
//****************************************************************************
static const unsigned int MULTIPLE_EVENTS_RESERVE = 32;

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWMultipleEvents::NWMultipleEvents()
{
    mEvents.reserve(MULTIPLE_EVENTS_RESERVE);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWMultipleEvents::NWMultipleEvents(NWEvent * _event0, NWEvent * _event1)
{
    mEvents.reserve(MULTIPLE_EVENTS_RESERVE);

    mEvents.push_back(_event0);
    mEvents.push_back(_event1);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWMultipleEvents::NWMultipleEvents(NWEvent * _event0, NWEvent * _event1, NWEvent * _event2)
{
    mEvents.reserve(MULTIPLE_EVENTS_RESERVE);

    mEvents.push_back(_event0);
    mEvents.push_back(_event1);
    mEvents.push_back(_event2);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWMultipleEvents::NWMultipleEvents(NWEvent * _event0, NWEvent * _event1, NWEvent * _event2, NWEvent * _event3)
{
    mEvents.reserve(MULTIPLE_EVENTS_RESERVE);

    mEvents.push_back(_event0);
    mEvents.push_back(_event1);
    mEvents.push_back(_event2);
    mEvents.push_back(_event3);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWMultipleEvents::~NWMultipleEvents()
{
    mEvents.clear();
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int NWMultipleEvents::getNumEvents()
{
    return (int)mEvents.size();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWMultipleEvents::addEvent(NWEvent * _event)
{
    mEvents.push_back(_event);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWMultipleEvents::removeEvent(int _num)
{
    if(_num < (int)mEvents.size())
        mEvents.erase(mEvents.begin() + _num);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWEvent * NWMultipleEvents::getEvent(int _num)
{
    return (_num < (int)mEvents.size()) ?  mEvents[_num] : NULL;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWMultipleEvents::signalEvent(int _num)
{
    int num = (int)mEvents.size();
    if(_num < num)
    {
        mEvents[_num]->signal();
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWMultipleEvents::resetEvent(int _num)
{
    int num = (int)mEvents.size();
    if(_num < num)
    {
        mEvents[_num]->reset();
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWMultipleEvents::signalEvents()
{
    int num = (int)mEvents.size();
    for(int i=0; i<num; i++)
    {
        mEvents[i]->signal();
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWMultipleEvents::resetEvents()
{
    int num = (int)mEvents.size();
    for(int i=0; i<num; i++)
    {
        mEvents[i]->reset();
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool NWMultipleEvents::isSignaled(int _num)
{
    bool bRet = false;

    int num = (int)mEvents.size();
    if(_num < num)
    {
        bRet = mEvents[_num]->isSignaled();
    }

    return bRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int NWMultipleEvents::anySignaled()
{
    int iRet = NWME_INVALID_EVENT;

    int num = (int)mEvents.size();
    for(int i=0; i<num; i++)
    {
        if(mEvents[i]->isSignaled())
        {
            iRet = i;
            break;
        }
    }

    return iRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool NWMultipleEvents::allSignaled()
{
    int num = (int)mEvents.size();

    bool bRet = num > 0;

    for(int i=0; i<num; i++)
    {
        bRet &= mEvents[i]->isSignaled();
    }

    return bRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int NWMultipleEvents::waitForSignal(unsigned int _msTimeout/*=NWME_INFINITE*/, bool _waitAll/*=false*/)
{
    int iRet = NWME_INVALID_EVENT;

    std::vector<NWEventHandle> mHandles;

    int num = (int)mEvents.size();
    mHandles.resize(num);

    for(int i=0; i<num; i++)
    {
        NWEventW32 * eventW32 = (NWEventW32 *)mEvents[i];
        mHandles[i] = eventW32->getHandle();
    }

    DWORD ret = WaitForMultipleObjects(num, &mHandles[0], _waitAll, (_msTimeout==NWME_INFINITE) ? INFINITE : _msTimeout);

    ASSERT(ret < WAIT_ABANDONED_0 || ret >= WAIT_ABANDONED_0 + num);
    ASSERT((ret >= WAIT_OBJECT_0 && ret < WAIT_OBJECT_0 + num) || ret == WAIT_TIMEOUT);

    if(ret >= WAIT_OBJECT_0 && ret < WAIT_OBJECT_0 + num)
        iRet = ret - WAIT_OBJECT_0;
    else if(ret == WAIT_TIMEOUT)
        iRet = NWME_TIMEOUT;

    return iRet;
}
