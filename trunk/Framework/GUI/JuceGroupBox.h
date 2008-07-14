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
