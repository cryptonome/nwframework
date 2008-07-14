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
#include "Win32Button.h"


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Win32Button::Win32Button() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32Button::init(GUI* _gui, const char* _name, GUIControl* _parent)
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
void Win32Button::done()
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
bool Win32Button::createWindow(GUIControl* _parent)
{
    bool ok = true;

    ASSERT(mhWnd == 0);

    HWND hParent = 0;
    if ( _parent != 0)
    {
        Win32ControlData* win32ControlData = Win32Control<::Button>::getWin32ControlDataFromControl(_parent);
        hParent = win32ControlData->getHWnd();
    }

    mhWnd = CreateWindow("button", "Push Button", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,0,0,0,0,hParent,0,GetModuleHandle(NULL),NULL);

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

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32Button::sendMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
    bool processed = false;

    switch(message)
    {
        // Close window button
        case WM_COMMAND:
            {
                if ( HIWORD(wParam) == BN_CLICKED )
                    sendEventButtonPressed();
            }
            break;
    }
    
    return processed;
}
