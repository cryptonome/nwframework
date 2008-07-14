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
*
*      Permission is hereby granted, free of charge, to any person obtaining
*      a copy of this software and associated documentation files (the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*      distribute, sublicense, and/or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions:
*
*      The above copyright notice and this permission notice shall be
*      included in all copies or substantial portions of the Software.
*
*      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
*      LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
*      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
