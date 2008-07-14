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
#ifndef GUIADAPTERCREATOR_H_
#define GUIADAPTERCREATOR_H_

#include "GUIAdapter.h"
#include "StrId.h"
#include "NWIP.h"

class GUIControl;
class IGUIAdapter;
class NWSvcDataServer;

//********************************************************************
//
//********************************************************************
template <typename T> inline const char* getDataTypeStringName();
template <> inline const char* getDataTypeStringName<bool>() { return "bool"; }
template <> inline const char* getDataTypeStringName<std::string>() { return "string"; }
template <> inline const char* getDataTypeStringName<int>() { return "int"; }
template <> inline const char* getDataTypeStringName<NWIP>() { return "NWIP"; }

//********************************************************************
//
//********************************************************************
class IGUIAdapterCreator
{
public:
    virtual ~IGUIAdapterCreator() { }

    virtual IGUIAdapter* create(GUIControl* _control, NWSvcDataServer * _dataProvider, StrId _dataObjContext, StrId _dataObjName) = 0;
    virtual StrId getControlType() = 0;
    virtual StrId getDataType() = 0;
};

//********************************************************************
//
//********************************************************************
/*#define IMPLEMENT_ADAPTER_CREATOR(classname) \
    class classname##Creator : public IGUIAdapterCreator \
    { \
    public: \
        virtual IGUIAdapter* create(GUIControl* _control, NWSvcDataServer * _dataProvider, StrId _dataObjContext, StrId _dataObjName) \
        { \
            GACheckBoxBool* adapter = NEW GACheckBoxBool(); \
            bool ok = adapter->init(_control, _dataProvider, _dataObjContext, _dataObjName); \
            ASSERT(ok); ok; \
            return adapter; \
        } \
        virtual StrId getControlType() \
        { \
            return classname::getControlType(); \
        } \
        \
        virtual StrId getDataType() \
        { \
            return classname::getDataType(); \
        } \
    };*/

template<typename GUI_ADAPTER_CLASS>
class GUIAdapterCreator : public IGUIAdapterCreator
{
    virtual IGUIAdapter* create(GUIControl* _control, NWSvcDataServer * _dataProvider, StrId _dataObjContext, StrId _dataObjName) 
    { 
        GUI_ADAPTER_CLASS* adapter = NEW GUI_ADAPTER_CLASS(); 
        bool ok = adapter->init(_control, _dataProvider, _dataObjContext, _dataObjName); 
        ASSERT(ok); ok; 
        return adapter; 
    } 
    virtual StrId getControlType() 
    { 
        return GUI_ADAPTER_CLASS::getControlType(); 
    } 
    
    virtual StrId getDataType() 
    { 
        return GUI_ADAPTER_CLASS::getDataType(); 
    }
};

//********************************************************************
//
//********************************************************************
#define REGISTER_ADAPTER_CREATOR(factory,classname) \
        { \
            GUIAdapterCreator<classname>* adapter = NEW GUIAdapterCreator<classname>(); \
            factory->registerAdapterCreator(adapter); \
        }

#endif
