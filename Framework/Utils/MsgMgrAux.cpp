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

#include "MsgMgrAux.h"
#include "NWEvent.h"
#include "MsgTypes.h"

#include "MemoryUtils.h"

#include "Log.h"

#include <windows.h>

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
StoredMsg::StoredMsg(u64 _msgRefId) :
    mSender(0),
    mMessageType(0),
    mMessageFamily(0),
    mMsgData(NULL),
    mDataLen(0),
    mMsgRefId(_msgRefId)
{
}

StoredMsg::StoredMsg(CommNodeId _sender, int _messageType, int _messageFamily, NWBaseMsgInternal * _msgData, int _dataLen, u64 _msgRefId)
{
    mSender = _sender;
    mMessageType = _messageType;
    mMessageFamily = _messageFamily;
    mMsgData = NULL;
    if(_msgData)
    {
        mMsgData = _msgData->createInstance();
        mMsgData->cloneFrom(_msgData);
    }
    mDataLen = _dataLen;
    mMsgRefId = _msgRefId;
}

StoredMsg::~StoredMsg()
{
    DISPOSE(mMsgData);
}

void StoredMsg::copyFrom(StoredMsg const & _other)
{
    mSender = _other.mSender;
    mMessageType = _other.mMessageType;
    mMessageFamily = _other.mMessageFamily;
    mMsgData = _other.mMsgData->createInstance();
    mMsgData->cloneFrom(_other.mMsgData);
    mDataLen = _other.mDataLen;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

ChannelListener::ChannelListener(MsgChannel * _channelListener, StoredMsg * _currentMsg)
{
    mChannelListener = _channelListener;
    mChannelId = _channelListener ? _channelListener->getChannelId() : InvalidChannelId;
    mEventMessagesAvailable = _channelListener ? _channelListener->getEventMessageAvailable() : NULL;
    mCurrentMsg = _currentMsg;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ListenedChannel::ListenedChannel(MsgChannel * _channelListened, ChannelListener * _listener, NWEvent * _eventMessagesAvailable) :
    mChannelListened(_channelListened),
    mListener(_listener),
    mEventMessagesAvailable(_eventMessagesAvailable)
{
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgChannel::MsgChannel() :
    mInitd(false),
    mChannelName("INVALID"),
    mChannelId(InvalidChannelId),
    mHeadMsgList(NULL),
    mTailMsg(NULL),
    mHeadListenerDList(NULL),
    mHeadListenedDList(NULL),
    mNumListeners(0),
    mNumListened(0),
    mEventMessagesAvailable(NULL),
    mMsgRefIdSeed(0)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgChannel::~MsgChannel()
{
    shutdown();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::init(const char * _name, ChannelId _channelId, NWEvent * _eventMsgAvailable)
{
    if(!mInitd)
    {
        if(_name)
            mChannelName = _name;

        mChannelId = _channelId;
        mEventMessagesAvailable = _eventMsgAvailable;

        mInitd = true;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::shutdown()
{
    if(mInitd)
    {
        waitForListeners();

        while(mHeadListenerDList)
        {
            mHeadListenerDList->mChannelListener->stopListeningTo(this);
        }
        
        while(mHeadListenedDList)
        {
            stopListeningTo(mHeadListenedDList->mListener->mChannelListener);
        }

        while(mHeadMsgList)
        {
            unlinkAndDestroyStoredMsgHead();
        }

        mInitd = false;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::listenTo(MsgChannel * _channel)
{
    ChannelListener * listener = _channel->addListener(this);
    
    ListenedChannel * listened = createListened(_channel, listener);
    linkListened(listened);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::stopListeningTo(MsgChannel * _channel)
{
    ListenedChannel * listened = findListened(_channel);
    if(listened)
    {
        unlinkAndDestroyListened(listened);
    }

    _channel->removeListener(this);
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool MsgChannel::testMsgAvailable(ChannelId _channelId/*=InvalidChannelId*/)
{
    bool bRet = false;

    ListenedChannel * listened = mHeadListenedDList;
    while(listened)
    {
        if(_channelId == InvalidChannelId || listened->mListener->mChannelListener->getChannelId() == _channelId)
        {
            if(listened->mListener->mCurrentMsg)
            {
                if(listened->mListener->mCurrentMsg->getNext())
                {
                    bRet = true;
                    break;
                }
            }
        }

        listened = listened->getNext();
    }

    return bRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
StoredMsg * MsgChannel::getAvailableMsg(ChannelId & msgFromChannel_, ChannelId _channelId/*=InvalidChannelId*/)
{
    StoredMsg * pRet = NULL;

    ListenedChannel * listened = mHeadListenedDList;
    while(listened)
    {
        if(_channelId == InvalidChannelId || listened->mListener->mChannelListener->getChannelId() == _channelId)
        {
            if(listened->mListener->mCurrentMsg)
            {
                if(listened->mListener->mCurrentMsg->getNext())
                {
                    pRet = listened->mListener->mCurrentMsg->getNext();
                    msgFromChannel_ = listened->mListener->mChannelListener->getChannelId();
                    break;
                }
            }
        }

        listened = listened->getNext();
    }

    return pRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::nextAvailableMsg(ChannelId _channelId/*=InvalidChannelId*/)
{
    ListenedChannel * listened = mHeadListenedDList;
    while(listened)
    {
        if(_channelId == InvalidChannelId || listened->mListener->mChannelListener->getChannelId() == _channelId)
        {
            if(listened->mListener->mCurrentMsg)
            {
                if(listened->mListener->mCurrentMsg->getNext())
                {
                    listened->mListener->mCurrentMsg = listened->mListener->mCurrentMsg->getNext();
                    break;
                }
            }
        }

        listened = listened->getNext();
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::waitForListeners(ChannelId _channelId/*=InvalidChannelId*/)
{
    bool bContinue = true;

    while(bContinue)
    {
        u64 minMsgRefId = 0xffffffffffffffff;

        ChannelListener * head = mHeadListenerDList;
        while(head)
        {
            if(_channelId==InvalidChannelId || head->mChannelId == _channelId)
            {
                if(minMsgRefId > head->mCurrentMsg->mMsgRefId)
                    minMsgRefId = head->mCurrentMsg->mMsgRefId;
            }

            head = head->getNext();
        }

        if(mTailMsg && minMsgRefId < mTailMsg->mMsgRefId)
        {
            Sleep(0);
        }
        else
        {
            bContinue = false;
        }
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelListener * MsgChannel::addListener(MsgChannel * _listener)
{
    if(!mTailMsg)
    {
        mTailMsg = NEW StoredMsg(InvalidCommNodeID, MsgType_Internal, MsgFamily_Null, NULL, 0, generateMsgRefId()/*0xffffffffffffffff*/);
        mHeadMsgList = mTailMsg;
    }

    ChannelListener * listener = createListener(_listener, mTailMsg);

    if(listener)
    {
        linkListener(listener);
        mNumListeners++;
    }

    return listener;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::removeListener(MsgChannel * _listener)
{
    ChannelListener * listener = findListener(_listener);

    if(listener)
    {
        unlinkAndDestroyListener(listener);
    }
}

//****************************************************************************
//
//****************************************************************************
StoredMsg * MsgChannel::createStoredMsg()
{
    u64 refId = generateMsgRefId();
    return NEW StoredMsg(refId);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::unlinkAndDestroyStoredMsgHead()
{
    if(mHeadMsgList)
    {
        StoredMsg * msg = mHeadMsgList;
        mHeadMsgList = msg->getNext();

        if(msg == mTailMsg)
            mTailMsg = NULL;

        DISPOSE(msg);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::linkStoredMsg(StoredMsg * _msg)
{
    StoredMsg * tail = mTailMsg;

    mTailMsg = _msg;

    if(!mHeadMsgList)
        mHeadMsgList = _msg;

    if(tail)
        tail->setNext(_msg);

    signalListeners();
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelListener * MsgChannel::createListener(MsgChannel * _channelListener, StoredMsg * _currentMsg)
{
    return NEW ChannelListener(_channelListener, _currentMsg);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::unlinkAndDestroyListener(ChannelListener * _listener)
{
    if(_listener)
    {
        if(_listener == mHeadListenerDList)
            mHeadListenerDList = _listener->getNext();

        _listener->unlink();
        DISPOSE(_listener);
        mNumListeners--;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::linkListener(ChannelListener * _listener)
{
    if(mHeadListenerDList)
    {
        ChannelListener * tail = mHeadListenerDList->getTail();
        _listener->linkAsNextOf(tail);
    }
    else
    {
        mHeadListenerDList = _listener;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelListener * MsgChannel::findListener(MsgChannel * _channelListener)
{
    ChannelListener * pRet = NULL;

    ChannelListener * head = mHeadListenerDList;
    while(head)
    {
        if(head->mChannelListener == _channelListener)
        {
            pRet = head;
            break;
        }
        head = head->getNext();
    }

    return pRet;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ListenedChannel * MsgChannel::createListened(MsgChannel * _listenedChannel, ChannelListener * _listener)
{
    return NEW ListenedChannel(_listenedChannel, _listener, mEventMessagesAvailable);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::unlinkAndDestroyListened(ListenedChannel * _listened)
{
    if(_listened)
    {
        if(_listened == mHeadListenedDList)
            mHeadListenedDList = _listened->getNext();

        _listened->unlink();
        DISPOSE(_listened);

        mNumListened--;
    }

}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgChannel::linkListened(ListenedChannel * _listened)
{
        if(mHeadListenedDList)
        {
            ListenedChannel * tail = mHeadListenedDList->getTail();
            _listened->linkAsNextOf(tail);
        }
        else
        {
            mHeadListenedDList = _listened;
        }

    mNumListened++;
}

ListenedChannel * MsgChannel::findListened(MsgChannel * _listenedChannel)
{
    ListenedChannel * pRet = NULL;

    ListenedChannel * head = mHeadListenedDList;
    while(head)
    {
        if(head->mChannelListened == _listenedChannel)
        {
            pRet = head;
            break;
        }
        head = head->getNext();
    }

    return pRet;
}

u64 MsgChannel::generateMsgRefId()
{
    mMsgRefIdSeed++;
    
    return mMsgRefIdSeed;
}

void MsgChannel::signalListeners()
{
    ChannelListener * head = mHeadListenerDList;
    while(head)
    {
        //if(!head->mCurrentMsg)
        //    head->mCurrentMsg = mHeadMsgList;

        if(head->mEventMessagesAvailable)
            head->mEventMessagesAvailable->signal();

        head = head->getNext();
    }
}

void MsgChannel::purgeSentMsgs()
{
    u64 minRefId = getMinMsgRefId();
    purgeSentMsgs(minRefId);
}

void MsgChannel::purgeSentMsgs(u64 _minMsgRefId)
{
    while(mHeadMsgList && _minMsgRefId > mHeadMsgList->mMsgRefId && mHeadMsgList != mTailMsg)
    {
        unlinkAndDestroyStoredMsgHead();
    }
}

u64 MsgChannel::getMinMsgRefId()
{
    u64 minMsgRefId = 0xffffffffffffffff;

    ChannelListener * head = mHeadListenerDList;
    while(head)
    {
        if(minMsgRefId > head->mCurrentMsg->mMsgRefId)
            minMsgRefId = head->mCurrentMsg->mMsgRefId;

        head = head->getNext();
    }

    return minMsgRefId;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelList::ChannelList() : 
    mHeadChannelList(NULL),
    mTailChannel(NULL)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelList::~ChannelList()
{
    shutdown();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool ChannelList::initialize()
{
    bool bRet = false;

    if(!mInitd)
    {
        mInitd = true;
        bRet = mInitd;
    }

    return bRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void ChannelList::shutdown()
{
    if(mInitd)
    {
        while(mTailChannel)
        {
            MsgChannel * head = mHeadChannelList;
            mHeadChannelList = mHeadChannelList->getNext();

            unlinkAndDestroyChannel(head->getChannelId());

            if(head == mTailChannel)
                mTailChannel = NULL;
        }

        mInitd = false;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgChannel * ChannelList::addChannel(const char * _name, ChannelId _channelId, NWEvent * _eventMsgAvailable)
{
    MsgChannel * msgChannel = createChannel(_name, _channelId, _eventMsgAvailable);
        
    linkChannelAsTail(msgChannel);

    return msgChannel;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void ChannelList::removeChannel(ChannelId _channelId)
{
    unlinkAndDestroyChannel(_channelId);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelId ChannelList::getChannelId(const char * _channelName)
{
    ChannelId channelId = InvalidChannelId;

    std::string name = _channelName;

    MsgChannel * channel = mHeadChannelList;
    while(channel)
    {
        std::string channelName = channel->getName();
        
        if(name == channelName)
        {
            channelId = channel->getChannelId();
            break;
        }

        channel = channel->getNext();
    }

    return channelId;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgChannel * ChannelList::getChannel(const char * _channelName)
{
    MsgChannel * pRet = NULL;
    std::string name = _channelName;

    MsgChannel * channel = mHeadChannelList;
    while(channel)
    {
        std::string channelName = channel->getName();

        if(name == channelName)
        {
            pRet = channel;
            break;
        }

        channel = channel->getNext();
    }

    return pRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgChannel * ChannelList::getChannel(ChannelId _channelId)
{
    MsgChannel * pRet = NULL;

    MsgChannel * channel = mHeadChannelList;
    while(channel)
    {

        if(_channelId == channel->getChannelId())
        {
            pRet = channel;
            break;
        }

        channel = channel->getNext();
    }

    return pRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgChannel * ChannelList::createChannel(const char * _name, ChannelId _channelId, NWEvent * _eventMsgAvailable)
{
    MsgChannel * channel = NEW MsgChannel();
    
    channel->init(_name, _channelId, _eventMsgAvailable);

    return channel;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void ChannelList::linkChannelAsTail(MsgChannel * msgChannel)
{
    if(mTailChannel)
    {
        mTailChannel->setNext(msgChannel);
    }
    else
    {
        mHeadChannelList = msgChannel;
    }
    mTailChannel = msgChannel;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void ChannelList::unlinkAndDestroyChannel(ChannelId _channelId)
{
    MsgChannel * prev = NULL;
    MsgChannel * channel = mHeadChannelList;
    while(channel)
    {
        if(_channelId == channel->getChannelId())
        {
            if(prev)
                prev->setNext(channel->getNext());
            if(channel == mHeadChannelList)
                mHeadChannelList = channel->getNext();
            if(channel == mTailChannel)
                mTailChannel = channel->getNext();

            channel->shutdown();
            DISPOSE(channel);
            break;
        }

        prev = channel;
        channel = channel->getNext();
    }
}

//****************************************************************************
//
//****************************************************************************
#include <hash_map>

typedef std::pair <const char *, ChannelId> DnsPair;
typedef stdext::hash_map<const char *, ChannelId>::iterator DnsIter;

struct DnsHash
{
    stdext::hash_map <const char *, ChannelId> hash;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgMgr_Dns::MsgMgr_Dns() :
    mDnsHash(NULL)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgMgr_Dns::~MsgMgr_Dns()
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool MsgMgr_Dns::initialize()
{
    mDnsHash = NEW DnsHash();

    return true;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgMgr_Dns::shutdown()
{
    if(mDnsHash)
    {
        DISPOSE(mDnsHash);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MsgMgr_Dns::regChannelName(const char * _name, ChannelId _id)
{
    mDnsHash->hash.insert(DnsPair(_name, _id));
}

void MsgMgr_Dns::unregChannelName(const char * _name)
{
    mDnsHash->hash.erase(_name);

    ASSERT(getChannelId(_name) == InvalidChannelId);
}

ChannelId MsgMgr_Dns::getChannelId(const char * _name)
{
    ChannelId channelId = InvalidChannelId;

    DnsIter it = mDnsHash->hash.find(_name);
    if(it != mDnsHash->hash.end())
    {
        channelId = (*it).second;
    }

    return channelId;
}
