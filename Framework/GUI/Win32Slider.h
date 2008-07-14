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
#ifndef WIN32SLIDER_H_
#define WIN32SLIDER_H_

#include "Slider.h"
#include "Win32Control.h"

//********************************************************************
//
//********************************************************************
class Win32Slider : public Win32Control<::Slider>
{
public:
    Win32Slider ();
    virtual    ~Win32Slider()                      { Win32Slider::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

private:
    typedef Win32Control<::Slider> Inherited;

    bool createWindow(GUIControl* _parent);
};

#endif
