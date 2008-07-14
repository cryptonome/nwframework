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
#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include <string>

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class ISerializerIn
{
public:
    virtual std::string getString() = 0;
    virtual bool getBool() = 0;
    virtual char getChar() = 0;
    virtual unsigned char getUChar(unsigned char &_val) = 0;
    virtual int getInt(int &_val) = 0;
    virtual unsigned int getUInt(unsigned int &_val) = 0;
    virtual float getFloat(float &_val) = 0;
    virtual double getDouble(double &_val) = 0;
    virtual void getBuffer(unsigned char * _buffer, int _bufferSize)=0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class ISerializerOut
{
public:
    virtual void addString(const char * _str) = 0;
    virtual void addBool(bool _val) = 0;
    virtual void addChar(char _val) = 0;
    virtual void addUChar(unsigned char _val) = 0;
    virtual void addInt(int _val) = 0;
    virtual void addUInt(unsigned int _val) = 0;
    virtual void addFloat(float _val) = 0;
    virtual void addDouble(double _val) = 0;
    virtual void addBuffer(unsigned char * _buffer, int _bufferSize) = 0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class Serializer : public ISerializerIn, public ISerializerOut
{
public:
    Serializer(int _bufferSize=2048);
    virtual ~Serializer();

protected:
    void add(void * _ptr, int _byteCount);
    void get(void * _ptr, int _byteCount);

    virtual void saveBlock(unsigned char * _ptr, int _count)=0;
    virtual void loadBlock(unsigned char * _ptr, int _count)=0;

private:
    unsigned char * mBuffer;
};

#endif // _SERIALIZER_H_

