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
#include "PchGUI.h"

#include "JuceGroupBox.h"
#include "juce.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
class JuceGroupBoxImp : public juce::GroupComponent
{
public:
    JuceGroupBoxImp(const char* _name) : juce::GroupComponent(_name,_name) { }
    ~JuceGroupBoxImp() {}
};


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
JuceGroupBox::JuceGroupBox() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool JuceGroupBox::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mJuceGroupBox = new JuceGroupBoxImp(_name);
        mJuceGroupBox->setSize(200,40);
        mJuceGroupBox->setVisible(true);

        setJuceComponent(this,mJuceGroupBox);

        bOK = Inherited::init(_gui,_name,_parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void JuceGroupBox::done()
{
    if (isOk())
    {

        Inherited::done();
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void JuceGroupBox::setText(const char* _text)
{
    mJuceGroupBox->setText(_text);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
const char* JuceGroupBox::getText() const
{
    // TODO: remove this static
    static std::string mTemp;
    mTemp = mJuceGroupBox->getText();
    return mTemp.c_str();
}

