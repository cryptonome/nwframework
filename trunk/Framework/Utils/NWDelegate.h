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
//****************************************************************************
// Delegate : Template to allow class member calls from other classes by the
// use of a member class pointer.
// WARNING : DOES NOT WORK WITH MULTIPLE INHERITANCE PARENT INSTANCES
//****************************************************************************

#ifndef _NW_DELEGATE_H_
#define _NW_DELEGATE_H_

template <class TypeParam>
class NWDelegateBase
{
public:
    NWDelegateBase(){}
    virtual ~NWDelegateBase(){}

    virtual void set(TypeParam _val){ASSERT(false);}
    virtual TypeParam get(){ASSERT(false); return false;}

private:
};

template <class T, class TypeParam>
class NWDelegate : public NWDelegateBase<TypeParam>
{
public:
    typedef void (T::*SetFnPtr)(TypeParam _val);
    typedef TypeParam (T::*GetFnPtr)();

    NWDelegate(T * _this, SetFnPtr, GetFnPtr);
    virtual ~NWDelegate();

    virtual void set(TypeParam _val);
    virtual TypeParam get();

private:
    T * mThis;
    SetFnPtr mSetFnPtr;
    GetFnPtr mGetFnPtr;
};

template<class T, class TypeParam>
NWDelegate<T, TypeParam>::NWDelegate(T * _this, SetFnPtr _setFn, GetFnPtr _getFn) :
    mThis(_this),
    mSetFnPtr(_setFn),
    mGetFnPtr(_getFn)
{
}

template<class T, class TypeParam>
NWDelegate<T, TypeParam>::~NWDelegate()
{
    mThis = NULL;
    mSetFnPtr = NULL;
    mGetFnPtr = NULL;
}

template<class T, class TypeParam>
/*virtual*/ void NWDelegate<T, TypeParam>::set(TypeParam _val)
{
    (mThis->*mSetFnPtr)(_val);
}

template<class T, class TypeParam>
/*virtual*/ TypeParam NWDelegate<T, TypeParam>::get()
{
    return (mThis->*mGetFnPtr)();
}


//****************************************************************************
//
//****************************************************************************
template <class TypeParam>
class NWDelegateDef
{
public:
    typedef void (NWDelegateDef::*SetFnPtr)(TypeParam _val);
    typedef TypeParam (NWDelegateDef::*GetFnPtr)();
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <class T>
class NWDelegateCaller
{
public:
    NWDelegateCaller(T * _instance) :
        mInstance(_instance)
    {
    }

    ~NWDelegateCaller()
    {
        mInstance = NULL;
    }

    template <class TypeParam>
    void set(typename NWDelegateDef<TypeParam>::SetFnPtr _fnSet, TypeParam _val)
    {
        typedef void (T::*SetGenericInvoker)(TypeParam _val);
    
        SetGenericInvoker fn = (SetGenericInvoker)_fnSet;
        (mInstance->*fn)(_val);
    }

    template <class TypeParam>
    TypeParam get(typename NWDelegateDef<TypeParam>::GetFnPtr _fnGet)
    {
        typedef TypeParam (T::*GetGenericInvoker)();

        GetGenericInvoker fn = (GetGenericInvoker) _fnGet;
        return (mInstance->*fn)();
    }

protected:
    T * mInstance;
};

#endif //_NW_DELEGATE_H_

