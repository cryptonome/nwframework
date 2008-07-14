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


#ifndef SVR_DATA_SERIALIZER_H
#define SVR_DATA_SERIALIZER_H

#include "MemorySerializer.h"
#include "NWIP.h"
#include "NWItemsList.h"

//****************************************************************************
// Generic Serializer : Implementation commented to get compile time errors
//                      when there isn't TypeParam specialization
//****************************************************************************
template <class TypeParam> static inline void SerializeDataIn(MemorySerializerIn & _serializerIn, TypeParam & _val);
template <class TypeParam> static inline void SerializeDataOut(MemorySerializerOut & _serializerOut, TypeParam const & _val);

//template <class TypeParam> /*static*/ inline void SerializeDataIn<TypeParam>(MemorySerializerIn & _serializerIn, TypeParam & _val);
//{
//    ASSERT(0);
//}
//
//template <class TypeParam> /*static*/ inline void SerializeDataOut<TypeParam>(MemorySerializerOut & _serializerOut, TypeParam & _val);
//{
//    ASSERT(0);
//}

//****************************************************************************
// Standard Especializations
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <> /*static*/ inline void SerializeDataIn<std::string>(MemorySerializerIn & _serializerIn, std::string & _val)
{
    _val = _serializerIn.getString();
}

template <> inline /*static*/ void SerializeDataOut<std::string>(MemorySerializerOut & _serializerOut, std::string const & _val)
{
    _serializerOut.addString(_val);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <> /*static*/ inline void SerializeDataIn<bool>(MemorySerializerIn & _serializerIn, bool & _val)
{
    _val = _serializerIn.getBool();
}

template <> inline /*static*/ void SerializeDataOut<bool>(MemorySerializerOut & _serializerOut, bool const & _val)
{
    _serializerOut.addBool(_val);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <> /*static*/ inline void SerializeDataIn<int>(MemorySerializerIn & _serializerIn, int & _val)
{
    _val = _serializerIn.getInt();
}

template <> inline /*static*/ void SerializeDataOut<int>(MemorySerializerOut & _serializerOut, int const & _val)
{
    _serializerOut.addInt(_val);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <> /*static*/ inline void SerializeDataIn<float>(MemorySerializerIn & _serializerIn, float & _val)
{
    _val = _serializerIn.getFloat();
}

template <> inline /*static*/ void SerializeDataOut<float>(MemorySerializerOut & _serializerOut, float const & _val)
{
    _serializerOut.addFloat(_val);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <> /*static*/ inline void SerializeDataIn<double>(MemorySerializerIn & _serializerIn, double & _val)
{
    _val = _serializerIn.getDouble();
}

template <> inline /*static*/ void SerializeDataOut<double>(MemorySerializerOut & _serializerOut, double const & _val)
{
    _serializerOut.addDouble(_val);
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <> /*static*/ inline void SerializeDataIn<NWIP>(MemorySerializerIn & _serializerIn, NWIP & _val)
{
    _val.a = _serializerIn.getUChar();
    _val.b = _serializerIn.getUChar();
    _val.c = _serializerIn.getUChar();
    _val.d = _serializerIn.getUChar();
}

template <> inline /*static*/ void SerializeDataOut<NWIP>(MemorySerializerOut & _serializerOut, NWIP const & _val)
{
    _serializerOut.addUChar(_val.a);
    _serializerOut.addUChar(_val.b);
    _serializerOut.addUChar(_val.c);
    _serializerOut.addUChar(_val.d);
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <> /*static*/ inline void SerializeDataIn<NWItemsList<int>>(MemorySerializerIn & _serializerIn, NWItemsList<int> & _val)
{
    int items = _serializerIn.getInt();
    for ( int i = 0 ; i < items ; ++i )
    {
        int val = _serializerIn.getInt();
        _val.addItem(val);
    }
}

template <> inline /*static*/ void SerializeDataOut<NWItemsList<int>>(MemorySerializerOut & _serializerOut, NWItemsList<int> const & _val)
{
    int items = _val.getNumItems();
    _serializerOut.addInt(items);
    for ( int i = 0 ; i < items ; ++i )
    {
        int val = _val.getItem(i);
        _serializerOut.addInt(val);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
template <> /*static*/ inline void SerializeDataIn<NWItemsList<std::string>>(MemorySerializerIn & _serializerIn, NWItemsList<std::string> & _val)
{
    int items = _serializerIn.getInt();
    for ( int i = 0 ; i < items ; ++i )
    {
        std::string val = _serializerIn.getString();
        _val.addItem(val);
    }
}

template <> inline /*static*/ void SerializeDataOut<NWItemsList<std::string>>(MemorySerializerOut & _serializerOut, NWItemsList<std::string> const & _val)
{
    int items = _val.getNumItems();
    _serializerOut.addInt(items);
    for ( int i = 0 ; i < items ; ++i )
    {
        std::string val = _val.getItem(i);
        _serializerOut.addString(val);
    }
}

#endif // SVR_DATA_SERVICE_H
