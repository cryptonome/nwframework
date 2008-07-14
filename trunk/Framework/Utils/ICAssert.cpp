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
#include "PchUtils.h"
#include "Assert.h"
#include <string>

//*****************************************************************************
// Visual Studio 
//*****************************************************************************
#ifdef _MSC_VER

    #include <windows.h>

    bool g_bAsserting = false;
    static bool g_bShowMsgBox = true;

    //-------------------------------------------------------------
    //
    //-------------------------------------------------------------
    void assertSetShowMessageBox(bool _show)
    {
        g_bShowMsgBox = _show;
    }

    //-------------------------------------------------------------
    //
    //-------------------------------------------------------------
    int AssertMessageDlg(const char* _msg, ...)
    {
        g_bAsserting = true;

        // Variable parameters
        va_list args;
        va_start (args, _msg);
          
        int iLen = _vscprintf(_msg, args) + 1; 

        char* fullMsg = NEW char[iLen]; 
        vsprintf (fullMsg, _msg, args);
        va_end (args);
        std::string msg = fullMsg;
        msg += "\n";
        DISPOSE_ARRAY(fullMsg);

        // Message box
        int ret = 0;
        if ( g_bShowMsgBox )
        {
            switch (MessageBox(NULL,msg.c_str(),"ASSERT", MB_YESNOCANCEL))
            {
                case IDYES:
                        ret = 0;
                    break;
                case IDNO:
                        ret = 1;
                    break;
                case IDCANCEL:
                        ret = 2;
                    break;
            }
        }

        g_bAsserting = false;

        return ret;
    }


#endif // Visual Studio

