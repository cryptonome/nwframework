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


#include "PchUtils.h"

#include "NWSvcDataContext.h"

#include "NWDelegate.h"

#include "DataScript.h"

#include <string>
#include <list>

//****************************************************************************
//
//****************************************************************************
NWSvcDataContext::NWSvcDataContext() :
    mInitd(false),
    mName("NONAMED_CONTEXT")
{
}

/*virtual*/ NWSvcDataContext::~NWSvcDataContext()
{
    ASSERT(!mInitd);
}

//****************************************************************************
//
//****************************************************************************
bool NWSvcDataContext::init(const char * _name)
{
    bool retVal = false;

    if(!mInitd)
    {
        mName = _name;
        retVal = true;
        mInitd = true;
    }

    return retVal;
}

void NWSvcDataContext::shutdown()
{
    if(mInitd)
    {
        mInitd = false;
    }
}

//****************************************************************************
//
//****************************************************************************
void NWSvcDataContext::regObject(const char * _objName, NWSvcDataObject * _svcDataObj)
{
    ASSERT(_objName && _svcDataObj);
    mHashObjects.insert(HashObjectsPair(_objName, _svcDataObj));
}

void NWSvcDataContext::unregObject(const char * _objName, NWSvcDataObject * _svcDataObj)
{
    ASSERT(_objName && _svcDataObj);
    HashObjectsIt it = mHashObjects.find(_objName);

    ASSERT(it != mHashObjects.end());
    while(it != mHashObjects.end())
    {
        if((*it).second == _svcDataObj)
        {
            mHashObjects.erase(it);
            break;
        }

        it++;
    }
}

NWSvcDataContext::HashObjectsItRange NWSvcDataContext::findObjects(const char * _objName)
{
    return mHashObjects.equal_range(_objName);
}

//****************************************************************************
//
//****************************************************************************
void NWSvcDataContext::regEventHandler(const char * _handlerName, NWSvcDataEventHandler * _svcDataEventHandler)
{
    ASSERT(_handlerName && _svcDataEventHandler);
    mHashEventHandlers.insert(HashEventHandlersPair(_handlerName, _svcDataEventHandler));
}

void NWSvcDataContext::unregEventHandler(const char * _handlerName, NWSvcDataEventHandler * _svcDataEventHandler)
{
    ASSERT(_handlerName && _svcDataEventHandler);
    HashEventHandlersIt it = mHashEventHandlers.find(_handlerName);

    ASSERT(it != mHashEventHandlers.end());
    while(it != mHashEventHandlers.end())
    {
        if((*it).second == _svcDataEventHandler)
        {
            mHashEventHandlers.erase(it);
            break;
        }

        it++;
    }
}

NWSvcDataContext::HashEventHandlersItRange NWSvcDataContext::findEventHandlers(const char * _handlerName)
{
    return mHashEventHandlers.equal_range(_handlerName);
}
