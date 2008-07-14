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
#include "PchGUI.h"

#include "JuceComboBox.h"
#include "juce.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
class JuceComboBoxImp : public juce::ComboBox
{
public:
    JuceComboBoxImp(const char* _name) : juce::ComboBox(_name) { }
    ~JuceComboBoxImp() {}
};


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
JuceComboBox::JuceComboBox() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool JuceComboBox::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mJuceComboBox = new JuceComboBoxImp(_name);
        mJuceComboBox->setSize(200,40);
        mJuceComboBox->setVisible(true);

        setJuceComponent(this,mJuceComboBox);

        bOK = Inherited::init(_gui,_name,_parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void JuceComboBox::done()
{
    if (isOk())
    {

        Inherited::done();
    }
}


