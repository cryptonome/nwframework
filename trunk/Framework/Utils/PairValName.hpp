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


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
PairValName<T>::PairValName()
{
    mPairs.clear();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
PairValName<T>::~PairValName()
{
    clearPairs();
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void PairValName<T>::addPair(const T val, const char* _name)
{
    ASSERT(getNameFromVal(val,false) == 0); T check;
    ASSERT(!getValFromName(_name,check)); check;

    Pair* pair = NEW Pair;
    pair->name = _name;
    pair->val = val;
    mPairs.push_back(pair);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void PairValName<T>::clearPairs()
{
    for ( std::vector<Pair*>::iterator it = mPairs.begin() ; it != mPairs.end() ; ++it )
    {
        Pair* pair = *it;
        DISPOSE(pair);
    }
    mPairs.clear();
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
bool PairValName<T>::getValFromName(const char* _name, T& val_) const
{
    bool found = false;

    for ( std::vector<Pair*>::const_iterator it = mPairs.begin() ; !found && it != mPairs.end() ; ++it )
    {
        const Pair* pair = *it;
        if ( strcmp(_name,pair->name.c_str()) == 0 )
        {
            found = true;
            val_ = pair->val;
        }
    }

    return found;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
const char* PairValName<T>::getNameFromVal(const T& _val, bool _mustExist) const
{
    const char* str = 0;

    for ( std::vector<Pair*>::const_iterator it = mPairs.begin() ; str == 0 && it != mPairs.end() ; ++it )
    {
        const Pair* pair = *it;
        if ( _val == pair->val )
            str = pair->name.c_str();
    }

    ASSERT(str != 0 || !_mustExist);

    return str;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
T PairValName<T>::getPairVal(int _index) const
{
    ASSERT(_index >= 0 && _index < getNumPairs());
    return mPairs[_index]->val;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
const char* PairValName<T>::getPairName(int _index) const
{
    ASSERT(_index >= 0 && _index < getNumPairs());
    return mPairs[_index]->name.c_str();
}

