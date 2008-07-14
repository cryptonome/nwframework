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
#ifndef WIN32COMBOBOX_H_
#define WIN32COMBOBOX_H_

#include "ComboBox.h"
#include "Win32Control.h"

//********************************************************************
//
//********************************************************************
class Win32ComboBox : public Win32Control<::ComboBox>
{
public:
    Win32ComboBox ();
    virtual    ~Win32ComboBox()                      { Win32ComboBox::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

private:
    typedef Win32Control<::ComboBox> Inherited;

    // ComboBox
    virtual void selectItem(int _index);
    virtual int getSelectedItem() const;
    virtual void internalAddItem(const char* _text);
    virtual void internalClearItemsList();

    // Win32ControlData
    virtual bool sendMsg(UINT message, WPARAM wParam, LPARAM lParam);

    bool createWindow(GUIControl* _parent);
};

#endif
