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
