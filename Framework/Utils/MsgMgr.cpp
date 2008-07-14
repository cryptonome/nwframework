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

#include "MsgMgr.h"

#include "MsgMgrAux.h"
#include "CommNode.h"

#include "NWEvent.h"
#include "NWMultipleEvents.h"
#include "NWCriticalSection.h"

//****************************************************************************
//
//****************************************************************************
static const int DISPATCHER_TIMEOUT_MS = /**/300;/**/ /*NWME_INFINITE;*/ // zzz test
static const int PURGE_MAX_MSGS_PER_CHANNEL = 1000;
static const int COMM_NODE_NOTIFICATION_CALLBACK_RESERVE = 16;

//****************************************************************************
//
//****************************************************************************
/*static*/ MsgMgr * MsgMgr::mInstance = NULL;

//****************************************************************************
// MsgMgr Singleton
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*static*/ bool MsgMgr::init(MsgMgr_InitData const * _initData)
{
    if(mInstance == NULL)
    {
        mInstance = NEW MsgMgr();
        mInstance->initializeInstance(_initData);
    }

    return (mInstance != NULL);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*static*/ void MsgMgr::done()
{
    if(mInstance)
    {
        mInstance->shutdownInstance();
        DISPOSE(mInstance);
    }
}

//****************************************************************************
// MsgMgr class
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgMgr::MsgMgr() :
    mMailboxUpdateEvent(NULL),
    mThreadDispatcher(NULL),
    mMsgMgrThreadParams(),
    mChannelList(NULL),
    mMsgMgrDns(NULL),
    mCritSecAddRemoveCommNodes(NULL),
    mCritSecDns(NULL),
    mChannelIdGenerator(0),
    mNotificationCallback(NULL)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgMgr::~MsgMgr()
{
}

//****************************************************************************
// Instance Init / Shutdown
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool MsgMgr::initializeInstance(MsgMgr_InitData const * _initData)
{
    mChannelList = NEW ChannelList();
    mChannelList->initialize();

    mMailboxUpdateEvent = NWEvent::create();

    mMsgMgrThreadParams.mMailboxUpdateEvent = mMailboxUpdateEvent;

    mThreadDispatcher = NWThread::create();
    mThreadDispatcher->start(this, (void *)&mMsgMgrThreadParams);

    mCritSecAddRemoveCommNodes = NWCriticalSection::create();
    mCritSecDns = NWCriticalSection::create();

    mMsgMgrDns = NEW MsgMgr_Dns();
    mMsgMgrDns->initialize();

    return true;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgMgr::shutdownInstance()
{
    mMsgMgrDns->shutdown();
    DISPOSE(mMsgMgrDns);

    NWThread::destroy(mThreadDispatcher);

    mChannelList->shutdown();
    DISPOSE(mChannelList);
    mChannelList = 0;

    NWEvent::destroy(mMailboxUpdateEvent);

    NWCriticalSection::destroy(mCritSecAddRemoveCommNodes);
    NWCriticalSection::destroy(mCritSecDns);
}

//****************************************************************************
// Add channel / Remove channel / SendTo Channel
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelId MsgMgr::addCommNodeToChannel(const char * _channelName, CommNode * _commNode)
{
    ASSERT(_commNode);

    ChannelId channelId = InvalidChannelId;
    
    {
        NWAutoCritSec critSec(mCritSecAddRemoveCommNodes);

        channelId = findChannel(_channelName);

        if(channelId == InvalidChannelId)
        {
            MsgChannel * channel = createChannel(_channelName);
            channelId = channel->getChannelId();
        }

        addCommNodeToChannel(channelId, _commNode);
    }
    
    return channelId;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool MsgMgr::addCommNodeToChannel(ChannelId _channelId, CommNode * _commNode)
{
    ASSERT(_commNode);

    bool bRet = false;

    ASSERT(_channelId != InvalidChannelId);
    if(_channelId != InvalidChannelId)
    {
        {
            NWAutoCritSec critSec(mCritSecAddRemoveCommNodes);

            MsgChannel * channel = mChannelList->getChannel(_channelId);
            ASSERT(channel);
            if(channel)
            {
                MsgChannel * commNodeChannel = _commNode->getLocalChannel();
                channel->listenTo(commNodeChannel);
                commNodeChannel->listenTo(channel);
                bRet = true;
            }
        }
    }

    return bRet;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelId MsgMgr::addCommNodeListenerToChannel(const char * _channelName, CommNode * _commNode)
{
    ASSERT(_commNode);

    ChannelId channelId = InvalidChannelId;

    {
        NWAutoCritSec critSec(mCritSecAddRemoveCommNodes);
        
        channelId = findChannel(_channelName);

        if(channelId == InvalidChannelId)
        {
            MsgChannel * channel = createChannel(_channelName);
            channelId = channel->getChannelId();
        }

        addCommNodeListenerToChannel(channelId, _commNode);
    }

    return channelId;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool MsgMgr::addCommNodeListenerToChannel(ChannelId _channelId, CommNode * _commNode)
{
    ASSERT(_commNode);

    bool bRet = false;

    ASSERT(_channelId != InvalidChannelId);
    if(_channelId != InvalidChannelId)
    {
        {
            NWAutoCritSec critSec(mCritSecAddRemoveCommNodes);

            MsgChannel * channel = mChannelList->getChannel(_channelId);
            ASSERT(channel);
            if(channel)
            {
                MsgChannel * commNodeChannel = _commNode->getLocalChannel();
                commNodeChannel->listenTo(channel);
                bRet = true;
            }
        }
    }

    return bRet;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelId MsgMgr::addCommNodeSenderToChannel(const char * _channelName, CommNode * _commNode)
{
    ASSERT(_commNode);

    ChannelId channelId = InvalidChannelId; 
    
    {
        NWAutoCritSec critSec(mCritSecAddRemoveCommNodes);
        
        channelId = findChannel(_channelName);

        if(channelId == InvalidChannelId)
        {
            MsgChannel * channel = createChannel(_channelName);
            channelId = channel->getChannelId();
        }

        addCommNodeSenderToChannel(channelId, _commNode);
    }

    return channelId;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool MsgMgr::addCommNodeSenderToChannel(ChannelId _channelId, CommNode * _commNode)
{
    bool bRet = false;

    ASSERT(_commNode);
    ASSERT(_channelId != InvalidChannelId);
    if(_channelId != InvalidChannelId)
    {
        {
            NWAutoCritSec critSec(mCritSecAddRemoveCommNodes);

            MsgChannel * channel = mChannelList->getChannel(_channelId);
            ASSERT(channel);
            if(channel)
            {
                MsgChannel * commNodeChannel = _commNode->getLocalChannel();
                channel->listenTo(commNodeChannel);
                bRet = true;
            }
        }
    }

    return bRet;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgMgr::removeCommNodeFromChannel(const char * _channelName, CommNode * _commNode)
{
    {
        NWAutoCritSec critSec(mCritSecAddRemoveCommNodes);

        ChannelId channelId = findChannel(_channelName, false);

        if(channelId != InvalidChannelId)
        {
            removeCommNodeFromChannel(channelId, _commNode);
        }
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgMgr::removeCommNodeFromChannel(ChannelId _channelId, CommNode * _commNode)
{
    if(_channelId != InvalidChannelId)
    {
        {
            NWAutoCritSec critSec(mCritSecAddRemoveCommNodes);

            MsgChannel * channel = mChannelList->getChannel(_channelId);
            MsgChannel * commNodeChannel = _commNode->getLocalChannel();

            if(channel)
            {
                channel->stopListeningTo(commNodeChannel);
                commNodeChannel->stopListeningTo(channel);

                if(channel->getNumListeners() < 1 && channel->getNumListened() < 1)
                {
                    unregChannelName(getChannelName(_channelId));
                    mChannelList->removeChannel(_channelId);
                }
            }
        }
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgMgr::regChannelName(const char * _name, ChannelId _id)
{
    {
        NWAutoCritSec critSec(mCritSecDns);

        mMsgMgrDns->regChannelName(_name, _id);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgMgr::unregChannelName(const char * _name)
{
    {
        NWAutoCritSec critSec(mCritSecDns);

        mMsgMgrDns->unregChannelName(_name);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelId MsgMgr::getChannelId(const char * _name)
{
    ChannelId channelId = InvalidChannelId;

    {
        NWAutoCritSec critSec(mCritSecDns);

        channelId = mMsgMgrDns->getChannelId(_name);
    }

    return channelId;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
const char * MsgMgr::getChannelName(ChannelId _channelId)
{
    const char * pRet = NULL;

    {
        NWAutoCritSec critSec(mCritSecDns);

        MsgChannel * channel = mChannelList->getChannel(_channelId);
        if(channel)
        {
            pRet = channel->getName();
        }
    }

    return pRet;
}

//****************************************************************************
// Main thread CommNodes fn : This type of nodes need help for message dispatching
//****************************************************************************
void MsgMgr::regNotificationCallback(MsgMgrNotificationCallback * _notificationCallback)
{
    mNotificationCallback = _notificationCallback;
}

void MsgMgr::addCommNodeToNotificationList(CommNode * _commNode)
{
    mCommNodeNotificationCallbackList.push_back(_commNode);
}

void MsgMgr::removeCommNodeFromNotificationList(CommNode * _commNode)
{
    mCommNodeNotificationCallbackList.remove(_commNode);
}

void MsgMgr::dispatchNotificationMessages()
{
    for(NotificationIt it = mCommNodeNotificationCallbackList.begin(); it != mCommNodeNotificationCallbackList.end(); it++)
    {
        CommNode * commNode = (*it);
        commNode->dispatchAvailableMessages();
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelId MsgMgr::findChannel(const char * _channelName, bool _createIfNotExist/*=true*/)
{
    ChannelId channelId = InvalidChannelId;
    
    {
        NWAutoCritSec critSec(mCritSecDns);
        
        channelId = mChannelList->getChannelId(_channelName);
    }

    return channelId;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelId MsgMgr::generateChannelId()
{
    ChannelId channelId = InvalidChannelId;

    {
        NWAutoCritSec critSec(mCritSecAddRemoveCommNodes);

        mChannelIdGenerator++;
        channelId = mChannelIdGenerator;
    }

    return channelId;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgChannel * MsgMgr::createChannel(const char * _channelName)
{
    MsgChannel * newChannel = mChannelList->addChannel(_channelName, generateChannelId(), mMailboxUpdateEvent);

    ChannelId channelId = newChannel->getChannelId();
    regChannelName(_channelName, channelId);

    return newChannel;
}

//****************************************************************************
// Dispatcher Thread Main Fn
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ unsigned int MsgMgr::threadMain(ThreadParams const * _params)
{

    MsgMgrThreadParams * msgMgrParams = (MsgMgrThreadParams *)_params->mUserParams;

    enum eMsgThreadEvents
    {
        MTE_MAILBOX_UPDATE = 0,
        MTE_END_REQUEST
    };

    NWMultipleEvents multipleEventWait(msgMgrParams->mMailboxUpdateEvent, _params->mEventEndRequest);

    bool bLoop = true;
    while(bLoop)
    {
        int eventSignaled = multipleEventWait.waitForSignal(DISPATCHER_TIMEOUT_MS); // wait for any event signaling
        if(eventSignaled == MTE_END_REQUEST)
        {
            bLoop = false;
            continue;
        }

        if(eventSignaled == NWME_TIMEOUT)
        {
            {
            NWAutoCritSec critSec(mCritSecAddRemoveCommNodes); // avoids message processing while adding or removing nodes (or add/remove nodes while processing msgs)

            threadPurgeMessages();
            }
        }
        else if(eventSignaled == MTE_MAILBOX_UPDATE)
        {
            {
            NWAutoCritSec critSec(mCritSecAddRemoveCommNodes); // avoids message processing while adding or removing nodes (or add/remove nodes while processing msgs)

            threadMailboxUpdate();
            }
        }
    }

    return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgMgr::threadPurgeMessages()
{
    MsgChannel * channel = mChannelList->getHeadChannelList();
    while(channel)
    {
        bool signalNewMessages = false;

        u64 minMsgId = 0xffffffffffffffff;

        ChannelListener * listener = channel->getHeadListenerChannel();
        while(listener)
        {
            if(listener->mCurrentMsg)
            {
                if(minMsgId > listener->mCurrentMsg->mMsgRefId)
                    minMsgId = listener->mCurrentMsg->mMsgRefId;
            }
            listener = listener->getNext();
        }

        int numMsgs = 0;
        while(channel->mHeadMsgList && channel->mHeadMsgList->mMsgRefId < minMsgId)
        {
            channel->unlinkAndDestroyStoredMsgHead();

            //numMsgs++;
            //if(numMsgs > PURGE_MAX_MSGS_PER_CHANNEL)
            //{
            //    break;
            //}
        }

        channel = channel->getNext();
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgMgr::threadMailboxUpdate()
{
    bool bCallNotificationCallback = false;
    bool bContinue = true;

    while(bContinue)
    {
        bContinue = false;

        ChannelId msgChannelFrom = InvalidChannelId;
        MsgChannel * channel = mChannelList->getHeadChannelList();
        while(channel)
        {
            bool signalNewMessages = false;

            ListenedChannel * listened = channel->getHeadListenedChannel();
            while(listened)
            {
                if(listened->mListener->mCurrentMsg)
                {
                    int numMsgs = 0;
                    StoredMsg * msg = listened->mListener->mCurrentMsg->getNext();
                    while(msg)
                    {
                        StoredMsg * newMsg = channel->createStoredMsg();
                        newMsg->copyFrom(*msg);

                        channel->linkStoredMsg(newMsg);

                        signalNewMessages = true;

                        listened->mListener->mCurrentMsg = listened->mListener->mCurrentMsg->getNext();

                        numMsgs++;
                        if(numMsgs > NumMaxMsgsDispatched) // avoid one sender collapse the msg system
                        {
                            bContinue = true;
                            msg = NULL;
                        }
                        else
                        {
                            msg = listened->mListener->mCurrentMsg->getNext();
                        }
                    }
                }

                listened = listened->getNext();
            }

            if(signalNewMessages)
            {
                ChannelListener * listener = channel->getHeadListenerChannel();
                while(listener)
                {
                    if(listener->mEventMessagesAvailable)
                    {
                        listener->mEventMessagesAvailable->signal();
                    }
                    listener = listener->getNext();
                }
                bCallNotificationCallback = true;
            }

            channel = channel->getNext();
        }

        threadPurgeMessages(); // zzz pending : add min time between purges?
    }

    if(bCallNotificationCallback && mNotificationCallback)
    {
        mNotificationCallback->msgMgrNotification();
    }
}
