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
