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
#ifndef GACHECKBOX_H_
#define GACHECKBOX_H_

#include "GUIAdapterCreator.h"
#include "CheckBox.h"

//********************************************************************
//
//********************************************************************
class GACheckBoxBool : public GUIAdapter<::CheckBox,bool>, public CheckBoxEventHandler
{
    virtual bool initAdapter();
    virtual void checkBoxModified(::CheckBox* _checkbox);
    virtual void update(const char * _contextName, const char * _objName);
};


//********************************************************************
//
//********************************************************************
class GACheckBoxString : public GUIAdapter<::CheckBox,std::string>, public CheckBoxEventHandler
{
    virtual bool initAdapter();
    virtual void checkBoxModified(::CheckBox* _checkbox);
    virtual void update(const char * _contextName, const char * _objName);
};


#endif
