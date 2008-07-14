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
#ifndef JUCESLIDER_H_
#define JUCESLIDER_H_

#include "Slider.h"
#include "JuceControl.h"

class JuceSliderImp;

//********************************************************************
//
//********************************************************************
class JuceSlider : public JuceControl<::Slider>
{
public:
    JuceSlider ();
    virtual    ~JuceSlider()                      { JuceSlider::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

private:
    typedef JuceControl<Slider> Inherited;

    JuceSliderImp* mJuceSlider;
};

#endif
