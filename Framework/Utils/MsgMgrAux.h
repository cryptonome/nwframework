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


#ifndef MESSAGE_MANAGER_AUX_H
#define MESSAGE_MANAGER_AUX_H

#include "MsgMgrDefs.h"
#include "NWSlink.h"
#include "NWTypes.h"
#include "MsgDefs.h"

#include <string>

class NWEvent;
class CommNode;
class MsgChannel;

//****************************************************************************
//
//****************************************************************************
struct StoredMsg : public SNLink<StoredMsg>
{
    CommNodeId mSender;
    int mMessageType;
    int mMessageFamily;
    NWBaseMsgInternal * mMsgData;
    int mDataLen;
    u64 mMsgRefId;

    StoredMsg(u64 _msgRefId);
    StoredMsg(CommNodeId _sender, int _messageType, int _messageFamily, NWBaseMsgInternal * _msgData, int _dataLen, u64 _msgRefId);
    ~StoredMsg();

    void copyFrom(StoredMsg const & _other);

    StoredMsg operator = (StoredMsg const & _other);
};

//****************************************************************************
//
//****************************************************************************
struct ChannelListener : public DLink<ChannelListener>
{
    MsgChannel * mChannelListener;
    ChannelId mChannelId;
    NWEvent * mEventMessagesAvailable;
    StoredMsg * volatile mCurrentMsg;

    ChannelListener(MsgChannel * _channelListener, StoredMsg * _currentMsg);
};


//****************************************************************************
//
//****************************************************************************
struct ListenedChannel : public DLink<ListenedChannel>
{
    MsgChannel * mChannelListened;
    ChannelListener * mListener;
    NWEvent * mEventMessagesAvailable;

    ListenedChannel(MsgChannel * _channelListened, ChannelListener * _listener, NWEvent * _eventMessagesAvailable);
};

//****************************************************************************
//
//****************************************************************************
class MsgChannel : public SNLink<MsgChannel>
{
public:
    MsgChannel();
    ~MsgChannel();

    void init(const char * _name, ChannelId _channelId, NWEvent * _eventMsgAvailable);
    void shutdown();

    void listenTo(MsgChannel * _channel);
    void stopListeningTo(MsgChannel * _channel);

    template <class T> inline void sendMessage(T const & _msg, CommNodeId _sender);

    bool testMsgAvailable(ChannelId _channelId=InvalidChannelId);
    StoredMsg * getAvailableMsg(ChannelId & msgFromChannel_, ChannelId _channelId=InvalidChannelId);
    void nextAvailableMsg(ChannelId _channelId=InvalidChannelId);

    inline const char * getName() const;

    inline int getNumListeners();
    inline int getNumListened();
    inline ChannelId getChannelId();
    inline NWEvent * getEventMessageAvailable();

    void waitForListeners(ChannelId _channelId=InvalidChannelId);
    void purgeSentMsgs();

private:
    friend class MsgMgr;

    bool mInitd;
    std::string mChannelName;
    ChannelId mChannelId;
    StoredMsg * mHeadMsgList;
    StoredMsg * mTailMsg;
    ChannelListener * mHeadListenerDList;
    ListenedChannel * mHeadListenedDList;
    int mNumListeners;
    int mNumListened;
    NWEvent * mEventMessagesAvailable;
    u64 mMsgRefIdSeed;

    ChannelListener * addListener(MsgChannel * _listener);
    void removeListener(MsgChannel * _listener);

    StoredMsg * createStoredMsg();
    void unlinkAndDestroyStoredMsgHead();
    void linkStoredMsg(StoredMsg * _msg);

    ChannelListener * createListener(MsgChannel * _channelListener, StoredMsg * _currentMsg);
    void unlinkAndDestroyListener(ChannelListener * _listener);
    void linkListener(ChannelListener * _listener);
    ChannelListener * findListener(MsgChannel * _channelListener);
    inline ChannelListener * getHeadListenerChannel();

    ListenedChannel * createListened(MsgChannel * _listenedChannel, ChannelListener * _listener);
    void unlinkAndDestroyListened(ListenedChannel * _listened);
    void linkListened(ListenedChannel * _listened);
    ListenedChannel * findListened(MsgChannel * _listenedChannel);
    inline ListenedChannel * getHeadListenedChannel();

    u64 generateMsgRefId();

    void signalListeners();

    void purgeSentMsgs(u64 _minMsgRefId);
    u64 getMinMsgRefId();
};

template <class T> inline void MsgChannel::sendMessage(T const & _msg, CommNodeId _sender)
{
    T * msgData = NEW T();
    *msgData = _msg;

    StoredMsg * storedMsg = createStoredMsg();
    storedMsg->mSender = _sender;
    storedMsg->mMsgData = msgData;
    storedMsg->mMessageFamily = T::MSG_FAMILY;
    storedMsg->mMessageType = T::MSG_TYPE;
    storedMsg->mDataLen = sizeof(T);

    linkStoredMsg(storedMsg);
}

inline const char * MsgChannel::getName() const
{
    return mChannelName.c_str();
}

inline int MsgChannel::getNumListeners()
{
    return mNumListeners;
}

inline int MsgChannel::getNumListened()
{
    return mNumListened;
}

inline ChannelId MsgChannel::getChannelId()
{
    return mChannelId;
}

inline NWEvent * MsgChannel::getEventMessageAvailable()
{
    return mEventMessagesAvailable;
}

inline ChannelListener * MsgChannel::getHeadListenerChannel()
{
    return mHeadListenerDList;
}

inline ListenedChannel * MsgChannel::getHeadListenedChannel()
{
    return mHeadListenedDList;
}

//****************************************************************************
//
//****************************************************************************
class ChannelList
{
public:
    ChannelList();
    ~ChannelList();

    bool initialize();
    void shutdown();

    MsgChannel * addChannel(const char * _name, ChannelId _channelId, NWEvent * _eventMsgAvailable);
    void removeChannel(ChannelId _channelId);

    ChannelId getChannelId(const char * _channelName);
    MsgChannel * getChannel(const char * _channelName);
    MsgChannel * getChannel(ChannelId _channelId);

private:
    friend class MsgMgr;

    bool mInitd;
    MsgChannel * mHeadChannelList;
    MsgChannel * mTailChannel;

    MsgChannel * createChannel(const char * _name, ChannelId _channelId, NWEvent * _eventMsgAvailable);
    void linkChannelAsTail(MsgChannel * msgChannel);
    void unlinkAndDestroyChannel(ChannelId _channelId);

    inline MsgChannel * getHeadChannelList();
};

inline MsgChannel * ChannelList::getHeadChannelList()
{
    return mHeadChannelList;
}

//****************************************************************************
//
//****************************************************************************
struct DnsHash;

class MsgMgr_Dns
{
public:
    MsgMgr_Dns();
    ~MsgMgr_Dns();

    bool initialize();
    void shutdown();

    void regChannelName(const char * _name, ChannelId _id);
    void unregChannelName(const char * _name);
    ChannelId getChannelId(const char * _name);

private:
    DnsHash * mDnsHash;
};

#endif // MESSAGE_MANAGER_AUX_H
