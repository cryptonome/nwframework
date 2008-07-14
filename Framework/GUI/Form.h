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
#ifndef FORM_H_
#define FORM_H_

#include "GUIControl.h"
#include "Button.h"
#include "CheckBox.h"

class FormControl;

//********************************************************************
//
//********************************************************************
class Form : public GUIControl, public ButtonEventHandler, public CheckBoxEventHandler
{
public:
    Form   ();
    virtual    ~Form  ()                      { Form::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent, const char* _fileName, const char* _context);
    virtual void          done        ();

    // Type
    virtual StrId getControlType() const;
    static StrId staticGetControlType();

    // Child
    virtual void addChild(GUIControl* _child);
    virtual int getNumChilds() const;
    virtual GUIControl* getChild(int _index) const;

    // Name
    virtual void setName(const char* _name);
    virtual const char* getName() const;

    // Position/Size
    virtual void setPosition(int _x, int _y);
    virtual void getPosition(int& _x, int& _y) const;
    virtual void setSize(int _width, int _height);
    virtual void getSize(int& _width, int& _height) const;

    // Visible
    virtual void setVisible(bool _visible);
    virtual bool getVisible() const;

    // Enabled
    virtual void setEnabled(bool _enabled);
    virtual bool getEnabled() const;

    virtual GUIControl* getControl ();

protected:
    // GUIControl
    virtual void removeChild(GUIControl* _control);

    bool registerEventsFromControl(const char* _name);
    bool registerEventsFromControl(GUIControl* _control);
    void registerEventsFromButtons();

private:
    friend class FormControl;
    void formControlRemoved();

    typedef GUIControl Inherited;
    FormControl* mFormControl;
};


#endif
