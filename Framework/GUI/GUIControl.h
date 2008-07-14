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


#ifndef CONTROL_H_
#define CONTROL_H_

#include "StrId.h"
#include "DataScript.h"
#include <string>

class GUI;
class IGUIAdapter;

//********************************************************************
//
//********************************************************************
class GUIControl
{
public:
    GUIControl            ();
    virtual ~GUIControl   ()                      { GUIControl::done(); }

    bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    bool          isOk        () const  { return mInit; }
    void          done        ();

    // Type
    virtual StrId getControlType() const = 0;
    template <typename T> T* convertTo(bool _checktype = true);

    // Child
    virtual void addChild(GUIControl* _child) = 0;
    virtual int getNumChilds() const = 0;
    virtual GUIControl* getChild(int _index) const = 0;

    // Name
    virtual void setName(const char* _name) = 0;
    virtual const char* getName() const = 0;

    // Position/Size
    virtual void setPosition(int _x, int _y) = 0;
    virtual void getPosition(int& _x, int& _y) const = 0;
    virtual void setSize(int _width, int _height) = 0;
    virtual void getSize(int& _width, int& _height) const = 0;

    // Visible
    virtual void setVisible(bool _visible) = 0;
    virtual bool getVisible() const = 0;

    // Enabled
    virtual void setEnabled(bool _enabled) = 0;
    virtual bool getEnabled() const = 0;

    // Load/Save
    virtual bool load(const DataScript& _data);
    virtual void save(DataScript& _data);

    // DataObj
    virtual void setDataObjContext(const char* _str) { mDataObjContext = _str; }
    virtual const char* getDataObjContext() const { return mDataObjContext.c_str(); }
    virtual void setDataObjName(const char* _str) { mDataObjName = _str; }
    virtual const char* getDataObjName() const { return mDataObjName.c_str(); }
    virtual void setDataObjType(const char* _str) { mDataObjType = _str; }
    virtual const char* getDataObjType() const { return mDataObjType.c_str(); }
    virtual void setDataObjParams(const char* _str) { mDataObjParams = _str; }
    virtual const char* getDataObjParams() const { return mDataObjParams.c_str(); }

    GUIControl* getChildByName(const char* _name, bool _recursive);
    template <typename T> T* getChildByName(const char* _name, bool _recursive)
    {
        GUIControl* control = getChildByName(_name,_recursive);
        return control ? control->convertTo<T>() : 0;
    }

    void createAdapter(bool _recursive) { internalCreateAdapter(_recursive); }


    virtual GUIControl* getControl () { return this; }

protected:
    friend class Form;
    friend class GUIAdapterFactory;
    void internalCreateAdapter(bool _recursive);
    void setAdapter(IGUIAdapter* _adapter);
    const char* getDataObjContextRecursive() const;

    GUI* mGUI;
    GUIControl* mParent;
    IGUIAdapter* mAdapter;

    std::string mDataObjContext;
    std::string mDataObjName;
    std::string mDataObjType;
    std::string mDataObjParams;

    virtual void removeChild(GUIControl* _control) = 0;

private:

    bool          mInit : 1;
};


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T> T* GUIControl::convertTo(bool _checktype)
{
    T* ret = 0;
    if ( getControlType() == T::staticGetControlType() )
        ret = dynamic_cast<T*>(this);
    else
    {
        ASSERT(_checktype == false);
    }
    return ret;
}


#endif
