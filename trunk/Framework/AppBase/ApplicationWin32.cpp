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
