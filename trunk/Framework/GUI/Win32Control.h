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
