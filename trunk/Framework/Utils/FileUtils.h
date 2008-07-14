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
