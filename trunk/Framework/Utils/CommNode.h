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
