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
