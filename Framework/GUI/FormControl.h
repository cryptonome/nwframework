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
#ifndef FORMCONTROL_H_
#define FORMCONTROL_H_

#include "GUIControl.h"

class Form;

//********************************************************************
//
//********************************************************************
class FormControl : public GUIControl
{
public:
    FormControl       ();
    virtual    ~FormControl      ()                      { FormControl::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    virtual StrId getControlType() const;
    static StrId staticGetControlType();

    void setParentToDelete(Form* _parent);

private:
    typedef GUIControl Inherited;

    Form* mParentToDelete;
};

#endif
