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
#ifndef _INCREW_THREAD_H_
#define _INCREW_THREAD_H_

class NWEvent;
class NWThreadFn;
class NWThreadInstance;

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
struct ThreadParams
{
    void * mUserParams;
    NWEvent * mEventEndRequest;

    inline ThreadParams(void * _userParams, NWEvent * _eventEndRequest);
};

inline ThreadParams::ThreadParams(void * _userParams, NWEvent * _eventEndRequest) :
    mUserParams(_userParams),
    mEventEndRequest(_eventEndRequest)
{
}

class NWThreadFn
{
protected:
    virtual unsigned int threadMain(ThreadParams const * _threadParams) = 0;

private:
    static unsigned int __stdcall threadEntryPoint(void * _params);
    friend class NWThreadInstance;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

enum eNWThreadPriority
{
    NWT_PRIORITY_NORMAL = 0,
    NWT_PRIORITY_LOW,
    NWT_PRIORITY_HIGH,
    NWT_PRIORITY_CRITICAL,
    NWT_PRIORITY_INVALID
};

//----------------------------------------------------------------------------
// Thread class to be used as class member
//----------------------------------------------------------------------------
class NWThread
{
public:
    bool start(NWThreadFn * _fn, void * _params=0, eNWThreadPriority _priority=NWT_PRIORITY_NORMAL);

    bool isRunning();

    void setPriority(eNWThreadPriority _priority);
    eNWThreadPriority getPriority();
    
    void requestEnd(bool _threadAsyncAutoDestroy=false);

    void waitForEnd();

    static NWThread * create();
    static void destroy(NWThread* & _thread);

private:
    NWThreadInstance * mNWThreadInstance;

    NWThread();
    ~NWThread();
};

#endif // _INCREW_THREAD_H_
