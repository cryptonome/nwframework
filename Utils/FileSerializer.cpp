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
#include "FileSerializer.h"

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
FileSerializer::FileSerializer()
{
}

/*virtual*/ FileSerializer::~FileSerializer()
{
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool FileSerializer::loadFile(const char * _filename)
{
    return false;
}

bool FileSerializer::saveFile(const char * _filename)
{
    return false;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ std::string FileSerializer::getString()
{
    return std::string("");
}

/*virtual*/ bool FileSerializer::getBool()
{
    return false;
}

/*virtual*/ char FileSerializer::getChar()
{
    return ' ';
}

/*virtual*/ unsigned char FileSerializer::getUChar(unsigned char &_val)
{
    return 0;
}

/*virtual*/ int FileSerializer::getInt(int &_val)
{
    return 0;
}

/*virtual*/ unsigned int FileSerializer::getUInt(unsigned int &_val)
{
    return 0;
}

/*virtual*/ float FileSerializer::getFloat(float &_val)
{
    return 0.0f;
}

/*virtual*/ double FileSerializer::getDouble(double &_val)
{
    return 0.0;
}

/*virtual*/ void FileSerializer::getBuffer(unsigned char * _buffer, int _bufferSize)
{
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void FileSerializer::addString(const char * _str)
{
}

/*virtual*/ void FileSerializer::addBool(bool _val)
{
}

/*virtual*/ void FileSerializer::addChar(char _val)
{
}

/*virtual*/ void FileSerializer::addUChar(unsigned char _val)
{
}

/*virtual*/ void FileSerializer::addInt(int _val)
{
}

/*virtual*/ void FileSerializer::addUInt(unsigned int _val)
{
}

/*virtual*/ void FileSerializer::addFloat(float _val)
{
}

/*virtual*/ void FileSerializer::addDouble(double _val)
{
}

/*virtual*/ void FileSerializer::addBuffer(unsigned char * _buffer, int _bufferSize)
{
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void saveBlock(unsigned char * _ptr, int _count)
{
}

/*virtual*/ void loadBlock(unsigned char * _ptr, int _count)
{
}

