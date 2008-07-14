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
