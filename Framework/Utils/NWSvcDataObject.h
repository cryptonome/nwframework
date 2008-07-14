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


#ifndef NW_SVR_DATA_OBJECT_H
#define NW_SVR_DATA_OBJECT_H

#include "NWDelegate.h"
#include "NWSvcDataSerializer.h"
#include "NWSvcDataContext.h"
#include "NWSvcDataServer.h"
#include "StrId.h"
#include "MemBufferRef.h"

#include <string>
#include <vector>

class NWSvcDataServer;
class MemorySerializerIn;
class MemorySerializerOut;

//****************************************************************************
//
//****************************************************************************

//****************************************************************************
//
//****************************************************************************
class NWSvcDataBase
{
public:
    NWSvcDataBase();
    virtual ~NWSvcDataBase();

    virtual bool init(const char * _contextName, const char * _objName, NWSvcDataServer * _dataServer, bool _isServer);
    virtual void shutdown();

    inline bool isInitd();

    inline bool beginUpdate();
    inline void endUpdate();

    inline const char* getObjName() const {return mName.c_str();}
    inline const char* getContextName() const{return mContextName.c_str();}

    inline bool isServer() const {return mServerMode;}

protected:
    bool mInitd;
    std::string mName;
    std::string mContextName;
    bool mServerMode;
    int mReentryUpdateLevel;
    NWSvcDataServer * mSvcDataServer;
    
    struct NotificationCallbackNode
    {
        void * mCallbackPtr;

        NotificationCallbackNode() :
            mCallbackPtr(NULL)
        {
        }
    };

