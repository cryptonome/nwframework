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
#ifndef _FILE_SERIALIZER_H_
#define _FILE_SERIALIZER_H_

#include "Serializer.h"

//****************************************************************************
//
//****************************************************************************
class FileSerializerIn : public SerializerIn
{
public:
    FileSerializerIn();
    virtual ~FileSerializerIn();

    bool loadFile(const char * _filename);

private:
    typedef SerializerIn Inherited;

    virtual int loadBlock(unsigned char * _ptr, int _count);
};

//****************************************************************************
//
//****************************************************************************
class FileSerializerOut : public SerializerOut
{
public:
    FileSerializerOut();
    virtual ~FileSerializerOut();

    bool saveFile(const char * _filename);

private:
    typedef SerializerOut Inherited;

    bool mFinalized;
    virtual void finalize();
    virtual void saveBlock(unsigned char * _ptr, int _count);
};

#endif // _FILE_SERIALIZER_H_

