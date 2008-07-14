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
