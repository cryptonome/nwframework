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

#include "JuceForm.h"
#include "juce.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
class JuceFormImp : public juce::Viewport
{
public:
    JuceFormImp(const char* _name) : juce::Viewport(_name) { }
    ~JuceFormImp() {}
};


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
JuceForm::JuceForm() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool JuceForm::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mJuceForm = new JuceFormImp(_name);
        mJuceForm->setSize(200,40);
        mJuceForm->setVisible(true);

        setJuceComponent(this,mJuceForm);

        bOK = Inherited::init(_gui,_name,_parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void JuceForm::done()
{
    if (isOk())
    {

        Inherited::done();
    }
}

