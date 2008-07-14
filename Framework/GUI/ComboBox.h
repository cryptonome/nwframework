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
