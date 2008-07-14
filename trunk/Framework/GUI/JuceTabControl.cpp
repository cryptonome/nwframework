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

#include "JuceTabControl.h"
#include "juce.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
class JuceTabControlImp : public juce::TabbedComponent
{
public:
    JuceTabControlImp(const char* _name) : juce::TabbedComponent(TabbedButtonBar::TabsAtTop) { }
    ~JuceTabControlImp() {}
};


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
JuceTabControl::JuceTabControl() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool JuceTabControl::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mJuceTabControl = new JuceTabControlImp(_name);
        mJuceTabControl->setSize(200,40);
        mJuceTabControl->setVisible(true);

        setJuceComponent(this,mJuceTabControl);

        bOK = Inherited::init(_gui,_name,_parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void JuceTabControl::done()
{
    if (isOk())
    {

        Inherited::done();
    }
}


