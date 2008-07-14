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
