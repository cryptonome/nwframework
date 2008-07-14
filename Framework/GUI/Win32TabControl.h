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
#ifndef WIN32TABCONTROL_H_
#define WIN32TABCONTROL_H_

#include "TabControl.h"
#include "Win32Control.h"

//********************************************************************
//
//********************************************************************
class Win32TabControl : public Win32Control<::TabControl>
{
public:
    Win32TabControl ();
    virtual    ~Win32TabControl()                      { Win32TabControl::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

private:
    typedef Win32Control<::TabControl> Inherited;

    bool createWindow(GUIControl* _parent);

    // TabControl
    virtual void setActiveTab(Tab* _tab);
    virtual Tab* getActiveTab();
    virtual Tab* createTab(const char* _name, Form* _form, int _index);
    virtual void destroyTab(Tab* _tab);
    virtual void setTabName(Tab* _tab, const char* _name);

    // Win32ControlData
    virtual bool sendMsg(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif
