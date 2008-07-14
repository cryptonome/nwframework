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

#include "GUIControl.h"
#include "GUI.h"
#include "ControlFactory.h"
#include "GUIAdapter.h"
#include "GUIAdapterFactory.h"
#include "Window.h"
#include "Form.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GUIControl::GUIControl() :
    mInit(false)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GUIControl::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        ASSERT(_parent || getControlType() == Window::staticGetControlType() || 
                          getControlType() == Form::staticGetControlType() );
        mGUI = _gui;
        mParent = 0;
        mAdapter = 0;
        mDataObjContext = "";
        mDataObjName = "";
        mDataObjType = "";
        mDataObjParams = "";

        if ( _parent )
            _parent->addChild(this);

        mInit = true;
    }
    return bOK;

}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GUIControl::done()
{
    if (isOk())
    {
        if ( mParent )
        {
            mParent->removeChild(this);
            mParent = 0;
        }

        int childs = 0;
        while ( childs = getNumChilds() )
        {
            GUIControl* control = getChild(0);
            DISPOSE(control);
        }

        DISPOSE(mAdapter);
        mInit = false;
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GUIControl::load(const DataScript& _data)
{
    bool bOK = true;

    DataScript& dsControl = (DataScript&)_data;

    int x = dsControl.getInt("POSX");
    int y = dsControl.getInt("POSY");
    setPosition(x,y);
    int width = dsControl.getInt("WIDTH");
    int height = dsControl.getInt("HEIGHT");
    setSize(width,height);

    mDataObjContext = dsControl.getString("DATAOBJCONTEXT");
    mDataObjName = dsControl.getString("DATAOBJNAME");
    mDataObjType = dsControl.getString("DATAOBJTYPE");
    mDataObjParams = dsControl.getString("DATAOBJPARAMS");

    // Children
    DataScript dsChildren = dsControl["CHILDREN"];
    int numChilds = dsChildren.count();
    for ( int i = 0 ; i < numChilds && bOK ; ++i )
    {
        DataScript dsChild = dsChildren[i];
        std::string type = dsChild.getString("TYPE");
        std::string name = dsChild.getString("NAME");
        GUIControl* child = mGUI->getControlFactory()->create(type.c_str(),name.c_str(),this);
        if (child)
        {
            bOK = child->load(dsChild);
            //addChild(child);
        }
    }

    ASSERT(bOK);

    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GUIControl::save(DataScript& _data)
{
    DataScript& dsControl = _data;

    dsControl.setInt("GUIVERSION", GUI::getVersionNumber());
    dsControl.setString("TYPE", getControlType().c_str());
    dsControl.setString("NAME", getName());
    int posX, posY;
    getPosition(posX,posY);
    dsControl.setInt("POSX", posX);
    dsControl.setInt("POSY", posY);
    int width, height;
    getSize(width,height);
    dsControl.setInt("WIDTH", width);
    dsControl.setInt("HEIGHT", height);

    dsControl.setString("DATAOBJCONTEXT", getDataObjContext());
    dsControl.setString("DATAOBJNAME", getDataObjName());
    dsControl.setString("DATAOBJTYPE", getDataObjType());
    dsControl.setString("DATAOBJPARAMS", getDataObjParams());

    // Children
    int numChilds = getNumChilds();
    if ( numChilds )
    {
        DataScript dsChildren = dsControl.addSection("CHILDREN");

        for ( int i = 0 ; i < numChilds ; ++i )
        {
            GUIControl* child = getChild(i);
            if ( child )
            {
                DataScript dsChild = dsChildren.addSection("CHILD");
                child->save(dsChild);
            }
        }
    }
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GUIControl* GUIControl::getChildByName(const char* _name, bool _recursive)
{
    GUIControl* control = 0;

    int childs = getNumChilds();
    for ( int i = 0 ; control == 0 && i < childs ; ++i )
    {
        GUIControl* itControl = getChild(i);
        if ( strcmp(itControl->getName(),_name) == 0 )
            control = itControl;
        else if ( _recursive )
            control = itControl->getChildByName(_name,_recursive);
    }    

    return control;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GUIControl::setAdapter(IGUIAdapter* _adapter)
{
    ASSERT(mAdapter == 0);
    mAdapter = _adapter;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GUIControl::internalCreateAdapter(bool _recursive)
{
    if ( _recursive )
    {
        int childs = getNumChilds();
        for ( int i = 0 ; i < childs ; ++i )
        {
            GUIControl* child = getChild(i);
            child->internalCreateAdapter(_recursive);
        }
    }

    if ( strcmp(getDataObjName(),"") != 0 )
        mGUI->getAdapterFactory()->createAdapter(this,getDataObjType(),getDataObjContextRecursive(),getDataObjName());
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
const char* GUIControl::getDataObjContextRecursive() const
{
    const char* context = getDataObjContext();
    if ( strcmp(context,"") == 0 && mParent )
        context = mParent->getDataObjContextRecursive();

    return context;
}
