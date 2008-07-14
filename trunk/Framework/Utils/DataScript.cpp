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
#include "DataScript.h"
#include "StrUtils.h"

//****************************************************************************
//
//****************************************************************************
typedef TiXmlDeclaration DataDeclaration;

static const char * XML_HEADER_VERSION = "1.0";
static const char * XML_HEADER_ENCODING = "UTF-8";
static const char * XML_HEADER_EXTRA = "";

static const char * ROOT_SECTION_NAME = "Root";

static const char * SCRIPT_ID_STRING    = "Str";
static const char * SCRIPT_ID_BOOL      = "Bool";
static const char * SCRIPT_ID_INT       = "Int";
static const char * SCRIPT_ID_UINT      = "UInt";
static const char * SCRIPT_ID_FLOAT     = "Float";
static const char * SCRIPT_ID_DOUBLE    = "Double";

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
DataScript::DataScript()
{
    mSharedNode = NEW SharedNode;
    mSharedNode->mDoc = NEW DataDocument();
    mSharedNode->mUsers = 1;

    DataDeclaration * decl = NEW DataDeclaration( XML_HEADER_VERSION, XML_HEADER_ENCODING, XML_HEADER_EXTRA );
	mSharedNode->mDoc->LinkEndChild( decl );

    mRoot = NEW DataElement(ROOT_SECTION_NAME);
    mSharedNode->mDoc->LinkEndChild(mRoot);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
DataScript::~DataScript()
{
    unlink();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
DataScript::DataScript(DataScript const & _other, const char * _key/*=0*/)
{
    linkTo(_other);

    if(_key)
	{
		DataElement * elem = mRoot->FirstChildElement(_key);

        if(elem)
        {
            mRoot = elem;
        }
        else
        {
            DataElement * dataElement = NEW DataElement(_key);
            mRoot->LinkEndChild(dataElement);
            mRoot = dataElement;
        }

        ASSERT(mRoot);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
DataScript::DataScript(DataScript const & _other, DataElement * _element/*=0*/)
{
    linkTo(_other);

    mRoot = _element;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
DataScript & DataScript::operator = (DataScript const & _other)
{
    DataElement * src = _other.mRoot;
    DataElement * dest = mRoot;

    // if src or dest are root elements (but not both) use a special per element copy
    if((src == _other.mSharedNode->mDoc->LastChild()) ^ (dest == mSharedNode->mDoc->LastChild()))
    {
        if(dest == mSharedNode->mDoc->LastChild())  // if dest is a root just erase his children and attrs
        {
            dest->ClearThis();
        }

        cloneElement(dest, src);
    }
    else
    {
        *mRoot = *_other.mRoot;      // uses TiXmlElement::operator= to clone _other and link copy as new root (first clears the current contents of mRoot)
    }

    return *this;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
DataScript & DataScript::operator += (DataScript const & _other)
{
    DataElement * src = _other.mRoot;
    DataElement * dest = mRoot;

    cloneElementRecursive(dest, src);

    return *this;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::cloneElement(DataElement * _dest, DataElement * _src)
{
    DataElement * source = _src->FirstChildElement(); // copy src elements
    DataElement * destination = 0;
    while(source)
    {
        destination = _dest->FirstChildElement(source->Value());
        if(destination)
        {
            *destination = *source;
        }
        else
        {
            destination = _dest;
            DataElement * clonedSource = (DataElement *)source->Clone();
            destination->LinkEndChild(clonedSource);
        }

        source = source->NextSiblingElement();
    }

    const TiXmlAttribute* attribute = 0;
    for(attribute = _src->FirstAttribute(); attribute; attribute = attribute->Next())
    {
        _dest->SetAttribute( attribute->Name(), attribute->Value() );
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::cloneElementRecursive(DataElement * _dest, DataElement * _src)
{
    DataElement * source = _src->FirstChildElement(); // copy src elements
    DataElement * destination = 0;
    while(source)
    {
        destination = _dest->FirstChildElement(source->Value());
        if(destination && destination->FirstChildElement())
        {
            cloneElementRecursive(destination, source);
        }
        else
        {
            destination = _dest;
            DataElement * clonedSource = (DataElement *)source->Clone();
            destination->LinkEndChild(clonedSource);
        }

        source = source->NextSiblingElement();
    }

    const TiXmlAttribute* attribute = 0;
    for(attribute = _src->FirstAttribute(); attribute; attribute = attribute->Next())
    {
        _dest->SetAttribute( attribute->Name(), attribute->Value() );
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool DataScript::loadFile(const char * _file)
{
    bool bRet = false;

    unlink();

    mSharedNode = NEW SharedNode;

    mSharedNode->mDoc = NEW DataDocument(_file);
    if(mSharedNode->mDoc->LoadFile(TIXML_ENCODING_UTF8))
    {
        mRoot = mSharedNode->mDoc->FirstChildElement(ROOT_SECTION_NAME);
        ASSERT(mRoot);

        if(mRoot)
        {
            bRet = true;
        }
    }

    mSharedNode->mUsers = 1;

    return bRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool DataScript::saveFile(const char * _file)
{
    return mSharedNode->mDoc->SaveFile(_file);
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
DataElement * DataScript::getRoot()
{
    return mRoot;
}

DataElement const * DataScript::getRoot() const
{
    return mRoot;
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
DataScript DataScript::operator[] (const char * _key)
{
    return DataScript(*this, _key);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int DataScript::count() const
{
    int num = 0;

    DataElement const * element = getRoot()->FirstChildElement();
    while(element)
    {
        num++;
        element = element->NextSiblingElement();
    }

    return num;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
DataScript DataScript::operator[] (int _Nth) 
{
    DataElement * element = 0;

    int num = 0;
    DataElement * iter = getRoot()->FirstChildElement();
    while(iter)
    {
        if(num == _Nth)
        {
            element = iter;
            break;
		}
        num++;
        iter = iter->NextSiblingElement();
    }

	ASSERT(element);

    return DataScript(*this, element);
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
std::string DataScript::getString(const char * _key, const char * _default/*=""*/)
{
    const char * attr = 0;
    std::string ansiStr;

    DataElement * dataElement = mRoot->FirstChildElement(_key);
    if(dataElement)
    {
        attr = dataElement->Attribute(SCRIPT_ID_STRING);

        if(attr)
        {
            ansiStr = StrUtils::Utf8ToAnsi(attr);
        }
    }

    return std::string(attr ? ansiStr.c_str() : _default);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool DataScript::getBool(const char * _key, bool _default/*=false*/)
{
    int value = (int)_default;

    DataElement * dataElement = mRoot->FirstChildElement(_key);
    if(dataElement)
    {
        dataElement->QueryIntAttribute( SCRIPT_ID_BOOL, &value );
    }

    return value != 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int DataScript::getInt(const char * _key, int _default/*=0*/)
{
    int value = _default;

    DataElement * dataElement = mRoot->FirstChildElement(_key);
    if(dataElement)
    {
        dataElement->QueryIntAttribute( SCRIPT_ID_INT, &value );
    }

    return value;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
unsigned int DataScript::getUInt(const char * _key, unsigned int _default/*=0*/)
{
    int value = (int)_default;

    DataElement * dataElement = mRoot->FirstChildElement(_key);
    if(dataElement)
    {
        dataElement->QueryIntAttribute( SCRIPT_ID_UINT, &value );
    }

    return (unsigned int)value;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
float DataScript::getFloat(const char * _key, float _default/*=0.f*/)
{
    float value = _default;

    DataElement * dataElement = mRoot->FirstChildElement(_key);
    if(dataElement)
    {
        dataElement->QueryFloatAttribute( SCRIPT_ID_FLOAT, &value );
    }

    return value;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
double DataScript::getDouble(const char * _key, double _default/*=0.0*/)
{
    double value = _default;

    DataElement * dataElement = mRoot->FirstChildElement(_key);
    if(dataElement)
    {
        dataElement->QueryDoubleAttribute( SCRIPT_ID_DOUBLE, &value );
    }

    return value;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
std::string DataScript::getValString(const char * _default/*=""*/)
{
    const char * attr = mRoot->Attribute(SCRIPT_ID_STRING);
    std::string ansiStr;

    if(attr)
    {
        ansiStr = StrUtils::Utf8ToAnsi(attr);
    }

    return std::string(attr ? ansiStr.c_str() : _default);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool DataScript::getValBool(bool _default/*=false*/)
{
    int value = (int)_default;

    mRoot->QueryIntAttribute( SCRIPT_ID_BOOL, &value );

    return value != 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int DataScript::getValInt(int _default/*=0*/)
{
    int value = _default;

    mRoot->QueryIntAttribute( SCRIPT_ID_INT, &value );

    return value;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
unsigned int DataScript::getValUInt(unsigned int _default/*=0*/)
{
    int value = (int)_default;

    mRoot->QueryIntAttribute( SCRIPT_ID_UINT, &value );

    return (unsigned int)value;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
float DataScript::getValFloat(float _default/*=0.f*/)
{
    float value = _default;

    mRoot->QueryFloatAttribute( SCRIPT_ID_FLOAT, &value );

    return value;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
double DataScript::getValDouble(double _default/*=0.0*/)
{
    double value = _default;

    mRoot->QueryDoubleAttribute( SCRIPT_ID_DOUBLE, &value );

    return value;
}

//****************************************************************************
//
//****************************************************************************
DataScript DataScript::addSection(const char * _name, bool _overwrite)
{
	DataElement * dataElement = 0;
	if ( _overwrite )
	{
		dataElement = mRoot->FirstChildElement(_name);
	}
	if ( dataElement == 0 )
	{
		dataElement = NEW DataElement(_name);
		mRoot->LinkEndChild(dataElement);
	}

    return DataScript(*this, dataElement);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::addValString(const char * _str)
{
    std::string ansiStr = StrUtils::AnsiToUtf8(_str);

    mRoot->SetAttribute(SCRIPT_ID_STRING, ansiStr.c_str());
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::addValBool(bool _val)
{
    int value = (int)_val;
    mRoot->SetAttribute(SCRIPT_ID_BOOL, value);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::addValInt(int _val)
{
    mRoot->SetAttribute(SCRIPT_ID_INT, _val);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::addValUInt(unsigned int _val)
{
    int value = (int)_val;
    mRoot->SetAttribute(SCRIPT_ID_UINT, value);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::addValFloat(float _val)
{
    double value = (double)_val;
    mRoot->SetDoubleAttribute(SCRIPT_ID_FLOAT, value);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::addValDouble(double _val)
{
    mRoot->SetDoubleAttribute(SCRIPT_ID_DOUBLE, _val);
}

//******************************************************************************
//
//******************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::setString(const char * _key, const char * _str)
{
    std::string ansiStr = StrUtils::AnsiToUtf8(_str);

	DataScript dataScript = DataScript(*this,_key);
	DataElement* dataElement = dataScript.mRoot;

	dataElement->SetAttribute(SCRIPT_ID_STRING, ansiStr.c_str());
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::setBool(const char * _key, bool _val)
{
	DataScript dataScript = DataScript(*this,_key);
	DataElement* dataElement = dataScript.mRoot;

	int value = (int)_val;
	dataElement->SetAttribute(SCRIPT_ID_BOOL, value);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::setInt(const char * _key, int _val)
{
	DataScript dataScript = DataScript(*this,_key);
	DataElement* dataElement = dataScript.mRoot;

	dataElement->SetAttribute(SCRIPT_ID_INT, _val);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::setUInt(const char * _key, unsigned int _val)
{
	DataScript dataScript = DataScript(*this,_key);
	DataElement* dataElement = dataScript.mRoot;

	int value = (int)_val;
	dataElement->SetAttribute(SCRIPT_ID_UINT, value);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::setFloat(const char * _key, float _val)
{
	DataScript dataScript = DataScript(*this,_key);
	DataElement* dataElement = dataScript.mRoot;

	double value = (double)_val;
	dataElement->SetDoubleAttribute(SCRIPT_ID_FLOAT, value);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::setDouble(const char * _key, double _val)
{
	DataScript dataScript = DataScript(*this,_key);
	DataElement* dataElement = dataScript.mRoot;

	dataElement->SetDoubleAttribute(SCRIPT_ID_DOUBLE, _val);
}

//******************************************************************************
//
//******************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::unlink()
{
	if(mSharedNode)
	{
		mSharedNode->mUsers--;

		if(mSharedNode->mUsers == 0)
		{
			if(mSharedNode->mDoc)
			{
				DISPOSE(mSharedNode->mDoc);
                mSharedNode->mDoc = 0;
            }

            DISPOSE(mSharedNode);
            mSharedNode = 0;
        }
    }

    mRoot = 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DataScript::linkTo(DataScript const & _other)
{
    mSharedNode = _other.mSharedNode;
    mSharedNode->mUsers++;

    mRoot = _other.mRoot;
}

