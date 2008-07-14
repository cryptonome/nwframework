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

#include <string>

class FileSerializer : public Serializer
{
public:
    FileSerializer();
    virtual ~FileSerializer();

    bool loadFile(const char * _filename);
    bool saveFile(const char * _filename);

    virtual std::string getString();
    virtual bool getBool();
    virtual char getChar();
    virtual unsigned char getUChar(unsigned char &_val);
    virtual int getInt(int &_val);
    virtual unsigned int getUInt(unsigned int &_val);
    virtual float getFloat(float &_val);
    virtual double getDouble(double &_val);
    virtual void getBuffer(unsigned char * _buffer, int _bufferSize);

    virtual void addString(const char * _str);
    virtual void addBool(bool _val);
    virtual void addChar(char _val);
    virtual void addUChar(unsigned char _val);
    virtual void addInt(int _val);
    virtual void addUInt(unsigned int _val);
    virtual void addFloat(float _val);
    virtual void addDouble(double _val);
    virtual void addBuffer(unsigned char * _buffer, int _bufferSize);

private:
    virtual void saveBlock(unsigned char * _ptr, int _count);
    virtual void loadBlock(unsigned char * _ptr, int _count);

};

#endif // _FILE_SERIALIZER_H_

