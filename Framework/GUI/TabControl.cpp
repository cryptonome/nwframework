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

