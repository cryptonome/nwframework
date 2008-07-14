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

#include "CheckBox.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
CheckBox::CheckBox() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool CheckBox::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mCheckBoxEventHandler = 0;

        bOK = Inherited::init(_gui, _name, _parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CheckBox::done()
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
StrId CheckBox::getControlType() const
{ 
    return staticGetControlType(); 
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
StrId CheckBox::staticGetControlType() 
{ 
    return StrId("CheckBox");
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool CheckBox::load(const DataScript& _data)
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
void CheckBox::save(DataScript& _data)
{
    Inherited::save(_data);

    _data.setString("TEXT", getText());
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CheckBox::sendEventCheckBoxPressed()
{
    if ( mCheckBoxEventHandler )
        mCheckBoxEventHandler->checkBoxPressed(this);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CheckBox::sendEventCheckBoxModified()
{
    if ( mCheckBoxEventHandler )
        mCheckBoxEventHandler->checkBoxModified(this);
}

