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

#include "Win32EditBox.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Win32EditBox::Win32EditBox() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32EditBox::init(GUI* _gui, const char* _name, GUIControl* _parent)
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
void Win32EditBox::done()
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
bool Win32EditBox::createWindow(GUIControl* _parent)
{
    bool ok = true;

    ASSERT(mhWnd == 0);

    HWND hParent = 0;
    if ( _parent != 0)
    {
        Win32ControlData* win32ControlData = Win32Control::getWin32ControlDataFromControl(_parent);
        hParent = win32ControlData->getHWnd();
    }

    mhWnd = CreateWindow("edit", "edit", WS_CHILD|WS_VISIBLE|ES_LEFT|WS_BORDER|WS_TABSTOP,0,0,0,0,hParent,0,GetModuleHandle(NULL),NULL);

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
bool Win32EditBox::sendMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
    bool processed = false;

    switch(message)
    {
        /*case WM_KEYDOWN:
            {
                // Enter pressed
                if ( wParam == VK_RETURN )
                {
                    sendEventEditBoxModified();
                }
            }
            break;*/
        case WM_COMMAND:
            {
                // CheckBox pressed
                if ( HIWORD(wParam) == EN_KILLFOCUS )
                {
                    // Send event
                    sendEventEditBoxModified();
                }
                int a = 0;
            }
            break;
        default:
            {
                int a = 0;
            }
            break;
    }
    
    return processed;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32EditBox::setText(const char* _text, bool _sendMofifiedEvent)
{ 
    win32SetText(_text);

    if ( _sendMofifiedEvent )
        sendEventEditBoxModified();
}
