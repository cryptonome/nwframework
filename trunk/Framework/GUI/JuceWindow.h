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
