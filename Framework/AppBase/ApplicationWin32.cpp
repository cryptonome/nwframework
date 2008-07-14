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
#include "PchAppBase.h"

#include "Application.h"
#include <windows.h>

static bool gQuit = false;

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Application::quit()
{
    gQuit = true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    Utils::init(true);

    Application* app = Application::CreateApplication();

    if ( app->init(lpCmdLine) )
    {
        MSG msg;
        while ( !gQuit && GetMessage(&msg, (HWND) NULL, 0, 0) )
        { 
            TranslateMessage(&msg); 
            DispatchMessage(&msg); 
        }        
    }
    app->done();
    Application::DestroyApplication(app);

    Utils::done();

    return 0;
}
