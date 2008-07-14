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
