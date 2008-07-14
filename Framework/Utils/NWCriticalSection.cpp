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
#include "PchUtils.h"

#include "NWCriticalSection.h"

#include <windows.h>

typedef CRITICAL_SECTION NWCriticalSectionHandle;

class NWCriticalSectionW32 : public NWCriticalSection
{
public:
    NWCriticalSectionW32();
    virtual ~NWCriticalSectionW32();

    virtual void enter();
    virtual void leave();

private:
    NWCriticalSectionHandle mHandle;
};

//****************************************************************************
//
//****************************************************************************
/*static*/ NWCriticalSection * NWCriticalSection::create()
{
    return NEW NWCriticalSectionW32();
}

/*static*/ void NWCriticalSection::destroy(NWCriticalSection* & _cs)
{
    DISPOSE(_cs);
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWCriticalSectionW32::NWCriticalSectionW32()
{
    InitializeCriticalSection(&mHandle);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWCriticalSectionW32::~NWCriticalSectionW32()
{
    DeleteCriticalSection(&mHandle);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWCriticalSectionW32::enter()
{
    EnterCriticalSection(&mHandle);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWCriticalSectionW32::leave()
{
    LeaveCriticalSection(&mHandle);
}
