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

#include "Win32GroupBox.h"


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Win32GroupBox::Win32GroupBox() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32GroupBox::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        if ( bOK )
            bOK = createWindow(_parent);

        bOK = Inherited::init(_gui,_name,_parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32GroupBox::done()
{
    if (isOk())
    {

        Inherited::done();
    }
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32GroupBox::createWindow(GUIControl* _parent)
{
    bool ok = true;

    ASSERT(mhWnd == 0);

    HWND hParent = 0;
    if ( _parent != 0)
    {
        Win32ControlData* win32ControlData = Win32Control<::GroupBox>::getWin32ControlDataFromControl(_parent);
        hParent = win32ControlData->getHWnd();
    }

    mhWnd = CreateWindow("button", "GroupBox", WS_CHILD|WS_VISIBLE|BS_GROUPBOX,0,0,0,0,hParent,0,GetModuleHandle(NULL),NULL);

    if (mhWnd != 0)
    {      
        ShowWindow(mhWnd,SW_SHOW);
    }
    else
    {
        ok = false;
        ASSERT(false);
    }

    return ok;
}

