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
