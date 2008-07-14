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
#ifndef JUCEWINDOW_H_
#define JUCEWINDOW_H_

#include "Window.h"
#include "JuceControl.h"

class JuceWindowImp;

//********************************************************************
//
//********************************************************************
class JuceWindow : public JuceControl<Window>
{
public:
    JuceWindow ();
    virtual    ~JuceWindow()                      { JuceWindow::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    virtual void addChild(GUIControl* _child);


private:
    friend class JuceWindowImp;
    typedef JuceControl<Window> Inherited;

    JuceWindowImp* mJuceWindow;
};

#endif
