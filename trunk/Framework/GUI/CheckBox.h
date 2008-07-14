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
#ifndef CHECKBOX_H_
#define CHECKBOX_H_

#include "GUIControl.h"

class CheckBox;

//********************************************************************
//
//********************************************************************
class CheckBoxEventHandler
{
public:
    virtual ~CheckBoxEventHandler() {}

    virtual void checkBoxPressed(::CheckBox* _checkbox) { }
    virtual void checkBoxModified(::CheckBox* _checkbox) { }
};

//********************************************************************
//
//********************************************************************
class CheckBox : public GUIControl
{
public:
    CheckBox     ();
    virtual    ~CheckBox    ()                      { CheckBox::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    // Text
    virtual void setText(const char* _text) = 0;
    virtual const char* getText() const = 0;

    // Checked
    virtual void setChecked(bool _checked, bool _sendMofifiedEvent = true) = 0;
    virtual bool getChecked() const = 0;

    void setCheckBoxEventHandler(CheckBoxEventHandler* _eventHandler) { mCheckBoxEventHandler = _eventHandler; }

    virtual bool load(const DataScript& _data);
    virtual void save(DataScript& _data);

    virtual StrId getControlType() const;
    static StrId staticGetControlType();

protected:
    // Events
    void sendEventCheckBoxPressed();
    void sendEventCheckBoxModified();

private:
    typedef GUIControl Inherited;

    CheckBoxEventHandler* mCheckBoxEventHandler;
};

#endif
