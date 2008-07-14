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

#include "JuceButton.h"
#include "juce.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
class JuceButtonImp : public juce::TextButton
{
public:
    JuceButtonImp(const char* _name) : juce::TextButton(_name) { }
    ~JuceButtonImp() {}
};


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
JuceButton::JuceButton() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool JuceButton::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mJuceButton = new JuceButtonImp(_name);
        mJuceButton->setSize(200,40);
        mJuceButton->setVisible(true);

        setJuceComponent(this,mJuceButton);

        bOK = Inherited::init(_gui,_name, _parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void JuceButton::done()
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
void JuceButton::setText(const char* _text)
{
    mJuceButton->setButtonText(_text);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
const char* JuceButton::getText() const
{
    // TODO: remove this static
    static std::string mTemp;
    mTemp = mJuceButton->getButtonText();
    return mTemp.c_str();
}

