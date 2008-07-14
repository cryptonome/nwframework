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
