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
#ifndef JUCEBUTTON_H_
#define JUCEBUTTON_H_

#include "Button.h"
#include "JuceControl.h"

class JuceButtonImp;

//********************************************************************
//
//********************************************************************
class JuceButton : public JuceControl<::Button>
{
public:
    JuceButton ();
    virtual    ~JuceButton()                      { JuceButton::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    // Text
    virtual void setText(const char* _text);
    virtual const char* getText() const;

private:
    typedef JuceControl<Button> Inherited;

    JuceButtonImp* mJuceButton;
};

#endif
