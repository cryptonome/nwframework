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
