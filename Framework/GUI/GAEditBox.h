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
#ifndef GAEDITBOX_H_
#define GAEDITBOX_H_

#include "GUIAdapterCreator.h"
#include "EditBox.h"
#include "NWIP.h"

//********************************************************************
//
//********************************************************************
class GAEditBoxString : public GUIAdapter<::EditBox,std::string>, public EditBoxEventHandler
{
    virtual bool initAdapter();
    virtual void editBoxModified(::EditBox* _editBox);
    virtual void update(const char * _contextName, const char * _objName);
};

//********************************************************************
//
//********************************************************************
class GAEditBoxInt : public GUIAdapter<::EditBox,int>, public EditBoxEventHandler
{
    virtual bool initAdapter();
    virtual void editBoxModified(::EditBox* _editBox);
    virtual void update(const char * _contextName, const char * _objName);
};

//********************************************************************
//
//********************************************************************
class GAEditBoxNWIP : public GUIAdapter<::EditBox,NWIP>, public EditBoxEventHandler
{
    virtual bool initAdapter();
    virtual void editBoxModified(::EditBox* _editBox);
    virtual void update(const char * _contextName, const char * _objName);
};


#endif
