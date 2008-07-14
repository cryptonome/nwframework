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

#include "Win32Window.h"
#include "WindowMenu.h"
#include "NWCriticalSection.h"
#include "ICAssert.h"

static int g_Windows = 0;
static ControlFactory* g_ControlFactory = 0;
static const int APP_MSG_ID = WM_APP+45;

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
LRESULT win32WindowProc(HWND h, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int win32WindowProcReentry = 0;
    if ( win32WindowProcReentry == 0 )
        assertSetShowMessageBox(false);
    win32WindowProcReentry++;

    Win32ControlFactory* controlFactory = (Win32ControlFactory*)g_ControlFactory;
    if (controlFactory != 0)
    {
        Win32ControlData* win32Control = controlFactory->getWin32Control(h);
        if ( win32Control != 0 )
        {
            if ( win32Control->sendMsg(message,wParam,lParam) )
                return 0;
        }
    }

    switch(message)
    {
        // Notify (used by common controls)
        case WM_NOTIFY:
            {
                LPNMHDR hdr = (LPNMHDR)lParam;
                HWND hWnd = hdr->hwndFrom;
                if (hWnd != 0 && controlFactory != 0)
                {
                    Win32ControlData* win32Control = controlFactory->getWin32Control(hWnd);
                    if ( win32Control->sendMsg(message,wParam,lParam) )
                        return 0;
                }
            }
            break;
        // Commands (used by standard controls)
        case WM_COMMAND:
            {
                HWND hWnd = (HWND)lParam;
                if (hWnd != 0 && controlFactory != 0)
                {
                    Win32ControlData* win32Control = controlFactory->getWin32Control(hWnd);
                    if ( win32Control && win32Control->sendMsg(message,wParam,lParam) )
                        return 0;
                }
            }
            break;
        /*case WM_KEYDOWN:
        case WM_KEYUP:
            {
                Win32ControlData* win32Control = controlFactory->getWin32Control(h);
                if ( win32Control != 0 && win32Control->sendMsg(message,wParam,lParam) )
                        return 0;
            }
            break;*/
    }

    win32WindowProcReentry--;
    if ( win32WindowProcReentry == 0 )
        assertSetShowMessageBox(true);

    return DefWindowProc (h, message, wParam, lParam);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Win32Window::Win32Window() : Inherited(),
    mhMenu(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32Window::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mMenu = 0;
        bOK = createWindow(_name, _parent);

        if ( bOK )
            bOK = Inherited::init(_gui, _name, _parent);

        ASSERT(g_ControlFactory == 0 || g_ControlFactory == _gui->getControlFactory());
        g_ControlFactory = _gui->getControlFactory();
        g_Windows++;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32Window::done()
{
    if (isOk())
    {
        g_Windows--;
        if ( g_Windows == 0 )
            g_ControlFactory = 0;
        Inherited::done();

        if ( mhMenu )
            DestroyMenu(mhMenu);
        DISPOSE(mMenu);
    }
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32Window::createWindow(const char* _name, GUIControl* _parent)
{
    bool ok = true;

    ASSERT(mhWnd == 0);

    const char windowClassName[] = "win32WindowClassName";

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
    wcex.hCursor        = LoadCursor(wcex.hInstance, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = 0;
    RegisterClassEx(&wcex);

    DWORD exstyle = 0;
    DWORD type = 0;

    //type |= WS_OVERLAPPED;
    exstyle |= WS_EX_APPWINDOW;

    // Close button
    type |= WS_SYSMENU;

    // Resizable
    type |= WS_THICKFRAME;

    // Minimize button
    type |= WS_MINIMIZEBOX;

    // Maximize button
    type |= WS_MAXIMIZEBOX;

    HWND hParent = 0;
    if ( _parent != 0)
    {
        Win32ControlData* win32ControlData = Win32Control<::Window>::getWin32ControlDataFromControl(_parent);
        hParent = win32ControlData->getHWnd();
    }

    mhMenu = CreateMenu();

    mhWnd = CreateWindowEx(exstyle, windowClassName, _name, type, 0, 0, 0, 0, hParent, mhMenu, wcex.hInstance, 0);

    if (mhWnd != 0)
    {
        //ShowWindow(mhWnd,SW_SHOW);
        UpdateWindow(mhWnd);
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
void Win32Window::msgClose()
{
    sendEventCloseButtonPressed();
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32Window::sendMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
    bool processed = false;

    switch(message)
    {
        // Close window button
        case WM_CLOSE:
            {
                msgClose();
                processed = true;
            }
            break;
        case WM_COMMAND:
            {
                // Menu item
                if ( lParam == 0 )
                {
                    WindowMenu* item = mMenu->getItemByNumId((int)wParam);
                    if ( item != 0 )
                    {
                        processed = true;
                        if ( mWindowEventHandler != 0 )
                            mWindowEventHandler->windowMenuItemSelected(item);
                    }
                }
            }
            break;
        // Application messages (used to processs user messages)
        case APP_MSG_ID:
            {
                ASSERT(wParam == 0);

                bool send = false;
                mcsUserMessages->enter();
                UserMessage* um = getUserMessage((int)lParam);
                ASSERT(um);
                if ( um && um->mPending )
                {
                    um->mPending = false;
                    send = true;
                }
                mcsUserMessages->leave();

                if ( mWindowEventHandler && send )
                    mWindowEventHandler->windowUserMsg(um->mId);

                processed = true;
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
void Win32Window::setMenu(WindowMenu* _menu)
{
    ASSERT(mMenu == 0);

    mMenu = _menu;
    int globalindex = 0;
    addMenuItem(_menu,mhMenu,0,globalindex,false);

    // TODO
    // Force the menu update, now using a hack
    /*ShowWindow(mhWnd,SW_HIDE);
    ShowWindow(mhWnd,SW_SHOW);*/
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32Window::addMenuItem(WindowMenu* _item, HMENU _hMenu, int _index, int& _globalindex_, bool _addOwn)
{
    HMENU hMenu = _hMenu;

    int itemNumId = _globalindex_++;
    _item->setNumId(itemNumId);

    if ( _addOwn )
    {
        MENUITEMINFO mif;
        memset(&mif,0,sizeof(mif));
        mif.cbSize = sizeof(mif);
        mif.fMask = MIIM_STRING | MIIM_ID;
        mif.fType = MFS_DEFAULT;
        mif.fState = MFS_DEFAULT;
        mif.wID = itemNumId;
        mif.dwTypeData = (LPSTR)_item->getName();
        mif.cch = (int)strlen(_item->getName());

        if ( _item->getNumItems() > 0 )
        {
            mif.fMask |= MIIM_SUBMENU;
            mif.hSubMenu = CreateMenu();
            mif.fType = MFT_MENUBREAK;
            hMenu = mif.hSubMenu;
        }
        BOOL ret = InsertMenuItem(_hMenu,_index,FALSE,&mif);
        ASSERT(ret != 0);
    }

    for ( int i = 0 ; i < _item->getNumItems() ; ++i )
    {
        int index = _addOwn ? i : _item->getNumItems()-1-i;
        WindowMenu* item = _item->getItem(index);
        addMenuItem(item,hMenu,i,_globalindex_,true);
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32Window::sendUserMsg(int _id)
{
    mcsUserMessages->enter();

    UserMessage* um = getUserMessage(_id);
    ASSERT(um);

    if ( um && !um->mPending )
    {
        um->mPending = true;
        PostMessage(mhWnd, APP_MSG_ID, 0, um->mId);
    }

    mcsUserMessages->leave();
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32Window::setSize(int _width, int _height)
{
    int borderWidth = 0;
    int borderHeight = 0;
    getWindowBorderSize(borderWidth, borderHeight);

    _width += borderWidth;
    _height += borderHeight;

    SetWindowPos (mhWnd, 0, 0, 0, _width, _height, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32Window::getSize(int& _width, int& _height) const
{
    Inherited::getSize(_width,_height);

    int borderWidth = 0;
    int borderHeight = 0;
    getWindowBorderSize(borderWidth, borderHeight);

    _width = MAX(_width-borderWidth,0);
    _height = MAX(_height-borderHeight,0);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32Window::getWindowBorderSize(int& width_, int& height_) const
{
    WINDOWINFO wi;
    memset(&wi,0,sizeof(wi));
    wi.cbSize = sizeof(wi);
    GetWindowInfo(mhWnd,&wi);

    width_ = (wi.rcClient.left - wi.rcWindow.left) + (wi.rcWindow.right - wi.rcClient.right);
    height_ = (wi.rcClient.top - wi.rcWindow.top) + (wi.rcWindow.bottom - wi.rcClient.bottom);

    width_ = MAX(width_,0);
    height_ = MAX(height_,0);
}

