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

#include "NWThread.h"
#include "NWEvent.h"

#include <process.h>
#include <windows.h>

typedef HANDLE NWThreadHandle;

struct NWThreadInitData;

//****************************************************************************
// Helper class to allow async thread finalization (used by NWThread only)
//****************************************************************************
class NWThreadInstance
{
public:
    bool isRunning();

    void setPriority(eNWThreadPriority _priority);
    eNWThreadPriority getPriority();
    
    void requestEnd(bool _threadAsyncAutoDestroy);

    void waitForEnd();

    NWThreadHandle getHandle();

    static NWThreadInstance * create(NWThreadFn * _fn, void * _params=0, eNWThreadPriority _priority=NWT_PRIORITY_NORMAL);
    static void destroy(NWThreadInstance * _thread);

private:
    friend class NWThread;
    friend class NWThreadFn;

    NWThreadInitData * mNWThreadInitData;
    NWThreadHandle mNWThreadHandle;

    NWThreadInstance(NWThreadFn * _fn, void * _params=0, eNWThreadPriority _priority=NWT_PRIORITY_HIGH);
    ~NWThreadInstance();

    NWThreadHandle startThread(NWThreadInitData * _initData);
};

//****************************************************************************
// Thread data
//****************************************************************************
struct NWThreadInitData
{
    NWThreadInstance * mNWThreadInstance;
    NWEvent * mNWEventEndRequest;
    NWEvent * mNWEventThreadEnded;
    NWThreadFn * mThis;
    void * params;
    volatile bool mAsyncDestroy;
    volatile bool mRunning;

    NWThreadInitData(NWThreadInstance * _threadInstance, NWEvent * _eventEndRequest, NWEvent * _eventThreadEnded, NWThreadFn * _fn, void * _params);
    ~NWThreadInitData();
};

//****************************************************************************
//
//****************************************************************************
NWThreadInitData::NWThreadInitData(NWThreadInstance * _threadInstance, NWEvent * _eventEndRequest, NWEvent * _eventThreadEnded, NWThreadFn * _fn, void * _params) :
    mNWThreadInstance(_threadInstance),
    mNWEventEndRequest(_eventEndRequest),
    mNWEventThreadEnded(_eventThreadEnded),
    mThis(_fn),
    params(_params),
    mAsyncDestroy(false),
    mRunning(false)
{
}

NWThreadInitData::~NWThreadInitData()
{
    NWEvent::destroy(mNWEventEndRequest);
    NWEvent::destroy(mNWEventThreadEnded);
}

//****************************************************************************
// Thread Callback
//****************************************************************************
/*static*/ unsigned int __stdcall NWThreadFn::threadEntryPoint(void * _params)
{
    unsigned int uRet = 0;

    NWThreadInitData * initData = (NWThreadInitData *)_params;
    if(initData)
    {
        ThreadParams threadParams(initData->params, initData->mNWEventEndRequest);

        initData->mRunning = true;
        uRet = initData->mThis->threadMain(&threadParams);
        initData->mRunning = false;

        if(initData->mAsyncDestroy)
        {
            NWThreadInstance::destroy(initData->mNWThreadInstance);
        }

        initData->mNWEventThreadEnded->signal(); // this must be after the async destuction to avoid problems with sync destruction (after the signal the other thread destroys initdata and the code may think it is async depending on memory value and tries to delete an already deleted thread)
    }

    return uRet;
}


