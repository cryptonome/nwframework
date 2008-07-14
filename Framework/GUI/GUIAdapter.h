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


