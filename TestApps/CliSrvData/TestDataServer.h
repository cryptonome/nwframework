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
