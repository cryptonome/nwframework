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

#include "JuceControl.h"
#include "juce.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
/*static void setChildrenUserDataToNULL(juce::Component* _component)
{
    int numChilds = _component->getNumChildComponents();
    for ( int i = 0 ; i < numChilds ; ++i )
    {
        juce::Component* child = _component->getChildComponent(i);
        child->setUserData(0);
        setChildrenUserDataToNULL(child);
    }
}*/

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void JuceControlData::setJuceComponent(GUIControl* _control, juce::Component* _component)
{ 
    mComponent = _component;
    _component->setUserData(_control);
    //setChildrenUserDataToNULL(_component);
}
