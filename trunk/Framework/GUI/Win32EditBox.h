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
#ifndef WIN32EDITBOX_H_
#define WIN32EDITBOX_H_

#include "EditBox.h"
#include "Win32Control.h"

//********************************************************************
//
//********************************************************************
class Win32EditBox : public Win32Control<::EditBox>
{
public:
    Win32EditBox ();
    virtual    ~Win32EditBox()                      { Win32EditBox::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    // EditBox
    virtual void setText(const char* _text, bool _sendMofifiedEvent);
    virtual const char* getText() const { return win32GetText(); }

private:
    typedef Win32Control<::EditBox> Inherited;

    // Win32ControlData
    virtual bool sendMsg(UINT message, WPARAM wParam, LPARAM lParam);

    bool createWindow(GUIControl* _parent);
};

#endif
