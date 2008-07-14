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

#include "GroupBox.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GroupBox::GroupBox() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GroupBox::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {

        bOK = Inherited::init(_gui, _name, _parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GroupBox::done()
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
StrId GroupBox::getControlType() const
{ 
    return staticGetControlType(); 
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
StrId GroupBox::staticGetControlType() 
{ 
    return StrId("GroupBox");
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GroupBox::load(const DataScript& _data)
{
    bool bOK = Inherited::load(_data);

    DataScript& dsControl = (DataScript&)_data;

    std::string text = dsControl.getString("TEXT");
    setText(text.c_str());

    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GroupBox::save(DataScript& _data)
{
    Inherited::save(_data);

    _data.setString("TEXT", getText());
}
