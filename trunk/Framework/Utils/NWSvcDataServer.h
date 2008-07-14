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
#ifndef NW_SVC_DATA_SERVER_H
#define NW_SVC_DATA_SERVER_H

#include "NWDelegate.h"
#include "NWRtti.h"
#include "CommNode.h"

#include <string>
#include <map>

class NWSvcDataObject;
class NWSvcDataEventHandler;
class MemorySerializerIn;
class MemorySerializerOut;
class NWSvcDataContext;
class NWSvcDataEvent;
class MemBufferRef;

//****************************************************************************
//
//****************************************************************************
#define SERVER_DATA_SERVICE_CHANNEL "SVR_DATA_SERVICE_CHANNEL"

typedef NWSvcDataEvent * (*EventCreationFn)();

//****************************************************************************
//
//****************************************************************************
class NWSvcDataServer : public MsgReceiverCallback
{
public:
    NWSvcDataServer();
    ~NWSvcDataServer();

    bool init(const char * _name, CommNodeId _commId);
    void shutdown();

    void regObj(const char * _context, const char * _name, NWSvcDataObject * _svcDataObj);
    void unregSvrObj(const char * _context, const char * _name, NWSvcDataObject * _svcDataObj);

    void regEventHandler(const char * _context, const char * _name, NWSvcDataEventHandler * _svcEventHandler);
    void unregEventHandler(const char * _context, const char * _name, NWSvcDataEventHandler * _svcEventHandler);

    void sendEvent(const char * _context, const char * _objName, int _eventType, MemBufferRef & _eventData, bool _clientMsg);

    void requestUpdate(NWSvcDataObject * _clientObj);
    void updateClients(NWSvcDataObject * _serverObj);
    void sendSetValue(NWSvcDataObject * _clientObj, MemorySerializerOut & _data);
    void updateObj(NWSvcDataObject * _obj, MemorySerializerIn & _data);

    virtual void receiveMessage(CommNodeId _from, int _msgFamily, int _msgType, void * _msg, int _msgSize);

private:
    bool mInitd;
    std::map<std::string, NWSvcDataContext> mHashContext;
    typedef std::map<std::string, NWSvcDataContext>::iterator HashContextIt;
    CommNode * mCommNode;

    NWSvcDataContext * findContext(std::string _contextName);
};

#endif // NW_SVC_DATA_SERVER_H
