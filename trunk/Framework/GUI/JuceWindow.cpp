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
