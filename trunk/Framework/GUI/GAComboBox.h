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
