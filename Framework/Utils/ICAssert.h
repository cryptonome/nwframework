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
#ifndef ICASSERT__
#define ICASSERT__

#include <stdio.h>

void assertSetShowMessageBox(bool _show);

//*****************************************************************************
// Debug
//*****************************************************************************
#ifdef _DEBUG

    //*****************************************************************************
    // Visual Studio 
    //*****************************************************************************
    #ifdef _MSC_VER

        extern bool g_bAsserting;

        int AssertMessageDlg(const char* _msg, ...);

	    #define ASSERT(p) \
		    do \
		    { \
		      static bool s_bBreak = true; \
		      if ( !(p) && s_bBreak ) \
		      { \
			      int result = AssertMessageDlg("ASSERTION FAILED:\n%s\nfile(%s), line(%i)\n\n Do you want to break?\n\nYes -> Break\nNo -> Continue execution\nCancel -> Continue execution and disable this assert",#p,__FILE__,__LINE__); \
			      switch(result) \
			      { \
				      case 0: /*Yes*/ \
					      __asm { int 3 }; \
					      break; \
				      case 1: /*No*/ \
					      break; \
				      case 2: /*Ignore*/ \
					      s_bBreak = false; \
					      break; \
			      } \
		      } \
		    } while (0)

    
    #endif // Visual Studio

    //*****************************************************************************
    // Borland Builder
    //*****************************************************************************
    #ifdef __BORLANDC__ 

      #include <vcl.h>

	    #define ASSERT(p) \
		    do \
		    { \
		      static bool s_bBreak = true; \
		      if ( !(p) && s_bBreak ) \
		      { \
			      char szTemp[1024]; \
			      sprintf(szTemp,"ASSERTION FAILED:\n%s\nfile(%s), line(%i)\n\n Do you want to break?\n\nYes -> Break\nNo -> Continue execution\nIgnore -> Continue execution and disable this assert",#p,__FILE__,__LINE__); \
			      int result = MessageDlg(szTemp, mtError, TMsgDlgButtons() << mbYes<<mbNo<<mbIgnore, 0); \
			      switch(result) \
			      { \
				      case mrYes: \
					      asm { int 3 }; \
					      break; \
				      case mrNo: \
					      break; \
				      case mrIgnore: \
					      s_bBreak = false; \
					      break; \
			      } \
		      } \
		    } while (0)

    #endif // Builder


//*****************************************************************************
// Release
//*****************************************************************************
#else

  #define ASSERT(test)

#endif




#endif // ICAssert
