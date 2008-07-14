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

#include "JuceControlFactory.h"
#include "JuceWindow.h"
#include "JuceButton.h"
#include "JuceForm.h"
#include "JuceGroupBox.h"
#include "JuceTabControl.h"
#include "JuceComboBox.h"
#include "JuceSlider.h"
#include "JuceLabel.h"

#define CREATE_JUCEFACTORY_CONTROL_INSTANCE(juceClassType) \
    if ( control == 0 && _type == juceClassType::staticGetControlType() ) \
    { \
        juceClassType* juceControl = NEW (juceClassType)(); \
        if ( juceControl->init(mGUI,_name,_parent) ) \
            control = juceControl; \
        else \
            DISPOSE(juceControl); \
    }


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
ControlFactory* JuceControlFactory::createFactory(GUI* _gui)
{
    JuceControlFactory* factory = NEW JuceControlFactory();

    bool bOK = factory->init(_gui);
    ASSERT(bOK); bOK;

    return factory;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
JuceControlFactory::JuceControlFactory() :
    mInit(false)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool JuceControlFactory::init(GUI* _gui)
{
    bool bOK = true;

    if (!isOk())
    {
        mGUI = _gui;

/*        mShinyLook = new ShinyLookAndFeel();
        LookAndFeel::setDefaultLookAndFeel(mShinyLook);*/

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void JuceControlFactory::done()
{
    if (isOk())
    {
        //delete mShinyLook;
        mInit = false;
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GUIControl* JuceControlFactory::create(StrId _type, const char* _name, GUIControl* _parent)
{
    GUIControl* control = 0;

    /*CREATE_JUCEFACTORY_CONTROL_INSTANCE(JuceWindow);
    CREATE_JUCEFACTORY_CONTROL_INSTANCE(JuceButton);
    CREATE_JUCEFACTORY_CONTROL_INSTANCE(JuceForm);
    CREATE_JUCEFACTORY_CONTROL_INSTANCE(JuceGroupBox);
    //CREATE_JUCEFACTORY_CONTROL_INSTANCE(JuceTabControl);
    CREATE_JUCEFACTORY_CONTROL_INSTANCE(JuceComboBox);
    CREATE_JUCEFACTORY_CONTROL_INSTANCE(JuceSlider);
    CREATE_JUCEFACTORY_CONTROL_INSTANCE(JuceLabel);*/

    return control;
}
