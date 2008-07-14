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
#include "PchUtils.h"
#include "FileSerializer.h"

//****************************************************************************
//
//****************************************************************************
static const int MAX_BUFFER_SIZE = 32 * 1024;
//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
FileSerializerIn::FileSerializerIn() : Inherited()
{
}

/*virtual*/ FileSerializerIn::~FileSerializerIn()
{
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool FileSerializerIn::loadFile(const char * _filename)
{
    return false;
}

//****************************************************************************
//
//****************************************************************************
/*virtual*/ int FileSerializerIn::loadBlock(unsigned char * _ptr, int _count)
{
    return 0;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
FileSerializerOut::FileSerializerOut() : Inherited(),
    mFinalized(false)
{
}

/*virtual*/ FileSerializerOut::~FileSerializerOut()
{
    if(!mFinalized)
        finalize();
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool FileSerializerOut::saveFile(const char * _filename)
{
    return false;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void FileSerializerOut::finalize()
{
    Inherited::finalize();

    // zzz CloseFile();
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void FileSerializerOut::saveBlock(unsigned char * _ptr, int _count)
{
}
