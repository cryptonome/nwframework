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
 */#include "PchUtils.h"

#include "FileUtils.h"
#include <windows.h>
#include "StrUtils.h"

//********************************************************************
//
//********************************************************************
namespace FileUtils
{

//********************************************************************
//
//********************************************************************
//-------------------------------------------------------------
//
//-------------------------------------------------------------
EFileType getFileType(const char* _fileName)
{
    EFileType type = E_FILETYPE_UNKNOWN;

    DWORD attributes = GetFileAttributes(_fileName);

    if ( attributes != INVALID_FILE_ATTRIBUTES )
    {
        if ( (attributes&FILE_ATTRIBUTE_DIRECTORY) != 0 )
            type = E_FILETYPE_DIRECTORY;
        else if ( (attributes&FILE_ATTRIBUTE_ARCHIVE) != 0 )
            type = E_FILETYPE_ARCHIVE;        
    }

    return type;
}


//********************************************************************
//
//********************************************************************
//-------------------------------------------------------------
//
//-------------------------------------------------------------
bool getFilesFromDirectory(const char* _path, std::list<std::string>& _files)
{
    bool bOK = true;

    std::string path = _path;
    path += "\\*.*";
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(path.c_str(), &findFileData);

    bOK = hFind != INVALID_HANDLE_VALUE;

    if ( bOK )
    {
        do
        {
            if ( (findFileData.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE) != 0 )
                _files.push_back(findFileData.cFileName);

        } while ( FindNextFile(hFind, &findFileData) != 0 );

        FindClose(hFind);
    }

    return bOK;
}


//********************************************************************
//
//********************************************************************
//-------------------------------------------------------------
//
//-------------------------------------------------------------
std::string setNewFileNameExtension(const char* _fileName, const char* _ext)
{
    char* fileNameWithNoExtension = StrUtils::stdStringToCharPointer(_fileName);

    // Find last point and change it by a zero
    int lastPoint = -1;
    int pos = 0;
    while ( fileNameWithNoExtension[pos] != '\0' )
    {
        if ( fileNameWithNoExtension[pos] == '.' )
            lastPoint = pos;
        pos++;
    }
    if ( lastPoint != -1 )
        fileNameWithNoExtension[lastPoint] = '\0';

    std::string fileName = fileNameWithNoExtension;
    DISPOSE_ARRAY(fileNameWithNoExtension);
    fileName += _ext;

    return fileName;
}





} // FileUtils