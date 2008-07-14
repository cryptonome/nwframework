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

