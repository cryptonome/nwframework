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
#ifndef _INCREW_MULTIPLE_EVENTS_H_
#define _INCREW_MULTIPLE_EVENTS_H_

class NWEvent;

#include <vector>

//****************************************************************************
//
//****************************************************************************
enum eNWMultipleEventsDefs
{
    NWME_INVALID_EVENT = -1,
    NWME_TIMEOUT = -2,

    NWME_INFINITE = 0xffffffff
};

class NWMultipleEvents
{
public:
    NWMultipleEvents();
    NWMultipleEvents(NWEvent * _event0, NWEvent * _event1);
    NWMultipleEvents(NWEvent * _event0, NWEvent * _event1, NWEvent * _event2);
    NWMultipleEvents(NWEvent * _event0, NWEvent * _event1, NWEvent * _event2, NWEvent * _event3);
    ~NWMultipleEvents();

    int getNumEvents();
    void addEvent(NWEvent * _event);
    void removeEvent(int _num);
    NWEvent * getEvent(int _num);

    void signalEvent(int _num);
    void resetEvent(int _num);

    void signalEvents();
    void resetEvents();

    bool isSignaled(int _num);
    int anySignaled();  // returns NWME_INVALID_EVENT if no one is signaled
    bool allSignaled();
    
    int waitForSignal(unsigned int _msTimeout=NWME_INFINITE, bool _waitAll=false); // returns NWME_INVALID_EVENT on error

private:
    std::vector<NWEvent *> mEvents;
};

#endif // _INCREW_MULTIPLE_EVENTS_H_

