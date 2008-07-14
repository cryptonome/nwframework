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
#ifndef WIN32WINDOW_H_
#define WIN32WINDOW_H_

#include "Window.h"
#include "Win32Control.h"

//********************************************************************
//
//********************************************************************
class Win32Window : public Win32Control<Window>
{
public:
    Win32Window ();
    virtual    ~Win32Window()                      { Win32Window::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    virtual void setSize(int _width, int _height);
    virtual void getSize(int& _width, int& _height) const;

    // Window
    virtual void setMenu(WindowMenu* _menu);
    virtual void sendUserMsg(int _id);

private:
    typedef Win32Control<Window> Inherited;
    friend LRESULT win32WindowProc(HWND h, UINT message, WPARAM wParam, LPARAM lParam);
    typedef Win32Control<Window> Inherited;

    // Win32ControlData
    virtual bool sendMsg(UINT message, WPARAM wParam, LPARAM lParam);

    bool createWindow(const char* _name, GUIControl* _parent);
    void addMenuItem(WindowMenu* _item, HMENU _hMenu, int _index, int& _globalindex_, bool _addOwn);

    void getWindowBorderSize(int& width_, int& height_) const;

    void msgClose();

    HMENU mhMenu;
    WindowMenu* mMenu;
};

#endif
