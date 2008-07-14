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

#ifndef DSUTILSOBJ_H_
#define DSUTILSOBJ_H_

struct IBaseFilter;
struct IPin;

//********************************************************************
// DS_RELEASE macro
//********************************************************************
#define DS_RELEASE(var)     \
                            if ( var != 0 ) \
                            { \
                                var->Release(); \
                                var = 0; \
                            }


//********************************************************************
//
//********************************************************************
namespace DSUtils
{
    ////////////////////////
    // DSObj
    ////////////////////////
    template <typename T>
    class DSObj
    {
    public:
        DSObj(T* _obj) : mObj(_obj) { mObj->AddRef(); }
        DSObj(const DSObj<T>& _obj) { mObj = _obj.getObj(); mObj->AddRef(); }
        ~DSObj() { DS_RELEASE(mObj); }

        DSObj& operator= (const DSObj& obj)
        {
            mObj = obj.getObj();
            mObj->AddRef();
            return *this;
        }

        T* getObj() const { return mObj; }
    private:
        T* mObj;
    };
    typedef DSObj<IBaseFilter> DSFilter;
    typedef std::list<DSFilter> DSFilterList;
    typedef DSObj<IPin> DSPin;
    typedef std::list<DSPin> DSPinList;
} // DSUtils

#endif
