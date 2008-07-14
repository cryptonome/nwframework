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
