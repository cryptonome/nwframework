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
