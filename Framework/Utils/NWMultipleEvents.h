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

