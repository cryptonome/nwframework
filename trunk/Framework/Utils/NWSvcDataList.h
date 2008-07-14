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


#ifndef NW_SVC_DATA_LIST_H
#define NW_SVC_DATA_LIST_H

#include "NWSvcDataObject.h"

#include <vector>

//****************************************************************************
//
//****************************************************************************
struct NWSvcDataListCallback
{
    virtual void dataListEvent(StrId _contextName, StrId _objName, int _eventType, int _updatedElementIndex, int _objUniqueId)=0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
class NWSvcDataList : public NWSvcDataObject, public NWSvcDataEventNotificationCallback
{
public:
    enum eDataListNotifications
    {
        // --- Notifications ---
        eDataListNotification_ElemCreated = 0,
        eDataListNotification_ElemErased,
        eDataListNotification_ValueChanged,
        // ---
        eDataListNotification_Num
    };

    NWSvcDataList(int _reservedElements=16);
    virtual ~NWSvcDataList();

    virtual bool init(const char * _contextName, const char * _objName, NWSvcDataServer * _dataServer, bool _isServer);
    virtual void shutdown();

    void addNotificationCallback(NWSvcDataListCallback * _callback);
    void removeNotificationCallback(NWSvcDataListCallback * _callback);

    inline int getNumElems() const;

    T const * getHead() const;
    T const * getTail() const;
    T const * getElem(int _pos) const;

    void createAsHead(T const & _val, bool _serverUpdate=false);
    void createAsTail(T const & _val, bool _serverUpdate=false);
    void createAsNextOf(int _pos, T const & _val, bool _serverUpdate=false);
    void createAsPrevOf(int _pos, T const & _val, bool _serverUpdate=false);

    void eraseHead(bool _serverUpdate=false);
    void eraseTail(bool _serverUpdate=false);
    void erase(int _pos, bool _serverUpdate=false);

    void setVal(int _pos, T const & _val, bool _serverUpdate=false);

protected:
    virtual void serializeIn(MemorySerializerIn & _serializerIn);
    virtual void serializeOut(MemorySerializerOut & _serializerOut) const;

    virtual void NotifyEvent(int _eventType, int _updatedElementIndex, int _objUniqueId);
    virtual void receiveEventMsg(int _eventType, MemBufferRef * _eventData);

private:
    typedef NWSvcDataObject Inherited;

    struct sObjNode
    {
        int mObjUniqueId;
        T mObj;
        sObjNode() :
            mObjUniqueId(0)
        {
        }
        sObjNode(int _objUniqueId, T const & _obj) :
            mObjUniqueId(_objUniqueId)
        {
            mObj = _obj;
        }
    };

    std::vector<sObjNode> mObjList;
    int mObjUniqueIdSeed;

    NWSVR_OBJ(NWSvcDataList);
    NWSVC_DATA_EVENTHANDLER(mEventHandler);

