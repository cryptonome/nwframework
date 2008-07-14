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
