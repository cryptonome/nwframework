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

