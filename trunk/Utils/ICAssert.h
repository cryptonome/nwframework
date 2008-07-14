#ifndef ICASSERT__
#define ICASSERT__

#include <stdio.h>

// Visual Studio
/*#ifndef  // TODO
*/

  #define ASSERT(test)
/*
// Builder
#else

  #include <vcl.h>

  // Debug
  #ifdef _DEBUG
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

  // Release
  #else
	  #define ASSERT(test)
  #endif

#endif // VS/Builder
  */

#endif

