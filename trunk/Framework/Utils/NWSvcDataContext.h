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
#ifndef NW_SVR_DATA_CONTEXT_H
#define NW_SVR_DATA_CONTEXT_H

//#include "NWDelegate.h"
//#include "NWSvcDataServer.h"

#include <map>
#include <string>

class NWSvcDataObject;
class NWSvcDataEventHandler;

//****************************************************************************
//
//****************************************************************************
class NWSvcDataContext
{
public:
    typedef std::multimap<std::string, NWSvcDataObject *>::iterator HashObjectsIt;
    typedef std::pair<HashObjectsIt, HashObjectsIt> HashObjectsItRange;

    typedef std::multimap<std::string, NWSvcDataEventHandler *>::iterator HashEventHandlersIt;
    typedef std::pair<HashEventHandlersIt, HashEventHandlersIt> HashEventHandlersItRange;

    NWSvcDataContext();
    virtual ~NWSvcDataContext();

    bool init(const char * _name);
    void shutdown();
    inline bool isOk();
        
    void regObject(const char * _objName, NWSvcDataObject * _svcDataObj);
    void unregObject(const char * _objName, NWSvcDataObject * _svcDataObj);
    HashObjectsItRange findObjects(const char * _objName);

    void regEventHandler(const char * _handlerName, NWSvcDataEventHandler * _svcDataEventHandler);
    void unregEventHandler(const char * _handlerName, NWSvcDataEventHandler * _svcDataEventHandler);
    HashEventHandlersItRange findEventHandlers(const char * _handlerName);

    inline int getNumObjects(){return (int)mHashObjects.size();}
    inline int getNumEventHandlers(){return (int)mHashEventHandlers.size();}
    inline std::string getName();

private:
    bool mInitd;
    std::string mName;

    std::multimap<std::string, NWSvcDataObject *> mHashObjects;
    typedef std::multimap<std::string, NWSvcDataObject *>::value_type HashObjectsPair;
    std::multimap<std::string, NWSvcDataEventHandler *> mHashEventHandlers;
    typedef std::multimap<std::string, NWSvcDataEventHandler *>::value_type HashEventHandlersPair;
};

inline bool NWSvcDataContext::isOk()
{
    return mInitd;
}

inline std::string NWSvcDataContext::getName()
{
    return mName;
}

#endif // NW_SVR_DATA_CONTEXT_H
