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
#ifndef GATABCONTROL_H_
#define GATABCONTROL_H_

#include "GUIAdapterCreator.h"
#include "TabControl.h"

//********************************************************************
//
//********************************************************************
class GATabControlString : public GUIAdapter<::TabControl,std::string>, public TabControlEventHandler
{
    virtual bool initAdapter();
    virtual void tabControlModified(::TabControl* _tabControl);
    virtual void update(const char * _contextName, const char * _objName);
};

#endif
