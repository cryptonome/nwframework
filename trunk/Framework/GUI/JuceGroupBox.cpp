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

