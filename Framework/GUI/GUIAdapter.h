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
 */
#ifndef GUIADAPTER_H_
#define GUIADAPTER_H_

#include "StrId.h"
#include "NWSvcDataObject.h"
#include "NWIP.h"

class GUIControl;

//********************************************************************
//
//********************************************************************
template <typename T> inline T getDataObjDefaultObj();
template <> inline bool getDataObjDefaultObj<bool>() { return false; }
template <> inline std::string getDataObjDefaultObj<std::string>() { return ""; }
template <> inline int getDataObjDefaultObj<int>() { return 0; }
template <> inline NWIP getDataObjDefaultObj<NWIP>() { NWIP ip; return ip; }

//********************************************************************
//
//********************************************************************
class IGUIAdapter
{
public:
    virtual ~IGUIAdapter() { }
};

//********************************************************************
//
//********************************************************************
template <typename CONTROL_TYPE, typename DATA_TYPE>
class GUIAdapter : public IGUIAdapter, public NWSvcDataObjNotificationCallback
{
public:
    GUIAdapter() : mData(getDataObjDefaultObj<DATA_TYPE>()) { }

    virtual ~GUIAdapter()
    {
        mData.removeNotificationCallback(this);
        mData.shutdown();
    }

    virtual bool initAdapter() = 0;

    virtual bool init(GUIControl* _control, NWSvcDataServer * _dataProvider, StrId _dataObjContext, StrId _dataObjName)
    {
        mControl = _control->convertTo<CONTROL_TYPE>();
        ASSERT(mControl != 0);
        
        mDataProvider = _dataProvider;
        REG_START(_dataProvider, _dataObjContext.c_str());
            REG_CLI_OBJ(_dataObjName.c_str(), mData);
        REG_END();
        mData.addNotificationCallback(this);

        return initAdapter();
    }

    static StrId getControlType()
    {
        return CONTROL_TYPE::staticGetControlType();
    }

    static StrId getDataType()
    {
        return getDataTypeStringName<DATA_TYPE>();
    }
    
protected:
    NWSvcDataObj<DATA_TYPE> mData;
    CONTROL_TYPE* mControl;
    NWSvcDataServer* mDataProvider;

    const char* getDataObjContext() const
    {
        return mData.getContextName();
    }

    const char* getDataObjName() const
    {
        return mData.getObjName();
    }
};


#endif


