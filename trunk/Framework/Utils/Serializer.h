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


#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include <string>

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class Serializer
{
public:
    Serializer();
    virtual ~Serializer();

    bool init(int _bufferSize);
    void done();

protected:
    bool mInitd;
    unsigned char * mBuffer;
    int mBufferSize;
    unsigned char * mCurrentPos;
    int mCurrentSize;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class ISerializerIn
{
public:
    virtual std::string     getString() = 0;
    virtual bool            getBool() = 0;
    virtual char            getChar() = 0;
    virtual unsigned char   getUChar() = 0;
    virtual int             getInt() = 0;
    virtual unsigned int    getUInt() = 0;
    virtual float           getFloat() = 0;
    virtual double          getDouble() = 0;
    virtual void            getBuffer(unsigned char * _buffer, int _bufferSize)=0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class ISerializerOut
{
public:
    virtual void addString(std::string _str) = 0;
    virtual void addBool(bool _val) = 0;
    virtual void addChar(char _val) = 0;
    virtual void addUChar(unsigned char _val) = 0;
    virtual void addInt(int _val) = 0;
    virtual void addUInt(unsigned int _val) = 0;
    virtual void addFloat(float _val) = 0;
    virtual void addDouble(double _val) = 0;
    virtual void addBuffer(unsigned char * _buffer, int _bufferSize) = 0;
    virtual void finalize() = 0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class SerializerIn : public ISerializerIn, public Serializer
{
public:
    SerializerIn();
    virtual ~SerializerIn();

    // --- Serializer ---
    virtual bool init(int _bufferSize);
    virtual void done();

    // --- ISerializerIn ---
    virtual std::string getString();
    virtual bool getBool();
    virtual char getChar();
    virtual unsigned char getUChar();
    virtual int getInt();
    virtual unsigned int getUInt();
    virtual float getFloat();
    virtual double getDouble();
    virtual void getBuffer(unsigned char * _buffer, int _bufferSize);

protected:
    void get(unsigned char * _ptr, int _byteCount);

    virtual int loadBlock(unsigned char * _ptr, int _count)=0;

private:
    typedef Serializer Inherited;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class SerializerOut : public ISerializerOut, public Serializer
{
public:
    SerializerOut();
    virtual ~SerializerOut();

    virtual bool init(int _bufferSize);
    virtual void done();

    // --- ISerializerOut ---
    virtual void addString(std::string _str);
    virtual void addBool(bool _val);
    virtual void addChar(char _val);
    virtual void addUChar(unsigned char _val);
    virtual void addInt(int _val);
    virtual void addUInt(unsigned int _val);
    virtual void addFloat(float _val);
    virtual void addDouble(double _val);
    virtual void addBuffer(unsigned char * _buffer, int _bufferSize);
    virtual void finalize();

protected:
    typedef Serializer Inherited;

    void add(unsigned char * _ptr, int _byteCount);

    virtual void saveBlock(unsigned char * _ptr, int _count)=0;

private:
    bool mFinalized;
};

#endif // _SERIALIZER_H_

