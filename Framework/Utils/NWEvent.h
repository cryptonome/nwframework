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
#ifndef _INCREW_EVENT_H_
#define _INCREW_EVENT_H_

enum eNWEventTimeout
{
    NWE_INFINITE = 0xffffffff
};

class NWEvent
{
public:
    virtual void signal() = 0;
    virtual void reset() = 0;

    virtual bool isSignaled() = 0;
    
    virtual bool waitForSignal(unsigned int _msTimeout=NWE_INFINITE) = 0;

    virtual const char * getName() = 0;

    static NWEvent * create(bool _manualReset=false, bool _initialState=false, const char * name=0);
    static void destroy(NWEvent* & _cs);

protected:
    NWEvent(bool _manualReset=false, bool _initialState=false, const char * name=0){}
    virtual ~NWEvent(){}
};

#endif // _INCREW_EVENT_H_
