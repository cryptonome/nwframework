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
#ifndef WIN32LABEL_H_
#define WIN32LABEL_H_

#include "Label.h"
#include "Win32Control.h"

//********************************************************************
//
//********************************************************************
class Win32Label : public Win32Control<::Label>
{
public:
    Win32Label ();
    virtual    ~Win32Label()                      { Win32Label::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    // Label
    virtual void setText(const char* _text) { win32SetText(_text); }
    virtual const char* getText() const { return win32GetText(); }

private:
    typedef Win32Control<::Label> Inherited;

    bool createWindow(GUIControl* _parent);
};

#endif
