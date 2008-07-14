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


#ifndef TABCONTROL_H_
#define TABCONTROL_H_

#include "GUIControl.h"
#include <list>

class Form;
class TabControl;

class TabControl;

//********************************************************************
//
//********************************************************************
class TabControlEventHandler
{
public:
    virtual ~TabControlEventHandler() {}

    virtual void tabControlModified(::TabControl* _tabControl) { }
};

//********************************************************************
//
//********************************************************************
class Tab
{
public:
    Tab() : mInit(false) { }
    ~Tab() { done(); }

    virtual bool init(TabControl* _tabControl, Form* _form, int _index, const char* _name);
    bool isOk() const { return mInit; }
    virtual void done();

    void setActive();
    bool isActive() const;
    int getIndex() const { return mIndex; }
    Form* getForm() { return mForm; }

    void setName(const char* _name);
    const char* getName() { return mName.c_str(); }

private:
    friend class TabControl;
    void setIndex(int _index) { mIndex = _index; }

    bool mInit;
    int mIndex;
    Form* mForm;
    TabControl* mTabControl;
    std::string mName;
};

//********************************************************************
//
//********************************************************************
class TabControl : public GUIControl
{
public:
    TabControl     ();
    virtual    ~TabControl    ()                      { TabControl::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    Tab* addTab(const char* _name, Form* _form);
    void removeTab(Tab* _tab);
    virtual void setActiveTab(Tab* _tab);
    Tab* getTab(int _index);
    Tab* getTabByName(const char* _name);
    virtual Tab* getActiveTab() = 0;

    void setTabControlEventHandler(TabControlEventHandler* _eventHandler) { mTabControlEventHandler = _eventHandler; }

    virtual StrId getControlType() const;
    static StrId staticGetControlType();

protected:
    // Events
    void sendEventTabControlModified();

private:
    typedef GUIControl Inherited;
    friend class Tab;

    void setVisibleTabForm(int _index);

    // 
    virtual Tab* createTab(const char* _title, Form* _form, int _index) = 0;
    virtual void destroyTab(Tab* _tab) = 0;
    virtual void setTabName(Tab* _tab, const char* _name) = 0;

    std::list<Tab*> mTabs;
    TabControlEventHandler* mTabControlEventHandler;
};

#endif
