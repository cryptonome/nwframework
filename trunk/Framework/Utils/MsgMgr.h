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
#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include "MsgMgrDefs.h"
#include "NWThread.h"

class NWEvent;
class CommNode;
class MsgMgr;
class ChannelList;
class MsgMgr_Dns;
class NWCriticalSection;
class MsgChannel;

#include <list>
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
enum eMsgMgrDefs
{
    MSG_MGR_INVALID_CHANNEL = 0,
    MSG_MGR_CONTROL_CHANNEL,
    MSG_MGR_ALL_MESSAGES_CHANNEL,
    MSG_MGR_USER_CHANNELS_START
};

struct MsgMgr_InitData
{
    int mReserveChannels;
    int mReserveMessages;
    int mReserveReceivers;
    int mPurgeEveryMsgs;

    MsgMgr_InitData();
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class MsgMgrNotificationCallback
{
public:
    virtual void msgMgrNotification()=0;
};
//typedef void (* MsgMgrNotificationCallback)();

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class MsgMgr : public NWThreadFn
{
public:
    // singleton management
    static bool init(MsgMgr_InitData const * _initData);
    static void done();
    static inline MsgMgr * instance();

    // Add channel / Remove channel / SendTo Channel
    ChannelId addCommNodeToChannel(const char * _channelName, CommNode * _commNode);
    bool addCommNodeToChannel(ChannelId _channelId, CommNode * _commNode);

    ChannelId addCommNodeListenerToChannel(const char * _channelName, CommNode * _commNode);
    bool addCommNodeListenerToChannel(ChannelId _channelId, CommNode * _commNode);

    ChannelId addCommNodeSenderToChannel(const char * _channelName, CommNode * _commNode);
    bool addCommNodeSenderToChannel(ChannelId _channelId, CommNode * _commNode);

    void removeCommNodeFromChannel(const char * _channelName, CommNode * _commNode);
    void removeCommNodeFromChannel(ChannelId _channelId, CommNode * _commNode);
    
    template <class T> void sendMessageTo(const char * _channelName, T const * _msg, CommNodeId _from);
    template <class T> void sendMessageTo(ChannelId _channel, T const * _msg, CommNodeId _from);
    void sendMessageTo(ChannelId _channel, int _messageType, int _messageFamily, unsigned char * _msg, int _msgDataSize, CommNodeId _from);

    // DNS - Refcounted, last one inserted with same name persists
    void regChannelName(const char * _name, ChannelId _id);
    void unregChannelName(const char * _name);
    ChannelId getChannelId(const char * _name);
    const char * getChannelName(ChannelId _channelId);

    // Main thread CommNodes fn : This type of nodes need help for message dispatching
    void regNotificationCallback(MsgMgrNotificationCallback * _notificationCallback);

    void addCommNodeToNotificationList(CommNode * _commNode);
    void removeCommNodeFromNotificationList(CommNode * _commNode);

    void dispatchNotificationMessages();

protected:
    virtual unsigned int threadMain(ThreadParams const * _params); // dispatcher thread entry point

private:
    struct MsgMgrThreadParams
    {
        NWEvent * mMailboxUpdateEvent;
        MsgMgrThreadParams() : mMailboxUpdateEvent(0) {}
    };

    static MsgMgr * mInstance;
   
    NWEvent * mMailboxUpdateEvent;
    NWThread * mThreadDispatcher;
    MsgMgrThreadParams mMsgMgrThreadParams;

    ChannelList * mChannelList;
    MsgMgr_Dns * mMsgMgrDns;

    NWCriticalSection * mCritSecAddRemoveCommNodes;
    NWCriticalSection * mCritSecDns;

    ChannelId mChannelIdGenerator;

    MsgMgrNotificationCallback * mNotificationCallback;

    std::list<CommNode *> mCommNodeNotificationCallbackList;
    typedef std::list<CommNode *>::iterator NotificationIt;

    MsgMgr();   // private constructor, I'm the only one allowed to create an instance
    virtual ~MsgMgr();  // private destructor, I'm the only one allowed to destroy the instance

    bool initializeInstance(MsgMgr_InitData const * _initData);
    void shutdownInstance();

    ChannelId findChannel(const char * _channelName, bool _createIfNotExist=true);
    ChannelId generateChannelId(); 

    void threadPurgeMessages();
    void threadMailboxUpdate();

    MsgChannel * createChannel(const char * _channelName);
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*static*/ inline MsgMgr * MsgMgr::instance()
{
    ASSERT(mInstance);
    return mInstance;
}

#endif // MESSAGE_MANAGER_H
