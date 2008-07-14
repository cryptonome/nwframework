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
#ifndef COMBOBOX_H_
#define COMBOBOX_H_

#include "GUIControl.h"
#include <vector>

class ComboBox;

//********************************************************************
//
//********************************************************************
class ComboBoxEventHandler
{
public:
    virtual ~ComboBoxEventHandler() {}

    virtual void comboBoxSelectionChanged(::ComboBox* _comboBox) { }
};

//********************************************************************
//
//********************************************************************
class ComboBox : public GUIControl
{
public:
    ComboBox     ();
    virtual    ~ComboBox    ()                      { ComboBox::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    int addItem(const char* _string, void* _userData);
    int getNumItems() const;
    void clearItemsList();
    void getItemData(int _index, std::string& string_, void*& userData_);
    int getItemIndexFromUserData(void* _userData);

    virtual void selectItem(int _index) = 0;
    virtual int getSelectedItem() const = 0;

    void setComboBoxEventHandler(ComboBoxEventHandler* _eventHandler) { mComboBoxEventHandler = _eventHandler; }

    virtual StrId getControlType() const;
    static StrId staticGetControlType();

protected:
    // Events
    void sendEventComboBoxSelectionChanged();

private:
    typedef GUIControl Inherited;

    virtual void internalAddItem(const char* _text) = 0;
    virtual void internalClearItemsList() = 0;

    struct Item
    {
        std::string mString;
        void* mUserData;
    };
    std::vector<Item*> mItems;
    ComboBoxEventHandler* mComboBoxEventHandler;
};

#endif
