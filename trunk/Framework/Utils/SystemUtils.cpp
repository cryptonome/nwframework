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
#include "PchUtils.h"
#include "SystemUtils.h"

#include "PchUtils.h"
#include "Utils.h"
#include <vector>
#include <windows.h>

//********************************************************************
//
//********************************************************************
namespace SystemUtils
{

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
std::string getCurrentDirectory()
{
    std::string directory;

	// Get current directory
	char Dir[1024]; Dir[0] = '\0';
	GetCurrentDirectory(sizeof(Dir),Dir);
    directory = Dir;

    return directory;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void messageBoxWithTitle(const char* _title, const char* _msg, ...)
{
    va_list args; 
    va_start (args, _msg);
      
    int iLen = _vscprintf(_msg, args) + 1; 

    char* fullMsg = NEW char[iLen]; 
    vsprintf (fullMsg, _msg, args);
    va_end (args);
    std::string msg = fullMsg;
    DISPOSE_ARRAY(fullMsg);

    MessageBox(NULL,msg.c_str(),_title,MB_OK);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void messageBox(const char* _msg, ...)
{
    va_list args; 
    va_start (args, _msg);
      
    int iLen = _vscprintf(_msg, args) + 1; 

    char* fullMsg = NEW char[iLen]; 
    vsprintf (fullMsg, _msg, args);
    va_end (args);
    std::string msg = fullMsg;
    DISPOSE_ARRAY(fullMsg);

    messageBoxWithTitle("",msg.c_str());
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
unsigned int getCurrentThreadId()
{
    return ::GetCurrentThreadId();
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool getOpenFileName(const char* _title, const std::list<FileType>& _fileTypes, std::string& fileName_)
{
    bool ok = true;

    // Create filter string
    std::vector<char> filter;
    for ( std::list<FileType>::const_iterator it = _fileTypes.begin() ; it != _fileTypes.end() ; ++it )
    {
        const FileType& fileType = *it;

        int i = 0;
        while ( fileType.mName[i] != '\0' )
            filter.push_back(fileType.mName[i++]);
        filter.push_back('\0');

        i = 0;
        filter.push_back('*');
        filter.push_back('.');
        while ( fileType.mExtension[i] != '\0' )
            filter.push_back(fileType.mExtension[i++]);
        filter.push_back('\0');

        filter.push_back('\0');
    }

    // OpenFile Dialog
    char fileName[255] = { 0 };
    OPENFILENAME opf;
    opf.hwndOwner = 0;
    opf.lpstrFilter = &filter[0];
    opf.lpstrCustomFilter = 0;
    opf.nMaxCustFilter = 0L;
    opf.nFilterIndex = 1L;
    opf.lpstrFile = fileName;
    opf.nMaxFile = sizeof(fileName);
    opf.lpstrFileTitle = 0;
    opf.nMaxFileTitle = 0;
    opf.lpstrInitialDir = 0;
    opf.lpstrTitle = _title;
    opf.nFileOffset = 0;
    opf.nFileExtension = 2;
    opf.lpstrDefExt = "*.*";
    opf.lpfnHook = NULL;
    opf.lCustData = 0;
    opf.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    opf.lStructSize = sizeof(OPENFILENAMEW);

    ok = GetOpenFileName(&opf) != 0;
    if ( ok )
        fileName_ = fileName;

    return ok;
}


} // SystemUtils

