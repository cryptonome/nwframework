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
#include "Win32CheckBox.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Win32CheckBox::Win32CheckBox() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32CheckBox::init(GUI* _gui, const char* _name, GUIControl* _parent)
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
void Win32CheckBox::done()
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
bool Win32CheckBox::createWindow(GUIControl* _parent)
{
    bool ok = true;

    ASSERT(mhWnd == 0);

    HWND hParent = 0;
    if ( _parent != 0)
    {
        Win32ControlData* win32ControlData = Win32Control<::CheckBox>::getWin32ControlDataFromControl(_parent);
        hParent = win32ControlData->getHWnd();
    }

    mhWnd = CreateWindow("button", "checkbox", WS_CHILD|WS_VISIBLE|BS_CHECKBOX|WS_TABSTOP,0,0,0,0,hParent,0,GetModuleHandle(NULL),NULL);

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
void Win32CheckBox::setChecked(bool _checked, bool _sendMofifiedEvent)
{
    bool checked = getChecked();
    if ( _checked != checked )
    {
        SendMessage(mhWnd,BM_SETCHECK,_checked ? BST_CHECKED : BST_UNCHECKED,0);
        if ( _sendMofifiedEvent )
            sendEventCheckBoxModified();
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32CheckBox::getChecked() const
{
    bool checked = SendMessage(mhWnd,BM_GETCHECK,0,0) == BST_CHECKED;

    return checked;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32CheckBox::sendMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
    bool processed = false;

    switch(message)
    {
        case WM_COMMAND:
            {
                // CheckBox pressed
                if ( HIWORD(wParam) == BN_CLICKED )
                {
                    // Change state
                    bool checked = getChecked();
                    setChecked(!checked);

                    // Send event
                    sendEventCheckBoxPressed();
                }
            }
            break;
    }
    
    return processed;
}
