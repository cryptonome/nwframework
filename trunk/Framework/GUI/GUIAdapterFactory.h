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
#ifndef GUIADAPTERFACTORY_H_
#define GUIADAPTERFACTORY_H_

#include "StrId.h"
#include <list>

class IGUIAdapterCreator;
class GUIControl;
class IGUIAdapter;
class GUI;

//********************************************************************
//
//********************************************************************
class GUIAdapterFactory
{
public:
    GUIAdapterFactory  ();
    virtual    ~GUIAdapterFactory ()                      { GUIAdapterFactory::done(); }

    virtual bool          init                (GUI * _gui);
    bool                  isOk                () const  { return mInit; }
    virtual void          done                ();

    void registerAdapterCreator(IGUIAdapterCreator* _creator);
    IGUIAdapter* createAdapter(GUIControl* _control, StrId _dataTypeString, StrId _dataObjContext, StrId _dataObjName);

private:
    void registerBaseGUIAdapterCreators();

    bool mInit : 1;
    GUI * mGUI;

    std::list<IGUIAdapterCreator*> mCreators;
};


#endif
