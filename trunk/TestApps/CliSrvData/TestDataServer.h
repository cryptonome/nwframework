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
#ifndef TEST_DATA_SERVER_H
#define TEST_DATA_SERVER_H

#include "NWSvcDataObject.h"
#include "NWSvcDataList.h"

class NWSvcDataServer;

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
struct TestIntDelegate
{
    int mValue;

    inline void set(int _value);
    inline int get() const;
};

inline void TestIntDelegate::set(int _value)
{
    mValue = _value;
}

inline int TestIntDelegate::get() const
{
    return mValue;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
struct sDataListObj
{
    int mIndex;
    StrId mName;

    sDataListObj()
    {
        mIndex = -1;
        mName = "NONAMED";
    }

    sDataListObj(int _index, StrId _name)
    {
        mIndex = _index;
        mName = _name;
    }

    void serializeIn(MemorySerializerIn & _serializerIn)
    {
        mIndex = _serializerIn.getInt();
        mName = _serializerIn.getString();
    }

    void serializeOut(MemorySerializerOut &_serializerOut) const
    {
        _serializerOut.addInt(mIndex);
        _serializerOut.addString(mName.c_str());
    }
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class TestDataServer : public NWSvcDataEventNotificationCallback
{
public:
    TestDataServer();
    ~TestDataServer();

    bool init(NWSvcDataServer * _svrDataService);
    void shutdown();

    virtual void receiveEventMsg(int _eventType, MemBufferRef * _eventData);

private:
    bool mInitd;
    NWSvcDataServer * mSvrDataService;
    TestIntDelegate mTestIntValue;

    NWSVR_OBJ(TestDataServer);

    NWSVC_DATA_OBJ(bool, SvrBool);
    NWSVC_DATA_OBJ(std::string, SvrText);

    NWSVC_DATA_EVENTHANDLER(mEventHandler);

    //NWSVC_DATA_OBJ_DELEGATE_MEMBER(TestIntDelegate, int, DelegateInt);

    NWSVC_DATA_OBJ_DELEGATE(int, TestDelegate2);
    //NWSvcDataObjDelegate<TestDataServer, int> mDelegateTestDelegate2;

    NWSVC_DATALIST(sDataListObj, mDataList);
};

#endif // TEST_DATA_SERVER_H
