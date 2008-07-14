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
#ifndef WIN32CHECKBOX_H_
#define WIN32CHECKBOX_H_

#include "CheckBox.h"
#include "Win32Control.h"

//********************************************************************
//
//********************************************************************
class Win32CheckBox : public Win32Control<::CheckBox>
{
public:
    Win32CheckBox ();
    virtual    ~Win32CheckBox()                      { Win32CheckBox::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    // CheckBox
    virtual void setText(const char* _text) { win32SetText(_text); }
    virtual const char* getText() const { return win32GetText(); }
    virtual void setChecked(bool _checked, bool _sendMofifiedEvent = true);
    virtual bool getChecked() const;

private:
    typedef Win32Control<::CheckBox> Inherited;

    // Win32ControlData
    virtual bool sendMsg(UINT message, WPARAM wParam, LPARAM lParam);

    bool createWindow(GUIControl* _parent);
};

#endif
