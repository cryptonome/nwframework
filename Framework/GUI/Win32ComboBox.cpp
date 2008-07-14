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

#include "Win32ComboBox.h"


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Win32ComboBox::Win32ComboBox() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32ComboBox::init(GUI* _gui, const char* _name, GUIControl* _parent)
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
void Win32ComboBox::done()
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
bool Win32ComboBox::createWindow(GUIControl* _parent)
{
    bool ok = true;

    ASSERT(mhWnd == 0);

    HWND hParent = 0;
    if ( _parent != 0)
    {
        Win32ControlData* win32ControlData = Win32Control<::ComboBox>::getWin32ControlDataFromControl(_parent);
        hParent = win32ControlData->getHWnd();
    }

    mhWnd = CreateWindow("ComboBox", "ComboBox", WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST,0,0,0,0,hParent,0,GetModuleHandle(NULL),NULL);

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
void Win32ComboBox::selectItem(int _index)
{
    ASSERT(_index >= -1 && _index < getNumItems());
    SendMessage(mhWnd, CB_SETCURSEL, _index, 0);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int Win32ComboBox::getSelectedItem() const
{
    LRESULT res = SendMessage(mhWnd, CB_GETCURSEL, 0, 0);

    int index = -1;
    if ( res != CB_ERR )
        index = (int)res;

    return index;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32ComboBox::internalAddItem(const char* _text)
{
    // Add the item at the end and select it
    SendMessage(mhWnd, CB_INSERTSTRING, -1, (LPARAM)_text);
    SendMessage(mhWnd, CB_SETCURSEL, getNumItems(), 0);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Win32ComboBox::internalClearItemsList()
{
    SendMessage(mhWnd, CB_RESETCONTENT, 0 , 0);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Win32ComboBox::sendMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
    bool processed = false;

    switch(message)
    {
        case WM_COMMAND:
            {
                // Selection changed
                if ( HIWORD(wParam) == CBN_SELCHANGE )
                {
                    // Send event
                    sendEventComboBoxSelectionChanged();
                }
                int a = 0;
            }
            break;
    }
    
    return processed;
}
