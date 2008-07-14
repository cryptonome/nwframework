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
#ifndef JUCEGROUPBOX_H_
#define JUCEGROUPBOX_H_

#include "GroupBox.h"
#include "JuceControl.h"

class JuceGroupBoxImp;

//********************************************************************
//
//********************************************************************
class JuceGroupBox : public JuceControl<GroupBox>
{
public:
    JuceGroupBox ();
    virtual    ~JuceGroupBox()                      { JuceGroupBox::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    // Text
    virtual void setText(const char* _text);
    virtual const char* getText() const;

private:
    typedef JuceControl<GroupBox> Inherited;

    JuceGroupBoxImp* mJuceGroupBox;
};

#endif
