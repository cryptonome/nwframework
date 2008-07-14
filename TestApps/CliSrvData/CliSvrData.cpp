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
#include "PchNomosVE.h"

#include <windows.h>
#include "AppModules.h"
#include "Application.h"
#include "FormMain.h"
#include "Window.h"
#include "GUI.h"

#include "TestDataServer.h"

const char* APPLICATION_NAME = "Nomos VE";

//****************************************************************************
//
//****************************************************************************
class CNomosVE : public Application
{
public:
    CNomosVE();

    virtual bool        init                  (const char* _cmdLine);
    virtual void        done                  ();

private:
    TestDataServer * mTestDataServer;
    FormMain * mMainForm;
};


//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
Application* Application::CreateApplication()
{
    return NEW CNomosVE();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Application::DestroyApplication(Application* _app)
{
    DISPOSE(_app);
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
CNomosVE::CNomosVE() : Application(),
    mTestDataServer(NULL)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool CNomosVE::init(const char* _cmdLine)
{
    bool bOK = true;
    LOG("NomosVE Start");

    bOK = Application::init(APPLICATION_NAME);

    if ( bOK )
    {
        bOK &= AppModules::initModules();
        ASSERT(bOK);

        mTestDataServer = NEW TestDataServer();
        bOK &= mTestDataServer->init(getServerDataProvider());
        ASSERT(bOK);

        // Main Form
        mMainForm = NEW FormMain;
        if ( !mMainForm->init(mGUI, "CliSvr", mGUI->getMainWindow(), 0, 0, getClientDataProvider()) )
            DISPOSE(mMainForm);

        mGUI->getMainWindow()->setVisible(true);
    }

    return bOK;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CNomosVE::done()
{
    DISPOSE(mMainForm);

    mTestDataServer->shutdown();
    DISPOSE(mTestDataServer);

    AppModules::shutdownModules();

    Application::done();

    LOG("NomosVE End");
}



