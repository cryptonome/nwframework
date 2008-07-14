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
#include "GAEditBox.h"
#include "StrUtils.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GAEditBoxString::initAdapter()
{
    mControl->setEditBoxEventHandler(this);
    mControl->setText("", false);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAEditBoxString::editBoxModified(::EditBox* _editBox)
{ 
    ASSERT(mControl == _editBox);

    mData.set(_editBox->getText());
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAEditBoxString::update(const char * _contextName, const char * _objName)
{
    mControl->setText(mData.get().c_str(), false);
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GAEditBoxInt::initAdapter()
{
    mControl->setEditBoxEventHandler(this);
    mControl->setText("", false);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAEditBoxInt::editBoxModified(::EditBox* _editBox)
{ 
    ASSERT(mControl == _editBox);

    int val = StrUtils::aToI(_editBox->getText());
    mData.set(val);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAEditBoxInt::update(const char * _contextName, const char * _objName)
{
    std::string str = StrUtils::iToA(mData.get());
    mControl->setText(str.c_str(), false);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GAEditBoxNWIP::initAdapter()
{
    mControl->setEditBoxEventHandler(this);
    mControl->setText("1.2.3.4", false);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAEditBoxNWIP::editBoxModified(::EditBox* _editBox)
{ 
    ASSERT(mControl == _editBox);

    NWIP ip = mData.get();
    ip.setFromStr(_editBox->getText());
    mData.set(ip);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAEditBoxNWIP::update(const char * _contextName, const char * _objName)
{
    NWIP ip = mData.get();
    mControl->setText(ip.getAsStr().c_str(), false);
}
