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
#ifndef MULTITASK_H_
#define MULTITASK_H_

#include "NWThread.h"
#include <list>

//********************************************************************
//
//********************************************************************
class MultiTask : public NWThreadFn
{
public:
    MultiTask  ();
    virtual    ~MultiTask ()                      { MultiTask::done(); }

    //--------------------------------
    // ITaskProcessor
    //--------------------------------
    class ITaskProcessor
    {
    public:
        virtual ~ITaskProcessor() { }

        virtual bool multiTaskProcess(MultiTask* _multiTask, int _task) = 0;
    };


    virtual bool          init        (int _tasks, ITaskProcessor* _processor);
    bool                  isOk        () const  { return mInit; }
    virtual void          done        ();

    bool start();
    void stop();

private:
    struct Thread
    {
        NWThread* mThread;
        NWEvent* mEventExit;
    };

    // NWThreadFn
    virtual unsigned int threadMain(ThreadParams const * _threadParams);

    int getThreadIndex(Thread* thread);

    bool          mInit : 1;
    std::list<Thread*> mThreads;
    ITaskProcessor* mTaskProcessor;
};

#endif
