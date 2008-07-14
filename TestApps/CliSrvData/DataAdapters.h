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
#ifndef DATA_ADAPTERS_H
#define DATA_ADAPTERS_H

#include "NWSvcDataObject.h"

class CheckBox;

class AdapterBoolCheckBox : public NWSvcDataObjNotificationCallback
{
public:
    AdapterBoolCheckBox();
    ~AdapterBoolCheckBox();

    bool init(NWSvcDataObj<bool> * _boolVar, CheckBox * _checkBox);
    void shutdown();

    virtual void update(const char * _contextName, const char * _objName);

private:
    bool mInitd;
    NWSvcDataObj<bool> * mBoolVar;
    CheckBox * mCheckBox;
};


#endif // DATA_ADAPTERS_H