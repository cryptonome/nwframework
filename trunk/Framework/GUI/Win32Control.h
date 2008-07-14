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
*
*      Permission is hereby granted, free of charge, to any person obtaining
*      a copy of this software and associated documentation files (the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*      distribute, sublicense, and/or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions:
*
*      The above copyright notice and this permission notice shall be
*      included in all copies or substantial portions of the Software.
*
*      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
*      LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
*      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#ifndef WIN32CONTROL_H_
#define WIN32CONTROL_H_

#include "GUIControl.h"
#include <vector>
#include <windows.h>

//extern long improbableWindowNumber;

//********************************************************************
//
//********************************************************************
class Win32ControlData
{
public:
    virtual void setParent(Win32ControlData* _control) = 0;
    virtual HWND getHWnd() = 0;
    virtual bool sendMsg(UINT message, WPARAM wParam, LPARAM lParam) = 0;
    virtual GUIControl* getGUIControl() = 0;
};

//********************************************************************
//
//********************************************************************
template <typename T>
class Win32Control : public T , public Win32ControlData
{
public:
    Win32Control() : mhWnd(0) { }
    virtual    ~Win32Control () { Win32Control::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void done();

    // GUIControl
    virtual void addChild(GUIControl* _child);
    virtual int getNumChilds() const;
    virtual GUIControl* getChild(int _index) const;

    virtual const char* getName() const;
    virtual void setName(const char* _name);

    virtual void setPosition(int _x, int _y);
    virtual void getPosition(int& _x, int& _y) const;
    virtual void setSize(int _width, int _height);
    virtual void getSize(int& _width, int& _height) const;

    virtual void setVisible(bool _visible);
    virtual bool getVisible() const;

    virtual void setEnabled(bool _enabled);
    virtual bool getEnabled() const;

    // Win32ControlData
    virtual bool sendMsg(UINT message, WPARAM wParam, LPARAM lParam) { return false; }

protected:
    friend class Win32ControlFactory;
    typedef T Inherited;

    bool existChild(GUIControl* _control);

    // GUIControl
    virtual void removeChild(GUIControl* _control);

    // Win32ControlData
    virtual void setParent(Win32ControlData* _control);
    virtual HWND getHWnd();
    virtual GUIControl* getGUIControl() { return this; }

    void win32SetText(const char* _text);
    const char* win32GetText() const;

    static Win32ControlData* getWin32ControlDataFromControl(GUIControl* _control) { return dynamic_cast<Win32ControlData*>(_control->getControl()); }

    HWND mhWnd;
    std::vector<GUIControl*> mChilds;
    std::string mName;
};

#include "Win32Control.hpp"

#endif
