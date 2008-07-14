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
#ifndef _REF_COUNT_H_
#define _REF_COUNT_H_

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class RefCount
{
public:
    inline RefCount();

    inline int addRef();
    inline int release(bool _dispose=true);

    inline int getNumRefs();

protected:
    virtual inline ~RefCount();

private:
    int mNumRefs;

    
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
inline RefCount::RefCount() :
    mNumRefs(1)
{
}

/*virtual*/ inline RefCount::~RefCount()
{
    ASSERT(mNumRefs == 0);
}

inline int RefCount::addRef()
{
    mNumRefs++;

    return mNumRefs;
}

inline int RefCount::release(bool _dispose/*=true*/)
{
    ASSERT(mNumRefs > 0);

    mNumRefs--;
    if(mNumRefs == 0 && _dispose)
    {
        delete this;
        return 0;
    }

    return mNumRefs;
}

inline int RefCount::getNumRefs()
{
    return mNumRefs;
}

#endif _REF_COUNT_H_
