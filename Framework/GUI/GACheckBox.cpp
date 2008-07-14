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

#include "GACheckBox.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GACheckBoxBool::initAdapter()
{
    mControl->setCheckBoxEventHandler(this);
    mControl->setChecked(mData.get(), false);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GACheckBoxBool::checkBoxModified(::CheckBox* _checkbox)
{ 
    ASSERT(mControl == _checkbox);

    bool checked = _checkbox->getChecked();
    mData.set(checked);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GACheckBoxBool::update(const char * _contextName, const char * _objName)
{
    mControl->setChecked(mData.get(), false);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GACheckBoxString::initAdapter()
{
    mControl->setCheckBoxEventHandler(this);
    bool checked = strcmp(mData.get().c_str(),mControl->getDataObjParams()) == 0;
    mControl->setChecked(checked, false);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GACheckBoxString::checkBoxModified(::CheckBox* _checkbox)
{ 
    ASSERT(mControl == _checkbox);

    bool checked = _checkbox->getChecked();
    if ( checked )
        mData.set(mControl->getDataObjParams());
    else
        mData.set(mData.get());
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GACheckBoxString::update(const char * _contextName, const char * _objName)
{
    bool checked = strcmp(mData.get().c_str(),mControl->getDataObjParams()) == 0;
    mControl->setChecked(checked, false);
}
