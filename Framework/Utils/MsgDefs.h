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
