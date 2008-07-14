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
#ifndef WINDOW_H_
#define WINDOW_H_

#include "GUIControl.h"
#include <list>

class Window;
class WindowMenu;
class NWCriticalSection;

//********************************************************************
//
//********************************************************************
class WindowEventHandler
{
public:
    virtual ~WindowEventHandler() {}

    virtual void windowCloseButtonPressed(Window* _window) { }
    virtual void windowMenuItemSelected(WindowMenu* _item) { }
    virtual void windowUserMsg(int _id) { }
};

//********************************************************************
//
//********************************************************************
class Window : public GUIControl
{
public:
    Window     ();
    virtual    ~Window    ()                      { Window::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    void setWindowEventHandler(WindowEventHandler* _eventHandler) { mWindowEventHandler = _eventHandler; }

    virtual void setMenu(WindowMenu* _menu) = 0;

    virtual int addUserMsg();
    virtual void sendUserMsg(int _id) = 0;

    virtual StrId getControlType() const;
    static StrId staticGetControlType();

protected:
    typedef GUIControl Inherited;

    // Events
    void sendEventCloseButtonPressed();

    WindowEventHandler* mWindowEventHandler;

    struct UserMessage
    {
        int mId;
        bool mPending;
    };
    UserMessage* getUserMessage(int _id) const;

    std::list<UserMessage*> mUserMessages;
    NWCriticalSection* mcsUserMessages;
    int mNextUserMsgId;
};


#endif
