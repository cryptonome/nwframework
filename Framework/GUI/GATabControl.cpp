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
