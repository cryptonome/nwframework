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
#ifndef MESSAGE_RECEIVER_H
#define MESSAGE_RECEIVER_H

#include "MsgMgrDefs.h"
#include "MsgMgrAux.h" // zzz temp
#include <string>

class NWEvent;
struct StoredMsg;
class MsgChannel;
struct SuscribedChannels;
struct MsgReceiverCallbackList;

enum eCommNodeTimeout
{
    COMM_NODE_WAIT_INFINITE = 0xffffffff
};

enum eCommNodeType
{
    COMM_NODE_INVALID   = 0x00000000,
    COMM_NODE_LISTENER  = 0x00000001,
    COMM_NODE_SENDER    = 0x00000002,

    COMM_NODE_BIDIR     = COMM_NODE_LISTENER|COMM_NODE_SENDER,
};

class MsgReceiverCallback
{
public:
    virtual void receiveMessage(CommNodeId _from, int _msgFamily, int _msgType, void * _msg, int _msgSize) = 0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class CommNode
{
public:
    CommNode();
    virtual ~CommNode();
    
    bool init(CommNodeId _id, const char * _name=NULL, int _maxChannels=16, int _maxCallbacks=32, NWEvent * _eventMsgAvailable=NULL);
    void done();

    ChannelId joinChannel(const char * _channelName, int _type=COMM_NODE_BIDIR);
    bool joinChannel(ChannelId _channelId, int _type=COMM_NODE_BIDIR);
    
    void exitChannelNth(int _connectedChannelNth);
    void exitChannel(const char * _channelName);
    void exitChannel(ChannelId _channelId);
    void exitAllChannels();

    template <class T> void sendMessage(T const & _msg);

    bool waitMessage(int _timeOutMs=COMM_NODE_WAIT_INFINITE);
    bool testMsgAvailable(ChannelId _channelId = InvalidChannelId);
    bool getAvailableMsg(int & msgFamily_, int & msgType_, void * & msg_, int & msgSize_, ChannelId & msgSenderChannel_, ChannelId _checkChannelId/*=InvalidChannelId*/);

    void addMessageReceiverCallback(MsgReceiverCallback * _receiver, ChannelId _channelId = InvalidChannelId);
    void removeMessageReceiverCallback(MsgReceiverCallback * _receiver, ChannelId _channelId = InvalidChannelId);
    void dispatchAvailableMessages(ChannelId _channelId=InvalidChannelId);

    inline void setEventMsgAvailable(NWEvent * _eventMsgAvailable);
    inline NWEvent * getEventMsgAvailable();

    int getNumConnectedChannels() const;
    const char * getConnectedChannelName(int _n);
    ChannelId getConnectedChannelId(int _n);
    int getConnectedChannelConnectionType(int _n);

    inline void waitDispatcher();
    inline void purgeSentMsgs();

    void addToNotificationList();

    inline void setReceiveAllMessages(bool _receiveAll){mReceiveAllMessages = _receiveAll;}

private:
    friend class MsgMgr;

    bool mInitd;
    CommNodeId mCommNodeId;
    SuscribedChannels * mSuscribedChannelsList;
    NWEvent * mEventMsgAvailable;
    bool mEventOwner;
    MsgChannel * mLocalChannel;
    MsgReceiverCallbackList * mMsgReceiverCallbackList;
    int mSentMsgs;
    bool mAddedNotificationList;
    bool mReceiveAllMessages;

    StoredMsg * addListener(MsgChannel * _listener);
    void removeListener(MsgChannel * _listener);
    inline MsgChannel * getLocalChannel();
    void dispatchMsg(StoredMsg const * _msg, ChannelId _msgFromChannel);
};

template <class T> void CommNode::sendMessage(T const & _msg)
{
    mLocalChannel->sendMessage(_msg, mCommNodeId);

    mSentMsgs++;
    if(mSentMsgs >= NumMaxMsgsDispatched)
    {
        mSentMsgs = 0;
        mLocalChannel->waitForListeners();
        mLocalChannel->purgeSentMsgs();
    }
}

inline void CommNode::setEventMsgAvailable(NWEvent * _eventMsgAvailable)
{
    mEventMsgAvailable = _eventMsgAvailable;
}

inline NWEvent * CommNode::getEventMsgAvailable()
{
    return mEventMsgAvailable;
}

inline void CommNode::waitDispatcher()
{
    mLocalChannel->waitForListeners();
}

inline void CommNode::purgeSentMsgs()
{
    mLocalChannel->purgeSentMsgs();
}

inline MsgChannel * CommNode::getLocalChannel()
{
    return mLocalChannel;
}

#endif // MESSAGE_RECEIVER_H
