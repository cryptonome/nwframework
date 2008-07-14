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



