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
