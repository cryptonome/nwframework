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

#ifndef SYSTEM_UTILS__

#define SYSTEM_UTILS__

#include "MemoryUtils.h"
#include <string>
#include <list>

//********************************************************************
//
//********************************************************************
namespace SystemUtils
{
    std::string getCurrentDirectory();

    void messageBox(const char* _message, ...);
    void messageBoxWithTitle(const char* _title, const char* _msg, ...);

    unsigned int getCurrentThreadId();

    struct FileType
    {
        std::string mName;
        std::string mExtension;
    };

    bool getOpenFileName(const char* _title, const std::list<FileType>& _fileTypes, std::string& fileName_);

} // SystemUtils

#endif
