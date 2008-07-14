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
