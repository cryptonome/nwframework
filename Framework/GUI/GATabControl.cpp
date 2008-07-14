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
#include "PchGUI.h"
#include "GATabControl.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GATabControlString::initAdapter()
{
    mControl->setTabControlEventHandler(this);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void  GATabControlString::tabControlModified(::TabControl* _tabControl)
{
    ASSERT(_tabControl == mControl);
    Tab* tab = mControl->getActiveTab();
    if ( tab )
        mData.set(tab->getName());
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GATabControlString::update(const char * _contextName, const char * _objName)
{
    std::string val = mData.get();
    Tab* tab = mControl->getTabByName(val.c_str());
    ASSERT(tab);
    if ( tab )
        tab->setActive();
}
