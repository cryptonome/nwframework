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
#ifndef _INCREW_CRITICAL_SECTION_H_
#define _INCREW_CRITICAL_SECTION_H_

class NWCriticalSection
{
public:
    virtual void enter() = 0;
    virtual void leave() = 0;

    static NWCriticalSection * create();
    static void destroy(NWCriticalSection* & _cs);

protected:
    NWCriticalSection(){}
    virtual ~NWCriticalSection(){}
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class NWAutoCritSec
{
public:
    inline NWAutoCritSec(NWCriticalSection * _critSec);
    inline ~NWAutoCritSec();

private:
    NWCriticalSection * mCritSec;
};

inline NWAutoCritSec::NWAutoCritSec(NWCriticalSection * _critSec)
{
    mCritSec = _critSec;
    if(mCritSec)
        mCritSec->enter();
}

inline NWAutoCritSec::~NWAutoCritSec()
{
    if(mCritSec)
        mCritSec->leave();
    mCritSec = 0;
}

#endif // _INCREW_CRITICAL_SECTION_H_
