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

#include "CommNode.h"
#include "MsgMgr.h"
#include "NWEvent.h"

#include <vector>

//****************************************************************************
//
//****************************************************************************
static const char * NON_CONNECTED_CHANNEL_NAME = "Disconected";

//****************************************************************************
//
//****************************************************************************
struct SuscribedChannel
{
    ChannelId mChannelID;
    std::string mChannelName;
    int mNodeType;

    SuscribedChannel();
    SuscribedChannel(ChannelId _channelID, const char * _name, int _nodeType);
};

SuscribedChannel::SuscribedChannel()
{
    mChannelID = InvalidChannelId;
}

SuscribedChannel::SuscribedChannel(ChannelId _channelID, const char * _name, int _nodeType)
{
    mChannelID = _channelID;
    mChannelName = _name;
    mNodeType = _nodeType;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
struct SuscribedChannels
{
    std::vector<SuscribedChannel> vec;
    
    SuscribedChannels(int _reserve);
    ~SuscribedChannels();
};

SuscribedChannels::SuscribedChannels(int _reserve)
{
    vec.reserve(_reserve);
}

SuscribedChannels::~SuscribedChannels()
{
    ASSERT(vec.size() == 0);
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
struct MsgReceiverCallbackNode
{
    ChannelId mChannelId;
    MsgReceiverCallback * mMsgReceiverCallback;

    MsgReceiverCallbackNode();
    MsgReceiverCallbackNode(ChannelId _channelId, MsgReceiverCallback * _msgReceiverCallback);
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgReceiverCallbackNode::MsgReceiverCallbackNode()
{
    mChannelId = InvalidChannelId;
    mMsgReceiverCallback = NULL;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
MsgReceiverCallbackNode::MsgReceiverCallbackNode(ChannelId _channelId, MsgReceiverCallback * _msgReceiverCallback)
{
    mChannelId = _channelId;
    mMsgReceiverCallback = _msgReceiverCallback;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
struct MsgReceiverCallbackList
{
    std::vector<MsgReceiverCallbackNode> mVec;

    MsgReceiverCallbackList(int _reserve);
};

MsgReceiverCallbackList::MsgReceiverCallbackList(int _reserve)
{
    mVec.reserve(_reserve);
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
CommNode::CommNode() :
    mInitd(false),
    mSuscribedChannelsList(NULL),
    mEventMsgAvailable(NULL),
    mEventOwner(false),
    mLocalChannel(NULL),
    mSentMsgs(0),
    mAddedNotificationList(false),
    mReceiveAllMessages(false)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ CommNode::~CommNode()
{
    done();
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool CommNode::init(CommNodeId _id, const char * _name/*=NULL*/, int _maxChannels/*=16*/, int _maxCallbacks/*=32*/, NWEvent * _eventMsgAvailable/*=NULL*/)
{
    bool bRet = false;

    if(!mInitd)
    {
        mCommNodeId = _id;
        
        mSuscribedChannelsList = NEW SuscribedChannels(_maxChannels);
        mMsgReceiverCallbackList = NEW MsgReceiverCallbackList(_maxCallbacks);

        if(_eventMsgAvailable)
        {
            mEventMsgAvailable = _eventMsgAvailable;
        }
        else
        {
            mEventMsgAvailable = NWEvent::create();
            mEventOwner = true;
        }

        mLocalChannel = NEW MsgChannel();
        mLocalChannel->init(_name, InvalidChannelId, mEventMsgAvailable);

        mSentMsgs = 0;

        mInitd = true;
        bRet = true;
    }

    return bRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CommNode::done()
{
    if(mInitd)
    {
        if(mEventOwner)
        {
            NWEvent::destroy(mEventMsgAvailable);
            mEventOwner = false;
        }

        DISPOSE(mMsgReceiverCallbackList);

        exitAllChannels();
        DISPOSE(mSuscribedChannelsList);

        mLocalChannel->shutdown();
        DISPOSE(mLocalChannel);

        mInitd = false;
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelId CommNode::joinChannel(const char * _channelName, int _type/*=COMM_NODE_BIDIR*/)
{
    ChannelId channelId = InvalidChannelId;

    switch(_type)
    {
        case COMM_NODE_BIDIR:
        {
            channelId = MsgMgr::instance()->addCommNodeToChannel(_channelName, this);
            break;
        }

        case COMM_NODE_LISTENER:
        {
            channelId = MsgMgr::instance()->addCommNodeListenerToChannel(_channelName, this);
            break;
        }

        case COMM_NODE_SENDER:
        {
            channelId = MsgMgr::instance()->addCommNodeSenderToChannel(_channelName, this);
            break;
        }
    }
        

    if(channelId != InvalidChannelId)
    {
        SuscribedChannel suscribedChannel(channelId, _channelName, _type);
        mSuscribedChannelsList->vec.push_back(suscribedChannel);
    }

    return channelId;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool CommNode::joinChannel(ChannelId _channelId, int _type/*=COMM_NODE_BIDIR*/)
{
    bool bRet = false;
        
    if(_channelId != InvalidChannelId)
    {
        switch(_type)
        {
            case COMM_NODE_BIDIR:
            {
                bRet = MsgMgr::instance()->addCommNodeToChannel(_channelId, this);
                break;
            }

            case COMM_NODE_LISTENER:
            {
                bRet = MsgMgr::instance()->addCommNodeListenerToChannel(_channelId, this);
                break;
            }

            case COMM_NODE_SENDER:
            {
                bRet = MsgMgr::instance()->addCommNodeSenderToChannel(_channelId, this);
                break;
            }
        }

        if(bRet)
        {
            const char * channelName = MsgMgr::instance()->getChannelName(_channelId);

            SuscribedChannel suscribedChannel(_channelId, channelName, _type);
            mSuscribedChannelsList->vec.push_back(suscribedChannel);
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
void CommNode::exitChannelNth(int _connectedChannelNth)
{
    int num = (int)mSuscribedChannelsList->vec.size();

    if(_connectedChannelNth < num)
    {
        SuscribedChannel * suscribedChannel = &mSuscribedChannelsList->vec[_connectedChannelNth];

        if(suscribedChannel)
        {
            mLocalChannel->waitForListeners(suscribedChannel->mChannelID);

            MsgMgr::instance()->removeCommNodeFromChannel(suscribedChannel->mChannelID, this);
        }

        mSuscribedChannelsList->vec.erase(mSuscribedChannelsList->vec.begin() + _connectedChannelNth);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CommNode::exitChannel(const char * _channelName)
{
    std::string name = _channelName;
    int num = (int)mSuscribedChannelsList->vec.size();

    for(int i=0; i<num; i++)
    {
        SuscribedChannel * suscribedChannel = &mSuscribedChannelsList->vec[i];
        if(name == suscribedChannel->mChannelName)
        {
            mLocalChannel->waitForListeners(suscribedChannel->mChannelID);

            MsgMgr::instance()->removeCommNodeFromChannel(suscribedChannel->mChannelID, this);

            mSuscribedChannelsList->vec.erase(mSuscribedChannelsList->vec.begin() + i);
        }
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CommNode::exitChannel(ChannelId _channelId)
{
    if(_channelId != InvalidChannelId)
    {
        int num = (int)mSuscribedChannelsList->vec.size();

        for(int i=0; i<num; i++)
        {
            SuscribedChannel * suscribedChannel = &mSuscribedChannelsList->vec[i];
            if(_channelId == suscribedChannel->mChannelID)
            {
                mLocalChannel->waitForListeners(_channelId);

                MsgMgr::instance()->removeCommNodeFromChannel(suscribedChannel->mChannelID, this);

                mSuscribedChannelsList->vec.erase(mSuscribedChannelsList->vec.begin() + i);
            }
        }
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CommNode::exitAllChannels()
{
    mLocalChannel->waitForListeners();

    int num = (int)mSuscribedChannelsList->vec.size();

    for(int i=num-1; i>=0; i--)
    {
        SuscribedChannel * suscribedChannel = &mSuscribedChannelsList->vec[i];
        MsgMgr::instance()->removeCommNodeFromChannel(suscribedChannel->mChannelID, this);
        mSuscribedChannelsList->vec.erase(mSuscribedChannelsList->vec.begin() + i);
    }

    if(mAddedNotificationList)
    {
        MsgMgr::instance()->removeCommNodeFromNotificationList(this);
        mAddedNotificationList = false;
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool CommNode::waitMessage(int _timeOutMs/*=COMM_NODE_WAIT_INFINITE*/)
{
    bool bRet = false;

    if(mEventMsgAvailable)
    {
        bRet = mEventMsgAvailable->waitForSignal((_timeOutMs != COMM_NODE_WAIT_INFINITE) ? _timeOutMs : NWE_INFINITE);
    }

    return bRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool CommNode::testMsgAvailable(ChannelId _channelId/*=InvalidChannelId*/)
{
    bool bRet = false;

    if(mLocalChannel->testMsgAvailable(_channelId))
    {
        bRet = true;
    }

    return true;
}

bool CommNode::getAvailableMsg(int & msgFamily_, int & msgType_, void * & msg_, int & msgSize_, ChannelId & msgSenderChannel_, ChannelId _checkChannelId/*=InvalidChannelId*/)
{
    bool bRet = false;

    StoredMsg * msg = mLocalChannel->getAvailableMsg(msgSenderChannel_, _checkChannelId);
    if(msg)
    {
        msgFamily_ = msg->mMessageFamily;
        msgType_ = msg->mMessageType;
        msg_ = msg->mMsgData;
        msgSize_ = msg->mDataLen;
        mLocalChannel->nextAvailableMsg();
        bRet = true;
    }

    return bRet;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CommNode::addMessageReceiverCallback(MsgReceiverCallback * _receiver, ChannelId _channelId/*=InvalidChannelId*/)
{
    MsgReceiverCallbackNode msgReceiverCallbackNode(_channelId, _receiver);
    mMsgReceiverCallbackList->mVec.push_back(msgReceiverCallbackNode);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CommNode::removeMessageReceiverCallback(MsgReceiverCallback * _receiver, ChannelId _channelId/*=InvalidChannelId*/)
{
    int num = (int)mMsgReceiverCallbackList->mVec.size();

    for(int i=0; i<num; i++)
    {
        if(mMsgReceiverCallbackList->mVec[i].mMsgReceiverCallback == _receiver && (_channelId == InvalidChannelId || mMsgReceiverCallbackList->mVec[i].mChannelId == _channelId))
        {
            mMsgReceiverCallbackList->mVec.erase(mMsgReceiverCallbackList->mVec.begin() + i);
        }
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CommNode::dispatchAvailableMessages(ChannelId _channelId/*=InvalidChannelId*/)
{
    ChannelId msgFromChannel = InvalidChannelId;
    StoredMsg * msg = mLocalChannel->getAvailableMsg(_channelId, msgFromChannel);

    while(msg)
    {
        if(mReceiveAllMessages || msg->mSender != mCommNodeId) // to avoid the reception of self sent msgs // zzz check if can be checked at server level
        {
            dispatchMsg(msg, msgFromChannel);
        }

        mLocalChannel->nextAvailableMsg();
        msg = mLocalChannel->getAvailableMsg(_channelId, msgFromChannel);
    }

    mLocalChannel->purgeSentMsgs();
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CommNode::getNumConnectedChannels() const
{
    return (int)mSuscribedChannelsList->vec.size();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
const char * CommNode::getConnectedChannelName(int _n)
{
    const char * pRet = NULL;

    int num = (int)mSuscribedChannelsList->vec.size();

    if(_n < num)
    {
        pRet = mSuscribedChannelsList->vec[_n].mChannelName.c_str();
    }

    return pRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ChannelId CommNode::getConnectedChannelId(int _n)
{
    ChannelId ret = InvalidChannelId;

    int num = (int)mSuscribedChannelsList->vec.size();

    if(_n < num)
    {
        ret = mSuscribedChannelsList->vec[_n].mChannelID;
    }

    return ret;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CommNode::getConnectedChannelConnectionType(int _n)
{
    int iRet = COMM_NODE_INVALID;

    int num = (int)mSuscribedChannelsList->vec.size();

    if(_n < num)
    {
        iRet = mSuscribedChannelsList->vec[_n].mNodeType;
    }

    return iRet;
}

void CommNode::addToNotificationList()
{
    if(!mAddedNotificationList)
    {
        MsgMgr::instance()->addCommNodeToNotificationList(this);
        mAddedNotificationList = true;
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CommNode::dispatchMsg(StoredMsg const * _msg, ChannelId _msgFromChannel)
{
    if(_msg)
    {
        int num = (int)mMsgReceiverCallbackList->mVec.size();

        for(int i=0; i<num; i++)
        {
            if(_msgFromChannel == InvalidChannelId || mMsgReceiverCallbackList->mVec[i].mChannelId == InvalidChannelId || mMsgReceiverCallbackList->mVec[i].mChannelId == _msgFromChannel)
                mMsgReceiverCallbackList->mVec[i].mMsgReceiverCallback->receiveMessage(_msg->mSender, _msg->mMessageFamily, _msg->mMessageType, _msg->mMsgData, _msg->mDataLen);
        }
    }
}
