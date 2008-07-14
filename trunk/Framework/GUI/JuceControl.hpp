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


