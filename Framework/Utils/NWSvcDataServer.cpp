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

#include "NWSvcDataServer.h"

#include "NWSvcDataContext.h"
#include "NWSvcDataObject.h"
#include "NWDelegate.h"

#include "NWCliSrvMsgs.h"

#include "MemorySerializer.h"

#include "NWCliSrvMsgs.h"

#include "CommNode.h"

#include <string>
#include <list>

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

NWSvcDataServer::NWSvcDataServer() :
    mInitd(false),
    mCommNode(NULL)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWSvcDataServer::~NWSvcDataServer()
{
    shutdown();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool NWSvcDataServer::init(const char * _name, CommNodeId _commId)
{
    bool retVal = false;

    if(!mInitd)
    {
        retVal = true;

        mCommNode = NEW CommNode();
        
        retVal &= mCommNode->init(_commId, _name);
        
        ASSERT(retVal);
        if(retVal)
        {
            mCommNode->setReceiveAllMessages(true);
            mCommNode->addToNotificationList();
            mCommNode->joinChannel(SERVER_DATA_SERVICE_CHANNEL);
            mCommNode->addMessageReceiverCallback(this);
        }
        
        mInitd = true;
    }

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWSvcDataServer::shutdown()
{
    if(mInitd)
    {
        mCommNode->done();
        DISPOSE(mCommNode);

        ASSERT(mHashContext.empty());
        mInitd = false;
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWSvcDataServer::regObj(const char * _context, const char * _name, NWSvcDataObject * _svcDataObj)
{
    NWSvcDataContext & context = mHashContext[_context];

    if(!context.isOk())
    {
        context.init(_context);
    }

    context.regObject(_name, _svcDataObj);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWSvcDataServer::unregSvrObj(const char * _context, const char * _name, NWSvcDataObject * _svcDataObj)
{
    HashContextIt it = mHashContext.find(_context);
    if(it != mHashContext.end())
    {
        NWSvcDataContext & context = (*it).second;

        context.unregObject(_name, _svcDataObj);

        if(context.getNumObjects() == 0 && context.getNumEventHandlers() == 0)
        {
            std::string contextName = context.getName();
            context.shutdown();
            mHashContext.erase(contextName);
        }
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWSvcDataServer::regEventHandler(const char * _context, const char * _name, NWSvcDataEventHandler * _svcEventHandler)
{
    NWSvcDataContext & context = mHashContext[_context];

    if(!context.isOk())
    {
        context.init(_context);
    }

    context.regEventHandler(_name, _svcEventHandler);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWSvcDataServer::unregEventHandler(const char * _context, const char * _name, NWSvcDataEventHandler * _svcEventHandler)
{
    HashContextIt it = mHashContext.find(_context);
    if(it != mHashContext.end())
    {
        NWSvcDataContext & context = (*it).second;

        context.unregEventHandler(_name, _svcEventHandler);

        if(context.getNumObjects() == 0 && context.getNumEventHandlers() == 0)
        {
            std::string contextName = context.getName();
            context.shutdown();
            mHashContext.erase(contextName);
        }
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWSvcDataServer::sendEvent(const char * _context, const char * _objName, int _eventType, MemBufferRef & _eventData, bool _clientMsg)
{
    MsgSvcEvent msg;
    msg.mEventType = _eventType;
    msg.mContext = _context;
    msg.mObjName = _objName;
    msg.mMemBuffer = _eventData;
    msg.mServerMsg = _clientMsg;

    mCommNode->sendMessage(msg);
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
// Client to Server: ClientObj asks for update
//----------------------------------------------------------------------------
void NWSvcDataServer::requestUpdate(NWSvcDataObject * _clientObj)
{
    MsgClientUpdateReq msg;
    msg.mContext = _clientObj->getContextName();
    msg.mObjName = _clientObj->getObjName();

    mCommNode->sendMessage(msg);
}

//----------------------------------------------------------------------------
// Server to Client : ServerObj has been actualized
//----------------------------------------------------------------------------
void NWSvcDataServer::updateClients(NWSvcDataObject * _serverObj)
{
    MemorySerializerOut data;
    _serverObj->serializeOut(data);
    data.finalize();

    MemBufferRef memBuffer = data.getMemBuffer();

    MsgServerUpdateClient msg;
    msg.mContext = _serverObj->getContextName();
    msg.mObjName = _serverObj->getObjName();
    msg.mMemBuffer = memBuffer;

    mCommNode->sendMessage(msg);
}

//----------------------------------------------------------------------------
// Client to Server : ClientObj requests an actualization
//----------------------------------------------------------------------------
void NWSvcDataServer::sendSetValue(NWSvcDataObject * _clientObj, MemorySerializerOut & _data)
{
    MemBufferRef memBuffer = _data.getMemBuffer();

    MsgClientSetValue msg;
    msg.mContext = _clientObj->getContextName();
    msg.mObjName = _clientObj->getObjName();
    msg.mMemBuffer = memBuffer;

    mCommNode->sendMessage(msg);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWSvcDataServer::updateObj(NWSvcDataObject * _obj, MemorySerializerIn & _data)
{
    _obj->serializeIn(_data);
}

//****************************************************************************
//
//****************************************************************************
/*virtual*/ void NWSvcDataServer::receiveMessage(CommNodeId _from, int _msgFamily, int _msgType, void * _msg, int _msgSize)
{
    if(_msgFamily == MsgFamily_CliSrvDataService)
    {
        switch(_msgType)
        {
            case MsgType_MsgClientUpdateReq: // Cli->Svr : A client wants be updated
            {
                MsgClientUpdateReq * msg = (MsgClientUpdateReq *)_msg;
                if(msg)
                {
                    NWSvcDataContext * context = findContext(msg->mContext);
                    if(context)
                    {
                        NWSvcDataContext::HashObjectsItRange itr = context->findObjects(msg->mObjName.c_str());
                        for(NWSvcDataContext::HashObjectsIt it = itr.first; it != itr.second; ++it)
                        {
                            NWSvcDataObject * obj = it->second;
                            if(obj && obj->isServer())
                            {
                                ASSERT(strcmp(obj->getContextName(),msg->mContext.c_str()) == 0);
                                ASSERT(strcmp(obj->getObjName(),msg->mObjName.c_str()) == 0);
                                updateClients(obj);
                            }
                        }
                    }
                }
                break;
            }

            case MsgType_MsgServerUpdateClient: // Srv->Cli : Server answer to update client
            {
                MsgServerUpdateClient * msg = (MsgServerUpdateClient *)_msg;
                if(msg)
                {
                    NWSvcDataContext * context = findContext(msg->mContext);
                    if(context)
                    {
                        NWSvcDataContext::HashObjectsItRange itr = context->findObjects(msg->mObjName.c_str());
                        for(NWSvcDataContext::HashObjectsIt it = itr.first; it != itr.second; ++it)
                        {
                            NWSvcDataObject * obj = it->second;
                            if(obj && !obj->isServer())
                            {
                                ASSERT(strcmp(obj->getContextName(),msg->mContext.c_str()) == 0);
                                ASSERT(strcmp(obj->getObjName(),msg->mObjName.c_str()) == 0);
                                MemorySerializerIn serializerIn;
                                serializerIn.setBuffer(msg->mMemBuffer.getPtr(), msg->mMemBuffer.getSize());

                                updateObj(obj, serializerIn);
                            }
                        }
                    }
                }
                break;
            }

            case MsgType_MsgClientSetValue: // Cli->Srv : Client wants set a value
            {
                MsgClientSetValue * msg = (MsgClientSetValue *)_msg;
                if(msg)
                {
                    NWSvcDataContext * context = findContext(msg->mContext);
                    if(context)
                    {
                        NWSvcDataContext::HashObjectsItRange itr = context->findObjects(msg->mObjName.c_str());
                        for(NWSvcDataContext::HashObjectsIt it = itr.first; it != itr.second; ++it)
                        {
                            NWSvcDataObject * obj = it->second;
                            if(obj && obj->isServer())
                            {
                                ASSERT(strcmp(obj->getContextName(),msg->mContext.c_str()) == 0);
                                ASSERT(strcmp(obj->getObjName(),msg->mObjName.c_str()) == 0);
                                MemorySerializerIn serializerIn;
                                serializerIn.setBuffer(msg->mMemBuffer.getPtr(), msg->mMemBuffer.getSize());

                                updateObj(obj, serializerIn);
                            }
                        }
                    }
                }
                break;
            }

            case MsgType_MsgSvcEvent:
            {
                MsgSvcEvent * msg = (MsgSvcEvent*)_msg;
                if(msg)
                {
                    NWSvcDataContext * context = findContext(msg->mContext);
                    if(context)
                    {
                        NWSvcDataContext::HashEventHandlersItRange itr = context->findEventHandlers(msg->mObjName.c_str());
                        for(NWSvcDataContext::HashEventHandlersIt it = itr.first; it != itr.second; ++it)
                        {
                            NWSvcDataEventHandler * eventHandler = it->second;
                            if(eventHandler && ((eventHandler->isServer() && msg->mServerMsg) || (!eventHandler->isServer() && !msg->mServerMsg)))
                            {
                                ASSERT(strcmp(eventHandler->getContextName(),msg->mContext.c_str()) == 0);
                                ASSERT(strcmp(eventHandler->getObjName(),msg->mObjName.c_str()) == 0);
                                eventHandler->receiveEventMsg(msg->mEventType, &msg->mMemBuffer);
                            }
                        }
                    }
                }
                break;
            };
        }
    }
}

NWSvcDataContext * NWSvcDataServer::findContext(std::string _contextName)
{
    NWSvcDataContext * retVal = NULL;

    HashContextIt it = mHashContext.find(_contextName);
    if(it != mHashContext.end())
    {
        retVal = &((*it).second);
    }

    return retVal;
}
