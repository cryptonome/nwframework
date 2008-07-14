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

#include "JuceLabel.h"
#include "juce.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
class JuceLabelImp : public juce::Label
{
public:
    JuceLabelImp(const char* _name) : juce::Label(_name,"") { }
    ~JuceLabelImp() {}
};


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
JuceLabel::JuceLabel() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool JuceLabel::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mJuceLabel = new JuceLabelImp(_name);
        mJuceLabel->setSize(200,40);
        mJuceLabel->setVisible(true);

        setJuceComponent(this,mJuceLabel);

        bOK = Inherited::init(_gui,_name,_parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void JuceLabel::done()
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
void JuceLabel::setText(const char* _text)
{
    mJuceLabel->setText(_text,true);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
const char* JuceLabel::getText() const
{
    // TODO: remove this static
    static std::string mTemp;
    mTemp = mJuceLabel->getText();
    return mTemp.c_str();
}

