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