    std::vector<NotificationCallbackNode> mNotificationCallbackList;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
inline bool NWSvcDataBase::isInitd()
{
    return mInitd;
}

inline bool NWSvcDataBase::beginUpdate()
{
    bool bRet = (mReentryUpdateLevel == 0);

    mReentryUpdateLevel++;

    return bRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
inline void NWSvcDataBase::endUpdate()
{
    if(mReentryUpdateLevel > 0)
    {
        mReentryUpdateLevel--;
    }
}

//****************************************************************************
//
//****************************************************************************
struct NWSvcDataObjNotificationCallback
{
    virtual void update(const char * _contextName, const char * _objName)=0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class NWSvcDataObject : public NWSvcDataBase
{
public:
    NWSvcDataObject();
    virtual ~NWSvcDataObject();

    virtual bool init(const char * _contextName, const char * _objName, NWSvcDataServer * _dataServer, bool _isServer);
    virtual void shutdown();

    virtual void addNotificationCallback(NWSvcDataObjNotificationCallback * _callback);
    virtual void removeNotificationCallback(NWSvcDataObjNotificationCallback * _callback);

    virtual void serializeIn(MemorySerializerIn & _serializerIn)=0;
    virtual void serializeOut(MemorySerializerOut & _serializerOut) const =0;

    inline bool isUpdated() const;

protected:
    virtual void NotifyUpdate();

    int mCurrentUpdateLevel;
    int mLastReceivedUpdateLevel;

private:
    typedef NWSvcDataBase Inherited;
};

inline bool NWSvcDataObject::isUpdated() const
{
    return isServer() ? true : (mCurrentUpdateLevel > 0 && (mCurrentUpdateLevel == mLastReceivedUpdateLevel));
}

//****************************************************************************
//
//****************************************************************************
template <typename TypeParam>
class NWSvcDataObj : public NWSvcDataObject
{
public:
    NWSvcDataObj();
    NWSvcDataObj(TypeParam _val);
    virtual ~NWSvcDataObj();

    virtual bool init(const char * _contextName, const char * _objName, NWSvcDataServer * _dataServer, bool _isServer);
    virtual void shutdown();

    virtual void set(TypeParam const & _val, bool _serverUpdate=false, bool _directUpdate=true);
    virtual TypeParam get() const;

    virtual void serializeIn(MemorySerializerIn & _serializerIn);
    virtual void serializeOut(MemorySerializerOut & _serializerOut) const;

protected:
    TypeParam mVal;

private:
    typedef NWSvcDataObject Inherited;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <typename TypeParam>
NWSvcDataObj<TypeParam>::NWSvcDataObj() : NWSvcDataObject()
{
}

template <typename TypeParam>
NWSvcDataObj<TypeParam>::NWSvcDataObj(TypeParam _val) : NWSvcDataObject(),
    mVal(_val)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <typename TypeParam>
NWSvcDataObj<TypeParam>::~NWSvcDataObj()
{
    if(mInitd)
        shutdown();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <typename TypeParam>
/*virtual*/ bool NWSvcDataObj<TypeParam>::init(const char * _contextName, const char * _objName, NWSvcDataServer * _dataServer, bool _isServer)
{
    return Inherited::init(_contextName, _objName, _dataServer, _isServer);
}

template <typename TypeParam>
/*virtual*/ void NWSvcDataObj<TypeParam>::shutdown()
{
    Inherited::shutdown();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <typename TypeParam>
inline void NWSvcDataObj<TypeParam>::set(TypeParam const & _val, bool _serverUpdate/*=false*/, bool _directUpdate/*=true*/)
{
    bool bRun = true;
    if(_directUpdate)
    {
        bRun = beginUpdate();
    }

    if(bRun)
    {
        ASSERT(mReentryUpdateLevel == 1);
        if(mReentryUpdateLevel == 1)
        {
            if(mServerMode || _serverUpdate) // if I'm the server or the server tells me to change the data...
            {
                mVal = _val;
                
                NotifyUpdate();

                if(mServerMode && mSvcDataServer)
                    mSvcDataServer->updateClients(this);

                if(_serverUpdate)
                    mLastReceivedUpdateLevel++;
            }
            else
            {
                if(mSvcDataServer)
                {
                    TypeParam val = _val;
                    MemorySerializerOut serializerOut;
                    SerializeDataOut<TypeParam>(serializerOut, val);
                    serializerOut.finalize();
                    mSvcDataServer->sendSetValue(this, serializerOut);
                }
            }
        }
    }

    if(_directUpdate)
    {
        endUpdate();
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <typename TypeParam>
inline TypeParam NWSvcDataObj<TypeParam>::get() const
{
    return mVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <typename TypeParam>
/*virtual*/ void NWSvcDataObj<TypeParam>::serializeIn(MemorySerializerIn & _serializerIn)
{
    TypeParam val;
    SerializeDataIn<TypeParam>(_serializerIn, val);

    set(val, true, true);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <typename TypeParam>
void NWSvcDataObj<TypeParam>::serializeOut(MemorySerializerOut & _serializerOut) const
{
    SerializeDataOut<TypeParam>(_serializerOut, mVal);
}

//****************************************************************************
//
//****************************************************************************
template <class T, typename TypeParam>
class NWSvcDataObjDelegate : public NWSvcDataObject
{
public:
    typedef typename NWDelegateDef<void*>::SetFnPtr SetFn;
    typedef typename NWDelegateDef<void*>::GetFnPtr GetFn;

    NWSvcDataObjDelegate();
    virtual ~NWSvcDataObjDelegate();

    virtual bool init(const char * _contextName, const char * _objName, NWSvcDataServer * _dataServer, SetFn _setFn, GetFn _getFn, T * _instance);
    virtual void shutdown();

    virtual void set(TypeParam const & _val, bool _serverUpdate=false, bool _directUpdate=true);
    virtual TypeParam get() const;

    virtual void serializeIn(MemorySerializerIn & _serializerIn);
    virtual void serializeOut(MemorySerializerOut & _serializerOut) const;

    typedef TypeParam DelegateParamType;
protected:
    SetFn mSetFn;
    GetFn mGetFn;
    NWDelegateCaller<T> * mDelegateCaller;
    
private:
    typedef NWSvcDataObject Inherited;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T, typename TypeParam>
NWSvcDataObjDelegate<T, TypeParam>::NWSvcDataObjDelegate() : Inherited(),
    mSetFn(NULL),
    mGetFn(NULL),
    mDelegateCaller(NULL)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T, typename TypeParam>
/*virtual*/ NWSvcDataObjDelegate<T, TypeParam>::~NWSvcDataObjDelegate()
{
    if(mInitd)
        shutdown();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T, typename TypeParam>
/*virtual*/ bool NWSvcDataObjDelegate<T, TypeParam>::init(const char * _contextName, const char * _objName, NWSvcDataServer * _dataServer, SetFn _setFn, GetFn _getFn, T * _instance)
{
    bool retVal = false;

    if(!mInitd)
    {
        ASSERT(_instance);
        retVal = Inherited::init(_contextName, _objName, _dataServer, true);
        
        ASSERT(retVal);

        if(retVal)
        {
            mSetFn = _setFn;
            mGetFn = _getFn;
            mDelegateCaller = NEW NWDelegateCaller<T>(_instance);
        }
    }

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T, typename TypeParam>
/*virtual*/ void NWSvcDataObjDelegate<T, TypeParam>::shutdown()
{
    if(mInitd)
    {
        DISPOSE(mDelegateCaller);

        Inherited::shutdown();
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T, typename TypeParam>
/*virtual*/ void NWSvcDataObjDelegate<T, TypeParam>::set(TypeParam const & _val, bool _serverUpdate/*=false*/, bool _directUpdate/*=true*/)
{
    bool bRun = true;
    if(_directUpdate)
    {
        bRun = beginUpdate();
    }

    if(bRun)
    {
        ASSERT(mReentryUpdateLevel == 1);
        if(mReentryUpdateLevel == 1)
        {
            ASSERT(mDelegateCaller);
            if(mDelegateCaller)
            {
                mDelegateCaller->set((NWDelegateDef<TypeParam>::SetFnPtr)mSetFn, _val);
                NotifyUpdate();
            }

            if(mSvcDataServer)
                mSvcDataServer->updateClients(this);
        }
    }

    if(_directUpdate)
    {
        endUpdate();
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T, typename TypeParam>
/*virtual*/ TypeParam NWSvcDataObjDelegate<T, TypeParam>::get() const
{
    return mDelegateCaller->get<TypeParam>((NWDelegateDef<TypeParam>::GetFnPtr)mGetFn);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T, typename TypeParam>
/*virtual*/ void NWSvcDataObjDelegate<T, TypeParam>::serializeIn(MemorySerializerIn & _serializerIn)
{
    TypeParam val;
    SerializeDataIn<TypeParam>(_serializerIn, val);

    set(val, true, true);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T, typename TypeParam>
/*virtual*/ void NWSvcDataObjDelegate<T, TypeParam>::serializeOut(MemorySerializerOut & _serializerOut) const
{
    TypeParam val = get();
    SerializeDataOut<TypeParam>(_serializerOut, val);
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class NWSvcDataEvent
{
public:
    bool mFromServer;       // event sent by the server to clients
    int mEventType;
    int mGenericType;

    enum eEventTypes
    {
        eEventType_Null = 0,
        eEventType_Generic,
        eEventType_GenericUserData2,
        eEventType_GenericBufferData,
        // --- ---
        eEventType_User
    };

    inline explicit NWSvcDataEvent(int _genericEventType) :
        mFromServer(false),
        mEventType(0),
        mGenericType(_genericEventType)

    {
    }

    virtual ~NWSvcDataEvent(){}

    inline int getEventType() const {return mEventType;}
    inline int getGenericEventType() const {return mGenericType;}

    virtual void serializeIn(MemorySerializerIn & _serializerIn)
    {
        mFromServer = _serializerIn.getBool();
        mEventType = _serializerIn.getInt();
        mGenericType = _serializerIn.getInt();
    }

    virtual void serializeOut(MemorySerializerOut & _serializerOut) const
    {
        _serializerOut.addBool(mFromServer);
        _serializerOut.addInt(mEventType);
        _serializerOut.addInt(mGenericType);
    }
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class NWGenericEvent : public NWSvcDataEvent
{
public:
    NWGenericEvent() : NWSvcDataEvent(eEventType_Generic),
        mUserData(0)
    {
    }

    int mUserData;

    virtual void serializeIn(MemorySerializerIn & _serializerIn)
    {
        NWSvcDataEvent::serializeIn(_serializerIn);

        mUserData = _serializerIn.getInt();
    }

    virtual void serializeOut(MemorySerializerOut & _serializerOut) const
    {
        NWSvcDataEvent::serializeOut(_serializerOut);

        _serializerOut.addInt(mUserData);
    }
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class NWGenericEventUserData2 : public NWSvcDataEvent
{
public:
    NWGenericEventUserData2() : NWSvcDataEvent(eEventType_GenericUserData2),
        mUserParam1(0),
        mUserParam2(0)
    {
    }

    unsigned int mUserParam1;
    unsigned int mUserParam2;

    virtual void serializeIn(MemorySerializerIn & _serializerIn)
    {
        NWSvcDataEvent::serializeIn(_serializerIn);

        mUserParam1 = _serializerIn.getInt();
        mUserParam2 = _serializerIn.getInt();
    }

    virtual void serializeOut(MemorySerializerOut & _serializerOut) const
    {
        NWSvcDataEvent::serializeOut(_serializerOut);

        _serializerOut.addInt(mUserParam1);
        _serializerOut.addInt(mUserParam2);
    }
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class NWGenericEventBufferData : public NWSvcDataEvent
{
public:
    NWGenericEventBufferData() : NWSvcDataEvent(eEventType_GenericBufferData) {}

    MemBufferRef mMemBuffer;

    virtual void serializeIn(MemorySerializerIn & _serializerIn)
    {
        NWSvcDataEvent::serializeIn(_serializerIn);

        int size = _serializerIn.getInt();
        if(size > 0)
        {
            MemBufferRef memBuffer(size);
            _serializerIn.getBuffer(memBuffer.getPtr(), memBuffer.getSize());
            mMemBuffer = memBuffer;
        }
    }

    virtual void serializeOut(MemorySerializerOut & _serializerOut) const
    {
        NWSvcDataEvent::serializeOut(_serializerOut);

        int size = mMemBuffer.getSize();
        _serializerOut.addInt(size);

        if(size > 0)
            _serializerOut.addBuffer(mMemBuffer.getPtr(), mMemBuffer.getSize());
    }
};

//****************************************************************************
//
//****************************************************************************
struct NWSvcDataEventNotificationCallback
{
    virtual void receiveEventMsg(int _eventType, MemBufferRef * _eventData)=0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class NWSvcDataEventHandler : public NWSvcDataBase
{
public:
    NWSvcDataEventHandler();
    virtual ~NWSvcDataEventHandler();

    virtual bool init(const char * _contextName, const char * _objName, NWSvcDataServer * _dataServer, bool _isServer);
    virtual void shutdown();

    virtual void addNotificationCallback(NWSvcDataEventNotificationCallback * _callback);
    virtual void removeNotificationCallback(NWSvcDataEventNotificationCallback * _callback);

    virtual void sendEvent(NWSvcDataEvent * _event);
    virtual void receiveEventMsg(int _eventType, MemBufferRef * _eventData);

protected:

private:
    typedef NWSvcDataBase Inherited;
};

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#define NWSVC_DATA_OBJ(DATAOBJTYPE, DATAOBJNAME) \
    NWSvcDataObj<DATAOBJTYPE> m##DATAOBJNAME;

#define REG_SVR_OBJ_NAMED(NWPSZ_NAME,NWCLASS_MEMBER,NWDEFAULT_VAL) \
    NWCLASS_MEMBER.init(s_currentContext,NWPSZ_NAME,s_currentSvrDataService,true); \
    s_currentSvrDataService->regObj(s_currentContext, NWPSZ_NAME, &NWCLASS_MEMBER); \
    NWCLASS_MEMBER.set(NWDEFAULT_VAL);

#define REG_SVR_OBJ(DATAOBJNAME,NWDEFAULT_VAL) \
    m##DATAOBJNAME.init(s_currentContext,#DATAOBJNAME,s_currentSvrDataService,true); \
    s_currentSvrDataService->regObj(s_currentContext, #DATAOBJNAME, &m##DATAOBJNAME); \
    m##DATAOBJNAME.set(NWDEFAULT_VAL);

#define REG_CLI_OBJ(NWPSZ_NAME,NWCLASS_MEMBER) \
    NWCLASS_MEMBER.init(s_currentContext,NWPSZ_NAME,s_currentSvrDataService,false); \
    s_currentSvrDataService->regObj(s_currentContext, NWPSZ_NAME, &NWCLASS_MEMBER);

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#define REG_SVR_OBJ_DELEGATE_AUX(NWPSZ_NAME,NWCLASS_MEMBER,NWCLASS_FNSET,NWCLASS_FNGET) \
    NWCLASS_MEMBER.init/*<NWTHIS_TYPE,NWDATA_TYPE>*/(s_currentContext,NWPSZ_NAME,s_currentSvrDataService,(NWDelegateDef<void *>::SetFnPtr)&NWCLASS_FNSET,(NWDelegateDef<void *>::GetFnPtr)&NWCLASS_FNGET,this); \
    s_currentSvrDataService->regObj(s_currentContext, NWPSZ_NAME, &NWCLASS_MEMBER);

#define REG_SVR_OBJ_DELEGATE(DATAOBJNAME) \
    REG_SVR_OBJ_DELEGATE_AUX(#DATAOBJNAME, mDelegate##DATAOBJNAME, NWSVCThisClass::setDelegate##DATAOBJNAME, NWSVCThisClass::getDelegate##DATAOBJNAME)

#define NWSVC_DATA_OBJ_DELEGATE(DATAOBJTYPE, DATAOBJNAME) \
    void setDelegate##DATAOBJNAME (DATAOBJTYPE _val); \
    DATAOBJTYPE getDelegate##DATAOBJNAME () const; \
    NWSvcDataObjDelegate<NWSVCThisClass, DATAOBJTYPE> mDelegate##DATAOBJNAME;

#define DELEGATE_SET_FN(THIS_CLASS, DATAOBJTYPE, DATAOBJNAME) \
    void THIS_CLASS::setDelegate##DATAOBJNAME (DATAOBJTYPE _val)

#define DELEGATE_GET_FN(THIS_CLASS, DATAOBJTYPE, DATAOBJNAME) \
    DATAOBJTYPE THIS_CLASS::getDelegate##DATAOBJNAME () const

// --- ---

#define NWSVC_DATA_OBJ_DELEGATE_MEMBER(MEMBER_TYPE, DATAOBJTYPE, DATAOBJNAME) \
    NWSvcDataObjDelegate<MEMBER_TYPE, DATAOBJTYPE> mDelegate##DATAOBJNAME;

#define REG_SVR_OBJ_DELEGATE_MEMBER(DATAOBJNAME, NWCLASS_FNSET, NWCLASS_FNGET, MEMBER_INSTANCE) \
    mDelegate##DATAOBJNAME.init/*<NWTHIS_TYPE,NWDATA_TYPE>*/(s_currentContext, #DATAOBJNAME, s_currentSvrDataService, (NWDelegateDef<void *>::SetFnPtr)&NWCLASS_FNSET, (NWDelegateDef<void *>::GetFnPtr)&NWCLASS_FNGET,&MEMBER_INSTANCE); \
    s_currentSvrDataService->regObj(s_currentContext, #DATAOBJNAME, &mDelegate##DATAOBJNAME);

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#define NWSVC_DATA_OBJ_RANGE(DATAOBJTYPE, DATAOBJNAME) \
    DATAOBJTYPE m##DATAOBJNAME;\
    DATAOBJTYPE m##DATAOBJNAME##Min; \
    DATAOBJTYPE m##DATAOBJNAME##Max; \
    void setDelegateRange##DATAOBJNAME (DATAOBJTYPE _val) \
    { \
        m##DATAOBJNAME = MAX(_val,m##DATAOBJNAME##Min);\
        m##DATAOBJNAME = MIN(m##DATAOBJNAME,m##DATAOBJNAME##Max);\
    } \
    DATAOBJTYPE getDelegateRange##DATAOBJNAME () const \
    { \
        return m##DATAOBJNAME;\
    } \
    NWSvcDataObjDelegate<NWSVCThisClass, DATAOBJTYPE> mDelegate##DATAOBJNAME;
    
#define REG_SVR_OBJ_RANGE(DATAOBJNAME, RANGE_MIN, RANGE_MAX, DEFAULT_VAL) \
    m##DATAOBJNAME##Min = RANGE_MIN; \
    m##DATAOBJNAME##Max = RANGE_MAX; \
    REG_SVR_OBJ_DELEGATE_AUX(#DATAOBJNAME, mDelegate##DATAOBJNAME, NWSVCThisClass::setDelegateRange##DATAOBJNAME, NWSVCThisClass::getDelegateRange##DATAOBJNAME) \
    mDelegate##DATAOBJNAME.set(DEFAULT_VAL)


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#define NWSVC_DATA_OBJ_LIST_ENUM(ENUMDATATYPE, DATAOBJNAME) \
    NWSvcDataObjDelegate<NWSVCThisClass, std::string> mDelegate##DATAOBJNAME; \
    NWSVC_DATA_OBJ(NWItemsList<std::string>, DATAOBJNAME##List); \
    PairValName<ENUMDATATYPE> m##DATAOBJNAME##Pairs; \
    ENUMDATATYPE m##DATAOBJNAME; \
    void setDelegate##DATAOBJNAME (std::string _val) \
    { \
        m##DATAOBJNAME##Pairs.getValFromName(_val.c_str(),m##DATAOBJNAME); \
    } \
    std::string getDelegate##DATAOBJNAME () const \
    { \
        return m##DATAOBJNAME##Pairs.getNameFromVal(m##DATAOBJNAME); \
    } 

#define NWSVC_DATA_OBJ_LIST(DATAOBJNAME) \
    NWSVC_DATA_OBJ_LIST_ENUM(int, DATAOBJNAME)

#define NWSVC_DATA_OBJ_LIST_DELEGATE_ENUM(ENUMDATATYPE, DATAOBJNAME) \
    NWSVC_DATA_OBJ_DELEGATE(std::string, DATAOBJNAME); \
    NWSVC_DATA_OBJ(NWItemsList<std::string>, DATAOBJNAME##List); \
    PairValName<ENUMDATATYPE> m##DATAOBJNAME##Pairs; \
    ENUMDATATYPE m##DATAOBJNAME;

#define NWSVC_DATA_OBJ_LIST_DELEGATE(DATAOBJNAME) \
    NWSVC_DATA_OBJ_LIST_DELEGATE_ENUM(int, DATAOBJNAME)


#define REG_SVR_OBJ_LIST_BEGIN_ENUM(ENUMDATATYPE, DATAOBJNAME, NWDEFAULT_VAL) \
    { \
        PairValName<ENUMDATATYPE>& s_currentDataObjPairs = m##DATAOBJNAME##Pairs; \
        const char* s_currentDataObjListName = #DATAOBJNAME"List"; \
        NWSvcDataObj<NWItemsList<std::string>>& s_currentDataObjList = m##DATAOBJNAME##List; \
        REG_SVR_OBJ_DELEGATE(DATAOBJNAME); \
        m##DATAOBJNAME = NWDEFAULT_VAL;

#define REG_SVR_OBJ_LIST_BEGIN(DATAOBJNAME, NWDEFAULT_VAL) \
    REG_SVR_OBJ_LIST_BEGIN_ENUM(int, DATAOBJNAME, NWDEFAULT_VAL)

#define REG_SVR_OBJ_LIST_ADD(PAIRVALUE, PAIRSTRING) \
        s_currentDataObjPairs.addPair(PAIRVALUE, PAIRSTRING);

#define REG_SVR_OBJ_LIST_END() \
        REG_SVR_OBJ_NAMED(s_currentDataObjListName, s_currentDataObjList, s_currentDataObjPairs.getItemsList()); \
    }


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#define REG_START(SVRPTR, NWCONTEXT) \
    { \
        \
        const char* s_currentContext = NWCONTEXT; \
        NWSvcDataServer* s_currentSvrDataService = SVRPTR; \

#define REG_END() \
    }

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#define NWSVR_OBJ(THIS_CLASS_NAME) \
    typedef THIS_CLASS_NAME NWSVCThisClass;



//****************************************************************************
//
//****************************************************************************
#define NWSVC_DATA_EVENTHANDLER(NWCLASS_MEMBER_NAME) \
    NWSvcDataEventHandler NWCLASS_MEMBER_NAME;
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#define REG_SVR_EVENTHANDLER(NWPSZ_NAME,NWCLASS_MEMBER) \
    NWCLASS_MEMBER.init(s_currentContext,NWPSZ_NAME,s_currentSvrDataService,true); \
    s_currentSvrDataService->regEventHandler(s_currentContext, NWPSZ_NAME, &NWCLASS_MEMBER);

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#define REG_CLI_EVENTHANDLER(NWPSZ_NAME,NWCLASS_MEMBER) \
    NWCLASS_MEMBER.init(s_currentContext,NWPSZ_NAME,s_currentSvrDataService,false); \
    s_currentSvrDataService->regEventHandler(s_currentContext, NWPSZ_NAME, &NWCLASS_MEMBER);

#endif // NW_SVR_DATA_OBJECT_H
