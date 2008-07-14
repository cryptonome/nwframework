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
