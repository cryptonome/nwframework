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
*
*      Permission is hereby granted, free of charge, to any person obtaining
*      a copy of this software and associated documentation files (the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*      distribute, sublicense, and/or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions:
*
*      The above copyright notice and this permission notice shall be
*      included in all copies or substantial portions of the Software.
*
*      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
*      LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
*      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#include "PchUtils.h"

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