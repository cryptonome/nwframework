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

#include "Win32ControlFactory.h"
#include "Win32Window.h"
#include "Win32Form.h"
#include "Win32Button.h"
#include "Win32Label.h"
#include "Win32GroupBox.h"
#include "Win32TabControl.h"
#include "Win32Slider.h"
#include "Win32ComboBox.h"
#include "Win32CheckBox.h"
#include "Win32EditBox.h"
#include <commctrl.h>
#include "Win32Control.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define CREATE_WIN32FACTORY_CONTROL_INSTANCE(win32ClassType) \
    if ( control == 0 && _type == win32ClassType::staticGetControlType() ) \
    { \
        win32ClassType* win32Control = NEW (win32ClassType)(); \
        if ( win32Control->init(mGUI,_name,_parent) ) \
            control = win32Control; \
        else \
            DISPOSE(win32Control); \
    }

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
ControlFactory* Win32ControlFactory::createFactory(GUI* _gui)
{
    Win32ControlFactory* factory = NEW Win32ControlFactory();

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
Win32ControlFactory::Win32ControlFactory() :
    mInit(false)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32ControlFactory::init(GUI* _gui)
{
    bool bOK = true;

    if (!isOk())
    {
        mGUI = _gui;

        InitCommonControls();

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32ControlFactory::done()
{
    if (isOk())
    {

        mInit = false;
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GUIControl* Win32ControlFactory::create(StrId _type, const char* _name, GUIControl* _parent)
{
    GUIControl* control = 0;

    CREATE_WIN32FACTORY_CONTROL_INSTANCE(Win32Window);
    CREATE_WIN32FACTORY_CONTROL_INSTANCE(Win32Form);
    CREATE_WIN32FACTORY_CONTROL_INSTANCE(Win32Button);
    CREATE_WIN32FACTORY_CONTROL_INSTANCE(Win32Label);
    CREATE_WIN32FACTORY_CONTROL_INSTANCE(Win32GroupBox);
    CREATE_WIN32FACTORY_CONTROL_INSTANCE(Win32TabControl);
    CREATE_WIN32FACTORY_CONTROL_INSTANCE(Win32Slider);
    CREATE_WIN32FACTORY_CONTROL_INSTANCE(Win32ComboBox);
    CREATE_WIN32FACTORY_CONTROL_INSTANCE(Win32CheckBox);
    CREATE_WIN32FACTORY_CONTROL_INSTANCE(Win32EditBox);

    return control;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32ControlFactory::registerControl(GUIControl* _control)
{
    ControlElement ce;
    ce.mControl = _control;
    ce.mhWnd = Win32Window::getWin32ControlDataFromControl(_control)->getHWnd();

    ASSERT(ce.mhWnd != 0);
    ASSERT(getWin32Control(ce.mhWnd) == 0);

    mRegisteredControls.push_back(ce);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32ControlFactory::unregisterControl(GUIControl* _control)
{
    bool found = false;

    for ( std::list<ControlElement>::iterator it = mRegisteredControls.begin() ; !found && it != mRegisteredControls.end() ; )
    {
        ControlElement& ce = *it;
        if ( ce.mControl == _control )
        {
            it = mRegisteredControls.erase(it);
            found = true;
        }
        else
        {
            ++it;
        }

    }

    ASSERT(found);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Win32ControlData* Win32ControlFactory::getWin32Control(HWND hWnd)
{
    GUIControl* control = 0;

    for ( std::list<ControlElement>::iterator it = mRegisteredControls.begin() ; control == 0 && it != mRegisteredControls.end() ; ++it )
    {
        ControlElement& ce = *it;
        if ( ce.mhWnd == hWnd )
            control = ce.mControl;
    }

    return control ? Win32Window::getWin32ControlDataFromControl(control) : 0;
}



