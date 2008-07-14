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
#ifndef _INCREW_DEFS_H_
#define _INCREW_DEFS_H_

//*****************************************************************************
// Debug && Visual Studio 
//*****************************************************************************
#if defined(_DEBUG) && defined(_MSC_VER) 

    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>

    #define NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #define DISPOSE(ptr) do { delete ptr; ptr = 0; } while (0)
    #define DISPOSE_ARRAY(ptr) do { delete[] ptr; ptr = 0; } while (0)

//*****************************************************************************
// Release
//*****************************************************************************
#else // Release

    #define NEW new
    #define DISPOSE(ptr) do { delete ptr; ptr = 0; } while (0)
    #define DISPOSE_ARRAY(ptr) do { delete[] ptr; ptr = 0; } while (0)

#endif 

#endif // _INCREW_DEFS_H_
