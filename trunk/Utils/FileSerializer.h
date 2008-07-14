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

