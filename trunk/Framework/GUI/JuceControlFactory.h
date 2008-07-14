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
#ifndef JUCECONTROLFACTORY_H_
#define JUCECONTROLFACTORY_H_

#include "ControlFactory.h"

namespace juce
{
    class ShinyLookAndFeel;
};

//********************************************************************
//
//********************************************************************
class JuceControlFactory : public ControlFactory
{
public:
    JuceControlFactory  ();
    virtual    ~JuceControlFactory ()                      { JuceControlFactory::done(); }

    virtual bool          init                 (GUI* _gui);
    bool                  isOk                 () const  { return mInit; }
    virtual void          done                 ();

    // ControlFactory
    virtual GUIControl*   create  (StrId _type, const char* _name, GUIControl* _parent);

    static ControlFactory* createFactory(GUI* _gui);

private:
    bool          mInit : 1;
    GUI* mGUI;
    juce::ShinyLookAndFeel* mShinyLook;
};

#endif
