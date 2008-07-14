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

#include "JuceWindow.h"
#include "juce.h"
#include "FormControl.h"
#include "GUI.h"
#include "ControlFactory.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
class JuceWindowImp : public juce::DocumentWindow
{
public:
    JuceWindowImp(const char* _name, JuceWindow* _juceWindow) : juce::DocumentWindow(_name,Colour(0xffffffff),DocumentWindow::minimiseButton|DocumentWindow::maximiseButton|DocumentWindow::closeButton,true), mJuceWindow(_juceWindow) { }
    ~JuceWindowImp() {}

    void closeButtonPressed()
    {
        mJuceWindow->sendEventCloseButtonPressed();
    }

    JuceWindow* mJuceWindow;
};


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
JuceWindow::JuceWindow() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool JuceWindow::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mJuceWindow = new JuceWindowImp(_name,this);
        mJuceWindow->setVisible(true);
        mJuceWindow->setSize(640,480);
        mJuceWindow->setResizable(true,true);

        setJuceComponent(this,mJuceWindow);

        FormControl* mainForm = _gui->getControlFactory()->create<FormControl>("MainForm",_parent);
        mJuceWindow->setContentComponent(getJuceComponentFromControl(mainForm));

        bOK = Inherited::init(_gui, _name, _parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void JuceWindow::done()
{
    if (isOk())
    {
        Inherited::done();
        DISPOSE(mJuceWindow);
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void JuceWindow::addChild(GUIControl* _child)
{
    juce::Component* juceComponentChild = getJuceComponentFromControl(_child);
    mJuceWindow->getContentComponent()->addChildComponent(juceComponentChild);
}
