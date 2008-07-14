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

#include "WindowMenu.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
WindowMenu::WindowMenu() :
    mInit(false),
    mNumId(-1)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool WindowMenu::init()
{
    return init("main menu","");
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool WindowMenu::init(const char* _name, const char* _id)
{
    bool bOK = true;

    if (!isOk())
    {
        mName = _name;
        mId = _id;
        mNumId = -1;

        mInit = true;
    }
    return bOK;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void WindowMenu::done()
{
    if (isOk())
    {
        int items = getNumItems();
        for ( int i = 0 ; i < items ; ++i )
        {
            WindowMenu* item = getItem(i);
            DISPOSE(item);
        }
        mItems.clear();
        mInit = false;
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
WindowMenu* WindowMenu::addItem(const char* _name, const char* _id)
{
    WindowMenu* item = NEW WindowMenu();
    item->init(_name, _id);
    
    mItems.push_back(item);

    return item;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
WindowMenu* WindowMenu::getItem(int _index)
{
    WindowMenu* item = 0;

    ASSERT(_index >= 0 && _index < getNumItems());

    item = mItems[_index];

    return item;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
WindowMenu* WindowMenu::getItemByNumId(int _numId)
{
    WindowMenu* item = 0;

    if ( mNumId == _numId )
        item = this;

    if ( item == 0 )
    {
        int items = getNumItems();
        for ( int i = 0 ; item == 0 && i < items ; ++ i )
        {
            item = getItem(i)->getItemByNumId(_numId);
        }
    }

    return item;
}