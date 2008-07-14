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


#ifndef _INCREW_CRITICAL_SECTION_H_
#define _INCREW_CRITICAL_SECTION_H_

class NWCriticalSection
{
public:
    virtual void enter() = 0;
    virtual void leave() = 0;

    static NWCriticalSection * create();
    static void destroy(NWCriticalSection* & _cs);

protected:
    NWCriticalSection(){}
    virtual ~NWCriticalSection(){}
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class NWAutoCritSec
{
public:
    inline NWAutoCritSec(NWCriticalSection * _critSec);
    inline ~NWAutoCritSec();

private:
    NWCriticalSection * mCritSec;
};

inline NWAutoCritSec::NWAutoCritSec(NWCriticalSection * _critSec)
{
    mCritSec = _critSec;
    if(mCritSec)
        mCritSec->enter();
}

inline NWAutoCritSec::~NWAutoCritSec()
{
    if(mCritSec)
        mCritSec->leave();
    mCritSec = 0;
}

#endif // _INCREW_CRITICAL_SECTION_H_
