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
#ifndef CONTROLFACTORY_H_
#define CONTROLFACTORY_H_

#include "StrId.h"

class GUI;
class GUIControl;

//********************************************************************
//
//********************************************************************
class ControlFactory
{
public:
    virtual    ~ControlFactory ()                      { }

    virtual GUIControl*   create  (StrId _type, const char* _name, GUIControl* _parent) = 0;
    template <typename T> T*   create  (const char* _name, GUIControl* _parent);

    static ControlFactory* createFactory(GUI* _gui);
    static void destroyFactory(ControlFactory* _factory);
};

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
T* ControlFactory::create(const char* _name, GUIControl* _parent)
{
    GUIControl* guiControl = create(T::staticGetControlType(),_name,_parent);
    return guiControl ? guiControl->convertTo<T>() : 0;
}


#endif
