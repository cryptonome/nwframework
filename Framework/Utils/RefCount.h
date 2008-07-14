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
