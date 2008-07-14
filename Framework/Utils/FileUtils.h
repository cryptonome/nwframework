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
#ifndef FILEUTILS_H_
#define FILEUTILS_H_

#include <list>
#include <string>

//********************************************************************
//
//********************************************************************
namespace FileUtils
{
    enum EFileType
    {
        E_FILETYPE_ARCHIVE = 0,
        E_FILETYPE_DIRECTORY,
        E_FILETYPE_UNKNOWN,
    };
    EFileType getFileType(const char* _fileName);

    bool getFilesFromDirectory(const char* _path, std::list<std::string>& _files);

    std::string setNewFileNameExtension(const char* _fileName, const char* _ext);

} // FileUtils

#endif
