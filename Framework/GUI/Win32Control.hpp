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


#include "Win32ControlFactory.h"
#include "GUI.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
bool Win32Control<T>::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mName = _name;
        bOK = Inherited::init(_gui, _name, _parent);

        ((Win32ControlFactory*)_gui->getControlFactory())->registerControl(this);

        // Set default windows font type
        HGDIOBJ hFont = GetStockObject(DEFAULT_GUI_FONT);
        SendMessage(mhWnd, WM_SETFONT, (WPARAM)hFont, TRUE);
    }
    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void Win32Control<T>::done()
{
    if ( isOk() )
    {
        Inherited::done();
        ((Win32ControlFactory*)mGUI->getControlFactory())->unregisterControl(this);
        if ( mhWnd )
        {
            DestroyWindow(mhWnd);
            mhWnd = 0;
        }
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void Win32Control<T>::addChild(GUIControl* _child)
{
    Win32ControlData* win32Child = getWin32ControlDataFromControl(_child);

    GUIControl* child = win32Child->getGUIControl();

    ASSERT(!existChild(child));

    win32Child->setParent(this);

    mChilds.push_back(child);
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
int Win32Control<T>::getNumChilds() const
{
    return (int)mChilds.size();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
GUIControl* Win32Control<T>::getChild(int _index) const
{
    ASSERT(_index >= 0 && _index < (int)mChilds.size());
    return mChilds[_index];
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
const char* Win32Control<T>::getName() const
{
    return mName.c_str();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void Win32Control<T>::setName(const char* _name)
{
    mName = _name;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void Win32Control<T>::setPosition (int _x, int _y)
{
    SetWindowPos (mhWnd, 0,
                  _x,
                  _y,
                  0, 0,
                  SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING);

    int x2, y2;
    getPosition(x2,y2);
    ASSERT(_x == x2 && _y == y2);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void Win32Control<T>::getPosition (int& _x, int& _y) const
{
    RECT rect;
    GetWindowRect(mhWnd,&rect);
    _x = rect.left;
    _y = rect.top;

    HWND parent = GetParent(mhWnd);
    if ( parent != 0 )
    {
        WINDOWINFO wi;
        memset(&wi,0,sizeof(wi));
        wi.cbSize = sizeof(wi);
        GetWindowInfo(parent,&wi);

        _x -= wi.rcClient.left;
        _y -= wi.rcClient.top;
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void Win32Control<T>::setSize (int _x, int _y)
{
    SetWindowPos (mhWnd, 0, 0, 0,
                  _x,
                  _y,
                  SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING);


    int x2, y2;
    getSize(x2,y2);
    ASSERT(_x == x2 && _y == y2);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void Win32Control<T>::getSize (int& _x, int& _y) const
{
    RECT rect;
    GetWindowRect(mhWnd,&rect);
    _x = rect.right-rect.left;
    _y = rect.bottom-rect.top;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void Win32Control<T>::setVisible(bool _visible)
{
    if ( _visible ) 
        ShowWindow(mhWnd,SW_SHOW);
    else
        ShowWindow(mhWnd,SW_HIDE);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
bool Win32Control<T>::getVisible() const
{
    // TODO
    ASSERT(false);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void Win32Control<T>::setEnabled(bool _enabled)
{
    EnableWindow(mhWnd, _enabled);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
bool Win32Control<T>::getEnabled() const
{
    bool enabled = IsWindowEnabled(mhWnd) != 0;
    return enabled;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
bool Win32Control<T>::existChild(GUIControl* _control)
{
    bool exist = false;

    int childs = (int)mChilds.size();
    for ( int i = 0 ; i < childs ; ++i )
    {
        GUIControl* child = mChilds[i];
        if ( child == _control )
            exist = true;
    }

    return exist;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void Win32Control<T>::setParent(Win32ControlData* _control)
{
    HWND parentHWnd = _control->getHWnd();
    SetParent(mhWnd,parentHWnd);
    mParent = _control->getGUIControl();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
HWND Win32Control<T>::getHWnd()
{
    return mhWnd;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void Win32Control<T>::removeChild(GUIControl* _control)
{
    bool found = false;

    for ( std::vector<GUIControl*>::iterator it = mChilds.begin() ; !found && it != mChilds.end() ;  )
    {
        GUIControl* child = *it;
        if ( child == _control )
        {
            found = true;
            mChilds.erase(it);
        }
        else
        {
            ++it;
        }
    }

    ASSERT(found);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void Win32Control<T>::win32SetText(const char* _text)
{
    ::SetWindowText(mhWnd,_text);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
const char* Win32Control<T>::win32GetText() const
{
    static char s_temp[256];
    ::GetWindowText(mhWnd,s_temp,sizeof(s_temp));
    return s_temp;
}

