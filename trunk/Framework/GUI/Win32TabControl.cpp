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

#include "Win32TabControl.h"
#include <commctrl.h>
#include "GUI.h"
#include "ControlFactory.h"
#include "FormControl.h"
#include "Form.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Win32TabControl::Win32TabControl() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32TabControl::init(GUI* _gui, const char* _name, GUIControl* _parent)
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
void Win32TabControl::done()
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
bool Win32TabControl::createWindow(GUIControl* _parent)
{
    bool ok = true;

    ASSERT(mhWnd == 0);

    HWND hParent = 0;
    if ( _parent != 0)
    {
        Win32ControlData* win32ControlData = Win32Control<::TabControl>::getWin32ControlDataFromControl(_parent);
        hParent = win32ControlData->getHWnd();
    }

    mhWnd = CreateWindow(WC_TABCONTROL, "", WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_TABSTOP,0,0,0,0,hParent,0,GetModuleHandle(NULL),NULL);

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
void Win32TabControl::setActiveTab(Tab* _tab)
{
    int index = _tab->getIndex();
    TabCtrl_SetCurFocus(mhWnd,index);
    Inherited::setActiveTab(_tab);

    sendEventTabControlModified();
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Tab* Win32TabControl::getActiveTab()
{
    int index = TabCtrl_GetCurFocus(mhWnd);
    return getTab(index);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Tab* Win32TabControl::createTab(const char* _name, Form* _form, int _index)
{
    TC_ITEM tie;
    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = -1;   
    tie.pszText = (LPSTR)_name;
    int ret = TabCtrl_InsertItem(mhWnd,_index,&tie);
    ASSERT(ret != -1);

    Tab* tab = NEW Tab();
    tab->init(this,_form,_index,_name);
    //addChild(_form);
    ASSERT(existChild(_form->getControl()));

    int x, y;
    getSize(x,y);
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = x;
    rect.bottom = y;
    TabCtrl_AdjustRect(mhWnd, FALSE, &rect);
    _form->setPosition(rect.left,rect.top);
    _form->setSize(rect.right-rect.left,rect.bottom-rect.top);

    return tab;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32TabControl::destroyTab(Tab* _tab)
{
    TabCtrl_DeleteItem(mhWnd,_tab->getIndex());
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32TabControl::setTabName(Tab* _tab, const char* _name)
{
    ASSERT(false); // TODO
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32TabControl::sendMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
    bool processed = false;

    switch(message)
    {
        case WM_NOTIFY:
            {
                // Tab selection changed
                LPNMHDR hdr = (LPNMHDR)lParam;
                if ( hdr->code == TCN_SELCHANGE )
                {
                    int index = TabCtrl_GetCurFocus(mhWnd);
                    Tab* tab = getTab(index);
                    setActiveTab(tab);
                }
            }
            break;
    }

    return processed;
}

