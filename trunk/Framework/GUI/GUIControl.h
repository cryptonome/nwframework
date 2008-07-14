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
