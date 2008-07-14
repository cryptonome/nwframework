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
