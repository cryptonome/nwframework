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
#ifndef _MSG_DEFINITIONS_H_
#define _MSG_DEFINITIONS_H_

#include "NWRtti.h"
#include "MemoryUtils.h"

#ifndef _VFINAL
    #ifndef CHECK_TYPE_ID
        #define CHECK_TYPE_ID
        #include "Log.h"
    #endif // CHECK_TYPE_ID
#endif // _VFINAL

//****************************************************************************
//  Type ID template
//****************************************************************************
typedef int TypeId;

#if defined CHECK_TYPE_ID

    struct TypePair
    {
        TypeId mTypeEnum;
        NWRttiTypeId mTypeRtti;
        TypePair * mPrev;
        TypePair(TypeId _typeEnum, NWRttiTypeId _typeIdRtti)
        {
            mTypeEnum = _typeEnum;
            mTypeRtti = _typeIdRtti;
            mPrev = 0;
        }
    };

    struct TypePairList
    {
        TypePair * mTypePairsTail;

        TypePairList()
        {
            mTypePairsTail = NULL;
        }

        ~TypePairList()
        {
            while(mTypePairsTail)
            {
                unlinkAndDestroyTail();
            }
        }

        TypePair * createPair(TypeId _typeEnum, NWRttiTypeId _typeIdRtti)
        {
            return NEW TypePair(_typeEnum, _typeIdRtti);
        }

        void linkPair(TypePair * _pair)
        {
            _pair->mPrev = mTypePairsTail;
            mTypePairsTail = _pair;
        }

        void unlinkAndDestroyTail()
        {
            TypePair * tail = mTypePairsTail;
            if(mTypePairsTail)
            {
                mTypePairsTail = mTypePairsTail->mPrev;
                DISPOSE(tail);
            }
        }
    };

    extern TypePairList gTypePairList;

    template <class T> class TypeIdCheck : public NWRtti<T>
    {
    public:
        TypeIdCheck();
        ~TypeIdCheck();
        void check(TypeId _typeId);

    private:
    };

    template <class T> TypeIdCheck<T>::TypeIdCheck()
    {
    }

    template <class T> TypeIdCheck<T>::~TypeIdCheck()
    {
    }

    template <class T> void TypeIdCheck<T>::check(TypeId _typeId)
    {
        TypeId typeEnum = _typeId;
        NWRttiTypeId typeRtti = NWRtti<T>::getTypeId();

        bool bFound = false;
        TypePair * typePair = gTypePairList.mTypePairsTail;
        while(typePair)
        {
            if(typePair->mTypeRtti != typeRtti && typePair->mTypeEnum == typeEnum)
            {
                LOG("Error : Message has invalid TypeId, EnumValue: 0x%08X RttiValue: 0x%08X", typeEnum, typeRtti);
                ASSERT(false);
            }
            else if(typePair->mTypeRtti == typeRtti && typePair->mTypeEnum == typeEnum)
            {
                bFound = true;
            }

            typePair = typePair->mPrev;
        }

        if(!bFound)
        {
            TypePair * newPair = gTypePairList.createPair(typeEnum, typeRtti);
            gTypePairList.linkPair(newPair);
        }
    }
#endif // CHECK_TYPE_ID

//****************************************************************************
//
//****************************************************************************
class NWBaseMsgInternal
{
public:
    inline NWBaseMsgInternal(){}
    inline virtual ~NWBaseMsgInternal(){}

    virtual NWBaseMsgInternal * createInstance()=0;
    virtual void cloneFrom(NWBaseMsgInternal * _from)=0;
};

template <class T, int msgType, int msgFamily=0> class NWBaseMsg : public NWBaseMsgInternal
{
public:
    NWBaseMsg();
    virtual ~NWBaseMsg();
    enum
    {
        MSG_TYPE = msgType,
        MSG_FAMILY = msgFamily
    };

    NWBaseMsg * next() const;

    virtual NWBaseMsgInternal * createInstance() {return (NWBaseMsgInternal*)NEW T;}
    virtual void cloneFrom(NWBaseMsgInternal * _from)
    {
        T * to = (T*)this;
        T * from = (T*)_from;

        to->operator =(*from);
    }

    inline TypeId getTypeId() const {return MSG_TYPE;}

private:
    friend class MsgMgr;
    u64 mMsgId;
    NWBaseMsg * mNext;

    #if defined CHECK_TYPE_ID
        //TypeIdCheck<T> mTypeIdCheck;
    #endif // CHECK_TYPE_ID
};

template <class T, int typeId, int msgFamily> NWBaseMsg<T, typeId, msgFamily>::NWBaseMsg() :
    mMsgId(0),
    mNext(NULL)
{
    #if defined CHECK_TYPE_ID
        //mTypeIdCheck.check(MSG_TYPE);
        ////LOG("MsgType Rtti Value: 0x%08X", mTypeIdCheck.getTypeIdRtti());
        ////LOG("MsgType Enum Value: 0x%08X", T::MSG_TYPE);
    #endif
}

template <class T, int TypeVal, int msgFamily> /*virtual*/ NWBaseMsg<T, TypeVal, msgFamily>::~NWBaseMsg()
{
}

//****************************************************************************
//
//****************************************************************************
#define NWMSG(T) struct T : public NWBaseMsg<T, MsgType_##T, MsgFamily_General>     // Msg with generic family
#define NWMSGF(T,F) struct T : public NWBaseMsg<T, MsgType_##T, F>   // User supplied family

#endif //_MSG_DEFINITIONS_H_
