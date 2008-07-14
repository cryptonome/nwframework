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
*
*      Permission is hereby granted, free of charge, to any person obtaining
*      a copy of this software and associated documentation files (the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*      distribute, sublicense, and/or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions:
*
*      The above copyright notice and this permission notice shall be
*      included in all copies or substantial portions of the Software.
*
*      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
*      LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
*      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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



