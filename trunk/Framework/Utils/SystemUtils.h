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