    enum eDataListEvent
    {
        // --- Internal Svr - Cli Msgs ---
        eDataListEvent_CreateHead = 0,
        eDataListEvent_CreateTail,
        eDataListEvent_CreateNextOf,
        eDataListEvent_CreatePrevOf,
        eDataListEvent_EraseHead,
        eDataListEvent_EraseTail,
        eDataListEvent_ErasePos,
        eDataListEvent_SetValPos,
        // --- ---
        eDataListEvent_Num
    };
};

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
NWSvcDataList<T>::NWSvcDataList(int _reservedElements/*=16*/) : Inherited(),
    mObjUniqueIdSeed(0)
{
    mObjList.reserve(_reservedElements);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
/*virtual*/ NWSvcDataList<T>::~NWSvcDataList()
{
    if(isInitd())
        shutdown();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
/*virtual*/ void NWSvcDataList<T>::addNotificationCallback(NWSvcDataListCallback * _callback)
{
    if(_callback)
    {
        NotificationCallbackNode node;
        node.mCallbackPtr = (void *)_callback;

        mNotificationCallbackList.push_back(node);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
/*virtual*/ void NWSvcDataList<T>::removeNotificationCallback(NWSvcDataListCallback * _callback)
{
    if(_callback)
    {
        int num = (int)mNotificationCallbackList.size();
        for (int i=0; i<num; i++)
        {
            if(mNotificationCallbackList[i].mCallbackPtr == _callback)
                mNotificationCallbackList.erase(mNotificationCallbackList.begin() + i);
        }
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
/*virtual*/ bool NWSvcDataList<T>::init(const char * _contextName, const char * _objName, NWSvcDataServer * _dataServer, bool _isServer)
{
    bool bRet = false;

    if(Inherited::init(_contextName, _objName, _dataServer, _isServer))
    {
        mObjUniqueIdSeed = 0;

        std::string objName = _objName;
        std::string addName = "EventHandler";
        std::string eventHandlerName = objName + addName;

        REG_START(_dataServer, _contextName);
            if(_isServer)
            {
                REG_SVR_EVENTHANDLER(eventHandlerName.c_str(), mEventHandler);
            }
            else
            {
                REG_CLI_EVENTHANDLER(eventHandlerName.c_str(), mEventHandler);
            }
        REG_END();

        mEventHandler.addNotificationCallback(this);

        bRet = true;
    }

    return bRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
/*virtual*/ void NWSvcDataList<T>::shutdown()
{
    if(isInitd())
    {
        Inherited::shutdown();

        mObjList.clear();
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
inline int NWSvcDataList<T>::getNumElems() const
{
    return (int)mObjList.size();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
T const * NWSvcDataList<T>::getHead() const
{
    return (mObjList.size() > 0) ? &mObjList[0].mObj : NULL;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
T const * NWSvcDataList<T>::getTail() const
{
    int numElems = mObjList.size();
    return (numElems > 0) ? &mObjList[numElems-1].mObj : NULL;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
T const * NWSvcDataList<T>::getElem(int _pos) const
{
    return (mObjList.size() > _pos) ? &mObjList[_pos].mObj : NULL;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
void NWSvcDataList<T>::createAsHead(T const & _val, bool _serverUpdate/*=false*/)
{
    if(mServerMode || _serverUpdate)
    {
        int objUniqueId = ++mObjUniqueIdSeed;
        sObjNode objNone(objUniqueId, _val);
        mObjList.insert(mObjList.begin(), objNone);

        NotifyEvent(eDataListNotification_ElemCreated, 0, objUniqueId);

        if(_serverUpdate)
            mLastReceivedUpdateLevel++;
    }
    else
    {
        NWGenericEventBufferData ev;
        ev.mEventType = eDataListEvent_CreateHead;
        
        MemorySerializerOut serializerOut;
        serializerOut.init(1024);

        _val.serializeOut(serializerOut);

        ev.mMemBuffer = serializerOut.getMemBuffer();

        mEventHandler.sendEvent(&ev);

        mCurrentUpdateLevel++;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
void NWSvcDataList<T>::createAsTail(T const & _val, bool _serverUpdate/*=false*/)
{
    if(mServerMode || _serverUpdate)
    {
        int objUniqueId = ++mObjUniqueIdSeed;
        sObjNode objNone(objUniqueId, _val);
        mObjList.push_back(objNone);

        NotifyEvent(eDataListNotification_ElemCreated, (int)mObjList.size()-1, objUniqueId);

        if(_serverUpdate)
            mLastReceivedUpdateLevel++;
    }
    else
    {
        NWGenericEventBufferData ev;
        ev.mEventType = eDataListEvent_CreateTail;

        MemorySerializerOut serializerOut;
        _val.serializeOut(serializerOut);

        ev.mMemBuffer = serializerOut.getMemBuffer();

        mEventHandler.sendEvent(&ev);

        mCurrentUpdateLevel++;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
void NWSvcDataList<T>::createAsNextOf(int _pos, T const & _val, bool _serverUpdate/*=false*/)
{
    if(mServerMode || _serverUpdate)
    {
        int objUniqueId = ++mObjUniqueIdSeed;
        sObjNode objNone(objUniqueId, _val);
        mObjList.insert(mObjList.begin()+_pos+1, objNone);

        NotifyEvent(eDataListNotification_ElemCreated, _pos+1, objUniqueId);

        if(_serverUpdate)
            mLastReceivedUpdateLevel++;
    }
    else
    {
        NWGenericEventBufferData ev;
        ev.mEventType = eDataListEvent_CreateNextOf;

        MemorySerializerOut serializerOut;
        serializerOut.addInt(_pos);
        serializerOut.addInt(mObjList[_pos].mObjUniqueId);
        _val.serializeOut(serializerOut);

        ev.mMemBuffer = serializerOut.getMemBuffer();

        mEventHandler.sendEvent(&ev);

        mCurrentUpdateLevel++;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
void NWSvcDataList<T>::createAsPrevOf(int _pos, T const & _val, bool _serverUpdate/*=false*/)
{
    if(mServerMode || _serverUpdate)
    {
        int objUniqueId = ++mObjUniqueIdSeed;
        sObjNode objNone(objUniqueId, _val);
        mObjList.insert(mObjList.begin()+_pos, objNone);

        NotifyEvent(eDataListNotification_ElemCreated, _pos, objUniqueId);

        if(_serverUpdate)
            mLastReceivedUpdateLevel++;
    }
    else
    {
        NWGenericEventBufferData ev;
        ev.mEventType = eDataListEvent_CreatePrevOf;

        MemorySerializerOut serializerOut;
        serializerOut.addInt(_pos);
        serializerOut.addInt(mObjList[_pos].mObjUniqueId);
        _val.serializeOut(serializerOut);

        ev.mMemBuffer = serializerOut.getMemBuffer();

        mEventHandler.sendEvent(&ev);

        mCurrentUpdateLevel++;
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
void NWSvcDataList<T>::eraseHead(bool _serverUpdate/*=false*/)
{
    if(mServerMode || _serverUpdate)
    {
        int objUniqueId = mObjList[0].mObjUniqueId;

        mObjList.erase(mObjList.begin());

        NotifyEvent(eDataListNotification_ElemErased, 0, objUniqueId);

        if(_serverUpdate)
            mLastReceivedUpdateLevel++;
    }
    else
    {
        NWGenericEvent ev;
        ev.mEventType = eDataListEvent_EraseHead;
        ev.mUserData = mObjList[0].mObjUniqueId;
        mEventHandler.sendEvent(&ev);

        mCurrentUpdateLevel++;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
void NWSvcDataList<T>::eraseTail(bool _serverUpdate/*=false*/)
{
    if(mServerMode || _serverUpdate)
    {
        int pos = (int)mObjList.size() - 1;
        int objUniqueId = mObjList[pos].mObjUniqueId;

        mObjList.pop_back();

        NotifyEvent(eDataListNotification_ElemErased, pos, objUniqueId);

        if(_serverUpdate)
            mLastReceivedUpdateLevel++;
    }
    else
    {
        NWGenericEvent ev;
        ev.mEventType = eDataListEvent_EraseTail;
        ev.mUserData = mObjList[mObjList.size()-1].mObjUniqueId;
        mEventHandler.sendEvent(&ev);

        mCurrentUpdateLevel++;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
void NWSvcDataList<T>::erase(int _pos, bool _serverUpdate/*=false*/)
{
    if(mServerMode || _serverUpdate)
    {
        int objUniqueId = mObjList[_pos].mObjUniqueId;

        mObjList.erase(mObjList.begin()+_pos);

        NotifyEvent(eDataListNotification_ElemErased, _pos, objUniqueId);

        if(_serverUpdate)
            mLastReceivedUpdateLevel++;
    }
    else
    {
        NWGenericEventUserData2 ev;
        ev.mEventType = eDataListEvent_ErasePos;
        ev.mUserParam1 = _pos;
        ev.mUserParam2 = mObjList[_pos].mObjUniqueId;
        mEventHandler.sendEvent(&ev);

        mCurrentUpdateLevel++;
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
void NWSvcDataList<T>::setVal(int _pos, T const & _val, bool _serverUpdate/*=false*/)
{
    if(mServerMode || _serverUpdate)
    {
        mObjList[_pos].mObj = _val;

        NotifyEvent(eDataListNotification_ValueChanged, _pos, mObjList[_pos].mObjUniqueId);

        if(_serverUpdate)
            mLastReceivedUpdateLevel++;
    }
    else
    {
        NWGenericEventBufferData ev;
        ev.mEventType = eDataListEvent_SetValPos;

        MemorySerializerOut serializerOut;
        serializerOut.addInt(_pos);
        serializerOut.addInt(mObjList[_pos].mObjUniqueId);
        _val.serializeOut(serializerOut);

        ev.mMemBuffer = serializerOut.getMemBuffer();

        mEventHandler.sendEvent(&ev);

        mCurrentUpdateLevel++;
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
/*virtual*/ void NWSvcDataList<T>::serializeIn(MemorySerializerIn & _serializerIn)
{
    ASSERT(mCurrentUpdateLevel > mLastReceivedUpdateLevel);
    mLastReceivedUpdateLevel++;

    int numElems = _serializerIn.getInt();
    mObjList.clear();
    mObjList.reserve(numElems * 2);

    for(int i=0; i<numElems; i++)
    {
        sObjNode node;
        mObjList.push_back(node);

        mObjList[i].mObjUniqueId = _serializerIn.getInt();
        mObjList[i].mObj.serializeIn(_serializerIn);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
/*virtual*/ void NWSvcDataList<T>::serializeOut(MemorySerializerOut & _serializerOut) const
{
    int numElems = (int)mObjList.size();
    _serializerOut.addInt(numElems);

    for(int i=0; i<numElems; i++)
    {
        _serializerOut.addInt(mObjList[i].mObjUniqueId);
        mObjList[i].mObj.serializeOut(_serializerOut);
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
/*virtual*/ void NWSvcDataList<T>::NotifyEvent(int _eventType, int _updatedElementIndex, int _objUniqueId)
{
    mLastReceivedUpdateLevel++;

    int num = (int)mNotificationCallbackList.size();
    for (int i=0; i<num; i++)
    {
        NWSvcDataListCallback * callback = (NWSvcDataListCallback *)mNotificationCallbackList[i].mCallbackPtr;
        if(callback)
            callback->dataListEvent(mContextName.c_str(), mName.c_str(), _eventType, _updatedElementIndex, _objUniqueId);
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
/*virtual*/ void NWSvcDataList<T>::receiveEventMsg(int _eventType, MemBufferRef * _eventData)
{
    MemorySerializerIn serializerIn;
    serializerIn.setBuffer(_eventData->getPtr(), _eventData->getSize());

    switch(_eventType)
    {
        case NWSvcDataEvent::eEventType_Generic:
        {
            NWGenericEvent ev;
            ev.serializeIn(serializerIn);

            if((ev.mFromServer && !mServerMode) || (!ev.mFromServer && mServerMode))
            {
                switch(ev.mEventType)
                {
                    case eDataListEvent_EraseHead:
                    {
                        int objUniqueId = ev.mUserData;
                        ASSERT(mObjList[0].mObjUniqueId == objUniqueId);
                        if(mObjList[0].mObjUniqueId == objUniqueId)
                        {
                            eraseHead(ev.mFromServer);

                            if(isServer())
                                mEventHandler.sendEvent(&ev);
                        }
                        break;
                    }

                    case eDataListEvent_EraseTail:
                    {
                        int objUniqueId = ev.mUserData;
                        int pos = (int)mObjList.size() - 1;
                        ASSERT(mObjList[pos].mObjUniqueId == objUniqueId);
                        if(mObjList[pos].mObjUniqueId == objUniqueId)
                        {
                            eraseTail(ev.mFromServer);

                            if(isServer())
                                mEventHandler.sendEvent(&ev);
                        }
                        break;
                    }
                }
            }
            break;
        }

        case NWSvcDataEvent::eEventType_GenericUserData2:
        {
            NWGenericEventUserData2 ev;
            ev.serializeIn(serializerIn);

            if((ev.mFromServer && !mServerMode) || (!ev.mFromServer && mServerMode))
            {
                switch(ev.mEventType)
                {
                    case eDataListEvent_ErasePos:
                    {
                        int pos = ev.mUserParam1;
                        int objUniqueId = ev.mUserParam2;
                        ASSERT(mObjList[pos].mObjUniqueId == objUniqueId);
                        if(mObjList[pos].mObjUniqueId == objUniqueId)
                        {
                            erase(pos, ev.mFromServer);

                            if(isServer())
                                mEventHandler.sendEvent(&ev);
                        }
                        break;
                    }
                }
            }
            break;
        }


        case NWSvcDataEvent::eEventType_GenericBufferData:
        {
            NWGenericEventBufferData ev;
            ev.serializeIn(serializerIn);

            if((ev.mFromServer && !mServerMode) || (!ev.mFromServer && mServerMode))
            {
                switch(ev.mEventType)
                {
                    case eDataListEvent_CreateHead:
                    {
                        MemorySerializerIn serializerIn;
                        serializerIn.setBuffer(ev.mMemBuffer.getPtr(), ev.mMemBuffer.getSize());

                        T val;
                        val.serializeIn(serializerIn);
                        createAsHead(val, ev.mFromServer);

                        if(isServer())
                            mEventHandler.sendEvent(&ev);
                        break;
                    }

                    case eDataListEvent_CreateTail:
                    {
                        MemorySerializerIn serializerIn;
                        serializerIn.setBuffer(ev.mMemBuffer.getPtr(), ev.mMemBuffer.getSize());

                        T val;
                        val.serializeIn(serializerIn);
                        createAsTail(val, ev.mFromServer);

                        if(isServer())
                            mEventHandler.sendEvent(&ev);
                        break;
                    }

                    case eDataListEvent_CreateNextOf:
                    {
                        MemorySerializerIn serializerIn;
                        serializerIn.setBuffer(ev.mMemBuffer.getPtr(), ev.mMemBuffer.getSize());

                        int pos = serializerIn.getInt();
                        int objUniqueId = serializerIn.getInt();

                        ASSERT(mObjList[pos].mObjUniqueId == objUniqueId);

                        T val;
                        val.serializeIn(serializerIn);
                        createAsNextOf(pos, val, ev.mFromServer);

                        if(isServer())
                            mEventHandler.sendEvent(&ev);
                        break;
                    }

                    case eDataListEvent_CreatePrevOf:
                    {
                        MemorySerializerIn serializerIn;
                        serializerIn.setBuffer(ev.mMemBuffer.getPtr(), ev.mMemBuffer.getSize());

                        int pos = serializerIn.getInt();
                        int objUniqueId = serializerIn.getInt();

                        ASSERT(mObjList[pos].mObjUniqueId == objUniqueId);

                        T val;
                        val.serializeIn(serializerIn);
                        createAsPrevOf(pos, val, ev.mFromServer);

                        if(isServer())
                            mEventHandler.sendEvent(&ev);
                        break;
                    }

                    case eDataListEvent_SetValPos:
                    {
                        MemorySerializerIn serializerIn;
                        serializerIn.setBuffer(ev.mMemBuffer.getPtr(), ev.mMemBuffer.getSize());

                        int pos = serializerIn.getInt();
                        int objUniqueId = serializerIn.getInt();

                        ASSERT(mObjList[pos].mObjUniqueId == objUniqueId);

                        T val;
                        val.serializeIn(serializerIn);
                        setVal(pos, val, ev.mFromServer);

                        if(isServer())
                            mEventHandler.sendEvent(&ev);
                        break;
                    }
                }
            }
            break;
        }
    }
}

//****************************************************************************
//
//****************************************************************************
#define NWSVC_DATALIST(OBJ_TYPE, NWCLASS_MEMBER_NAME) \
    NWSvcDataList<OBJ_TYPE> NWCLASS_MEMBER_NAME;

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#define REG_SVR_DATALIST(NWPSZ_NAME,NWCLASS_MEMBER) \
    NWCLASS_MEMBER.init(s_currentContext,NWPSZ_NAME,s_currentSvrDataService,true); \
    s_currentSvrDataService->regObj(s_currentContext, NWPSZ_NAME, &NWCLASS_MEMBER);

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#define REG_CLI_DATALIST(NWPSZ_NAME,NWCLASS_MEMBER) \
    NWCLASS_MEMBER.init(s_currentContext,NWPSZ_NAME,s_currentSvrDataService,false); \
    s_currentSvrDataService->regObj(s_currentContext, NWPSZ_NAME, &NWCLASS_MEMBER);


#endif // NW_SVC_DATA_LIST_H
