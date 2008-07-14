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