//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWThreadInstance::NWThreadInstance(NWThreadFn * _fn, void * _params/*=0*/, eNWThreadPriority _priority/*=NWT_PRIORITY_NORMAL*/) :
    mNWThreadInitData(NULL),
    mNWThreadHandle(NULL)
{
    NWEvent * eventEndRequest = NWEvent::create(true, false, NULL);
    NWEvent * eventThreadEnded = NWEvent::create(true, false, NULL);
    mNWThreadInitData = NEW NWThreadInitData(this, eventEndRequest, eventThreadEnded, _fn, _params);
    mNWThreadHandle = startThread(mNWThreadInitData);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWThreadInstance::~NWThreadInstance()
{
    waitForEnd();

    DISPOSE(mNWThreadInitData);
    CloseHandle(mNWThreadHandle);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool NWThreadInstance::isRunning()
{
    return mNWThreadInitData->mRunning;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWThreadInstance::setPriority(eNWThreadPriority _priority)
{
    int sysPriority = THREAD_PRIORITY_ABOVE_NORMAL;

    switch(_priority)
    {
        case NWT_PRIORITY_CRITICAL:
        {
            sysPriority = THREAD_PRIORITY_TIME_CRITICAL;
            break;
        }

        case NWT_PRIORITY_HIGH:
        {
            sysPriority = THREAD_PRIORITY_ABOVE_NORMAL; // THREAD_PRIORITY_HIGHEST
            break;
        }

        case NWT_PRIORITY_NORMAL:
        {
            sysPriority = THREAD_PRIORITY_NORMAL;
            break;
        }

        case NWT_PRIORITY_LOW:
        {
            sysPriority = THREAD_PRIORITY_BELOW_NORMAL; // THREAD_PRIORITY_LOWEST //THREAD_PRIORITY_IDLE
            break;
        }
    }

    SetThreadPriority(mNWThreadHandle, sysPriority);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
eNWThreadPriority NWThreadInstance::getPriority()
{
    eNWThreadPriority eRet = NWT_PRIORITY_INVALID;

    int priority = GetThreadPriority(mNWThreadHandle);
    switch(priority)
    {
        case THREAD_PRIORITY_TIME_CRITICAL:
        {
            eRet = NWT_PRIORITY_CRITICAL;
            break;
        }

        case THREAD_PRIORITY_ABOVE_NORMAL:
        case THREAD_PRIORITY_HIGHEST:
        {
            eRet = NWT_PRIORITY_HIGH;
            break;
        }

        case THREAD_PRIORITY_NORMAL:
        {
            eRet = NWT_PRIORITY_NORMAL;
            break;
        }

        case THREAD_PRIORITY_IDLE:
        case THREAD_PRIORITY_LOWEST:
        case THREAD_PRIORITY_BELOW_NORMAL:
        {
            eRet = NWT_PRIORITY_LOW;
            break;
        }
    }

    return eRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWThreadInstance::requestEnd(bool _threadAsyncAutoDestroy/*=false*/)
{
    if(_threadAsyncAutoDestroy)
        mNWThreadInitData->mAsyncDestroy = _threadAsyncAutoDestroy;

    mNWThreadInitData->mNWEventEndRequest->signal();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWThreadInstance::waitForEnd()
{
    mNWThreadInitData->mNWEventThreadEnded->waitForSignal();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWThreadHandle NWThreadInstance::getHandle()
{
    return mNWThreadHandle;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWThreadHandle NWThreadInstance::startThread(NWThreadInitData * _initData)
{
    return (NWThreadHandle)_beginthreadex(NULL, 0, &_initData->mThis->threadEntryPoint, (void*)_initData, 0, 0);
}

//****************************************************************************
// Internal Thread Creation
//****************************************************************************
/*static*/ NWThreadInstance * NWThreadInstance::create(NWThreadFn * _fn, void * _params/*=0*/, eNWThreadPriority _priority/*=NWT_PRIORITY_NORMAL*/)
{
    return NEW NWThreadInstance(_fn, _params, _priority);
}

/*static*/ void NWThreadInstance::destroy(NWThreadInstance * _thread)
{
    DISPOSE(_thread);
}

//****************************************************************************
// User exposed thread
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWThread::NWThread() : 
    mNWThreadInstance(NULL)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWThread::~NWThread()
{
    if(mNWThreadInstance)   // if != NULL -> sync end
    {
        NWThreadInstance::destroy(mNWThreadInstance);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool NWThread::start(NWThreadFn * _fn, void * _params/*=0*/, eNWThreadPriority _priority/*=NWT_PRIORITY_NORMAL*/)
{
    bool bRet = false;

    if(!mNWThreadInstance)
    {
        mNWThreadInstance = NWThreadInstance::create(_fn, _params, _priority);
        bRet = mNWThreadInstance->getHandle() != NULL;
    }

    return bRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool NWThread::isRunning()
{
    return mNWThreadInstance ? mNWThreadInstance->isRunning() : false;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWThread::setPriority(eNWThreadPriority _priority)
{
    if(mNWThreadInstance)
        mNWThreadInstance->setPriority(_priority);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
eNWThreadPriority NWThread::getPriority()
{
    return mNWThreadInstance ? mNWThreadInstance->getPriority() : NWT_PRIORITY_INVALID;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWThread::requestEnd(bool _threadAsyncAutoDestroy/*=false*/)
{
    if(mNWThreadInstance)
    {
        mNWThreadInstance->requestEnd(_threadAsyncAutoDestroy);
        if(_threadAsyncAutoDestroy)
        {
            mNWThreadInstance = NULL;
        }
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWThread::waitForEnd()
{
    if(mNWThreadInstance)
        mNWThreadInstance->waitForEnd();
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*static*/ NWThread * NWThread::create()
{
    return NEW NWThread();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*static*/ void NWThread::destroy(NWThread* & _thread)
{
    if(_thread)
    {
        if(_thread->isRunning())
            _thread->requestEnd();
        DISPOSE(_thread);
    }
}
