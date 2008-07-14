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
#ifndef GACOMBOBOX_H_
#define GACOMBOBOX_H_

#include "GUIAdapterCreator.h"
#include "ComboBox.h"
#include "NWItemsList.h"
#include <string>

//********************************************************************
//
//********************************************************************
class GAComboBoxInt : public GUIAdapter<::ComboBox,int>, public ComboBoxEventHandler
{
    ~GAComboBoxInt();

    virtual bool initAdapter();
    virtual void comboBoxSelectionChanged(::ComboBox* _comboBox);
    virtual void update(const char * _contextName, const char * _objName);

    NWSvcDataObj<NWItemsList<int>> mDataList;
private:
    void updateSelection();
};


//********************************************************************
//
//********************************************************************
class GAComboBoxString : public GUIAdapter<::ComboBox,std::string>, public ComboBoxEventHandler
{
    ~GAComboBoxString();

    virtual bool initAdapter();
    virtual void comboBoxSelectionChanged(::ComboBox* _comboBox);
    virtual void update(const char * _contextName, const char * _objName);

    NWSvcDataObj<NWItemsList<std::string>> mDataList;
private:
    void updateSelection();
};

#endif