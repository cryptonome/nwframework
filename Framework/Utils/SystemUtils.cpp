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

