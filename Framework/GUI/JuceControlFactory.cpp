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
