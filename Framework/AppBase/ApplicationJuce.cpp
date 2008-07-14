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
#include "PchAppBase.h"

#include "Application.h"
#include "juce.h"

class InCrewApplication;

static InCrewApplication* sApplication = 0;

//****************************************************************************
//
//****************************************************************************
//==============================================================================
class InCrewApplication : public JUCEApplication
{
    /* Important! NEVER embed objects directly inside your JUCEApplication class! Use
       ONLY pointers to objects, which you should create during the initialise() method
       (NOT in the constructor!) and delete in the shutdown() method (NOT in the
       destructor!)

       This is because the application object gets created before Juce has been properly
       initialised, so any embedded objects would also get constructed too soon.
   */
    Application* mApp;

public:
    //==============================================================================
    InCrewApplication()
        : mApp(0)
    {
        // NEVER do anything in here that could involve any Juce function being called
        // - leave all your startup tasks until the initialise() method.
        sApplication = this;
    }

    ~InCrewApplication()
    {
        // Your shutdown() method should already have done all the things necessary to
        // clean up this app object, so you should never need to put anything in
        // the destructor.

        // Making any Juce calls in here could be very dangerous...
        sApplication = 0;
    }

    //==============================================================================
    void initialise (const String& commandLine)
    {
        Utils::init(false);

        ASSERT(mApp == 0);
        mApp = Application::CreateApplication();
        mApp->init(commandLine);
    }

    void shutdown()
    {
        if (mApp)
        {
            mApp->done();
            Application::DestroyApplication(mApp);
            mApp = 0;
        }
    
        Utils::done();
    }

    //==============================================================================
    const String getApplicationName()
    {
        return T("InCrew App");
    }

    const String getApplicationVersion()
    {
        return T("1.0");
    }

    bool moreThanOneInstanceAllowed()
    {
        return true;
    }

    void anotherInstanceStarted (const String& commandLine)
    {
        // This will get called if the user launches another copy of the app, but
        // there's nothing that the demo app needs to do here.
    }

    /*virtual void systemRequestedQuit()
    {
        mApp->systemRequestedQuit();
    }*/
};


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
START_JUCE_APPLICATION (InCrewApplication)

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Application::quit()
{
    sApplication->quit();
}
