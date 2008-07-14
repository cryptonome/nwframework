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

#include "Win32Form.h"

LRESULT win32WindowProc(HWND h, UINT message, WPARAM wParam, LPARAM lParam);

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Win32Form::Win32Form() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32Form::init(GUI* _gui, const char* _name, GUIControl* _parent)
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
void Win32Form::done()
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
bool Win32Form::createWindow(GUIControl* _parent)
{
    bool ok = true;

    ASSERT(mhWnd == 0);

    const char windowClassName[] = "win32FormClassName";

    WNDCLASSEX wcex;
    memset(&wcex,0,sizeof(wcex));
    wcex.cbSize         = sizeof (wcex);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = (WNDPROC)win32WindowProc;
    wcex.lpszClassName  = windowClassName;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 32;
    wcex.hInstance      = GetModuleHandle(NULL);
    wcex.hIcon          = NULL;
    wcex.hIconSm        = NULL;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNSHADOW);
    wcex.lpszMenuName   = 0;
    RegisterClassEx(&wcex);

    DWORD exstyle = 0;
    DWORD type = 0;

    HWND hParent = 0;
    type |= WS_CHILD;
    if ( _parent != 0)
    {
        type |= WS_CHILD;
        Win32ControlData* win32ControlData = Win32Control<FormControl>::getWin32ControlDataFromControl(_parent);
        hParent = win32ControlData->getHWnd();
    }
    else
    {
        type |= WS_POPUP;
    }

    mhWnd = CreateWindowEx(exstyle, windowClassName, "", type, 0, 0, 0, 0, hParent, 0, 0, 0);

    if (mhWnd != 0)
    {    
        ShowWindow(mhWnd,SW_SHOW);
        UpdateWindow(mhWnd);
    }
    else
    {
        ok = false;
        ASSERT(false);
    }

    return ok;
}
