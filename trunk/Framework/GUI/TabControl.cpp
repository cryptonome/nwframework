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
#include "PchGUI.h"

#include "TabControl.h"
#include "GUI.h"
#include "ControlFactory.h"
#include "Form.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Tab::init(TabControl* _tabControl, Form* _form, int _index, const char* _name)
{
    bool ok = true;

    if (!isOk())
    {
        mIndex = _index;
        mForm = _form;
        mTabControl = _tabControl;
        mName = _name;
        mInit = true;
    }

    return ok;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Tab::done()
{
    if (isOk())
    {
        DISPOSE(mForm);
        //DISPOSE(mTabControl);
        mInit = false;
    }
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Tab::setActive()
{
    mTabControl->setActiveTab(this);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Tab::isActive() const
{
    bool active = mTabControl->getActiveTab() == this;
    return active;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Tab::setName(const char* _name)
{
    mName = _name;
    mTabControl->setTabName(this, _name);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
TabControl::TabControl() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool TabControl::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mTabControlEventHandler = 0;

        bOK = Inherited::init(_gui, _name, _parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void TabControl::done()
{
    if (isOk())
    {
        for ( std::list<Tab*>::iterator it = mTabs.begin() ; it != mTabs.end() ; ++it )
        {
            Tab* itTab = *it;
            DISPOSE(itTab);
        }
        mTabs.clear();

        Inherited::done();
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
StrId TabControl::getControlType() const
{ 
    return staticGetControlType(); 
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
StrId TabControl::staticGetControlType() 
{ 
    return StrId("TabControl");
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Tab* TabControl::addTab(const char* _name, Form* _form)
{
    if ( _form == 0 )
    {
        _form = new Form();
        _form->init(mGUI,_name,0,0,0);
        _form->setSize(800,600);
    }
    int index = (int)mTabs.size();
    Tab* tab = createTab(_name,_form,index);
    mTabs.push_back(tab);

    setActiveTab(tab);

    return tab;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void TabControl::removeTab(Tab* _tab)
{
    bool found = false;

    int index = _tab->getIndex();

    for ( std::list<Tab*>::iterator it = mTabs.begin() ; it != mTabs.end() ; )
    {
        Tab* itTab = *it;
        if ( _tab == itTab )
        {
            destroyTab(_tab);
            it = mTabs.erase(it);
            found = true;
        }
        else
        {
            // Decrementamos en 1 los indices de los que estaban por detras del que eliminamos
            if ( found )
                itTab->setIndex(itTab->getIndex()-1);
            ++it;
        }
    }

    // Ponemos como activo el tab que estaba en la posicion de el que
    // hemos eliminado
    if ( index >= (int)mTabs.size() )
        index--;
    if ( index >= 0 )
        setActiveTab(getTab(index));

    ASSERT(found);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void TabControl::setVisibleTabForm(int _index)
{
    bool found = false;
    for ( std::list<Tab*>::iterator it = mTabs.begin() ; it != mTabs.end() ; ++it )
    {
        Tab* tab = *it;
        bool visible = tab->getIndex() == _index;
        tab->getForm()->setVisible(visible);
        found |= visible;
    }
    ASSERT(found);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void TabControl::setActiveTab(Tab* _tab)
{
    int index = _tab->getIndex();

    setVisibleTabForm(index);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Tab* TabControl::getTab(int _index)
{
    Tab* tab = 0;
    for ( std::list<Tab*>::iterator it = mTabs.begin() ; tab == 0 && it != mTabs.end() ; ++it )
    {
        Tab* itTab = *it;
        if ( itTab->getIndex() == _index )
            tab = itTab;
    }
    ASSERT(tab);

    return tab;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Tab* TabControl::getTabByName(const char* _name)
{
    Tab* tab = 0;
    for ( std::list<Tab*>::iterator it = mTabs.begin() ; tab == 0 && it != mTabs.end() ; ++it )
    {
        Tab* itTab = *it;
        if ( strcmp(itTab->getName(),_name) == 0 )
            tab = itTab;
    }
    ASSERT(tab);

    return tab;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void TabControl::sendEventTabControlModified()
{
    if ( mTabControlEventHandler )
        mTabControlEventHandler->tabControlModified(this);
}

