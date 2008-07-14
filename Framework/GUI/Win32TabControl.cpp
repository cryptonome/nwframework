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

