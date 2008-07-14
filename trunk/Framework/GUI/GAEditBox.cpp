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
