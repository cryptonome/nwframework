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
#ifndef JUCECONTROL_H_
#define JUCECONTROL_H_

#include "GUIControl.h"

namespace juce
{
    class Component;
};

//********************************************************************
//
//********************************************************************
class JuceControlData
{
public:
    JuceControlData () : mComponent(0) {}
    virtual ~JuceControlData() { }
    void setJuceComponent(GUIControl* _control, juce::Component* _component);
    juce::Component* getJuceComponent() const { ASSERT(mComponent != 0); return mComponent; };
private:
    juce::Component* mComponent;
};

//********************************************************************
//
//********************************************************************
template <typename T>
class JuceControl : public T, public JuceControlData
{
public:
    virtual    ~JuceControl () { JuceControl::done(); }

    virtual void done();

    // GUIControl
    virtual void addChild(GUIControl* _child);
    virtual int getNumChilds() const;
    virtual GUIControl* getChild(int _index) const;

    virtual const char* getName() const;
    virtual void setName(const char* _name);

    virtual void setPosition(int _x, int _y);
    virtual void getPosition(int& _x, int& _y) const;
    virtual void setSize(int _width, int _height);
    virtual void getSize(int& _width, int& _height) const;

protected:
    typedef T Inherited;
    static juce::Component* getJuceComponentFromControl(GUIControl* _control) { return dynamic_cast<JuceControlData*>(_control->getControl())->getJuceComponent(); }
};

#include "JuceControl.hpp"

#endif
