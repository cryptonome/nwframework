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

#include "juce.h"
#include <list>

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void JuceControl<T>::done()
{
    if ( isOk() )
    {
        juce::Component* juceComponent = getJuceComponent();
        std::list<GUIControl*> controls;
        int numChildren = getNumChilds();
        for ( int i = 0 ; i < numChildren ; ++i )
        {
            juce::Component* component = juceComponent->getChildComponent(i);
            void* userData = component->getUserData();
            if ( userData != 0 )
                controls.push_back(static_cast<GUIControl*>(userData));
        }
        for ( std::list<GUIControl*>::iterator it = controls.begin() ; it != controls.end() ; ++it )
        {
            GUIControl* guiControl = *it;
            DISPOSE(guiControl);
        }
        Inherited::done();
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void JuceControl<T>::addChild(GUIControl* _child)
{
    juce::Component* juceComponent = getJuceComponent();
    juce::Component* juceComponentChild = getJuceComponentFromControl(_child);

    juceComponent->addChildComponent(juceComponentChild);
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
int JuceControl<T>::getNumChilds() const
{
    juce::Component* juceComponent = getJuceComponent();
    return juceComponent->getNumChildComponents();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
GUIControl* JuceControl<T>::getChild(int _index) const
{
    juce::Component* juceComponent = getJuceComponent();
    juce::Component* child = juceComponent->getChildComponent(_index);
    GUIControl* control = (GUIControl*)(child->getUserData());
    return control;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
const char* JuceControl<T>::getName() const
{
    juce::Component* juceComponent = getJuceComponent();
    return juceComponent->getName();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void JuceControl<T>::setName(const char* _name)
{
    juce::Component* juceComponent = getJuceComponent();
    juceComponent->setName(_name);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void JuceControl<T>::setPosition (int _x, int _y)
{
    juce::Component* juceComponent = getJuceComponent();
    juceComponent->setTopLeftPosition(_x,_y);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void JuceControl<T>::getPosition (int& _x, int& _y) const
{
    juce::Component* juceComponent = getJuceComponent();
    _x = juceComponent->getScreenX();
    _y = juceComponent->getScreenY();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void JuceControl<T>::setSize (int _x, int _y)
{
    juce::Component* juceComponent = getJuceComponent();
    juceComponent->setSize(_x,_y);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void JuceControl<T>::getSize (int& _x, int& _y) const
{
    juce::Component* juceComponent = getJuceComponent();
    _x = juceComponent->getWidth();
    _y = juceComponent->getHeight();
}


