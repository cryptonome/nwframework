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
#ifndef _DATA_SCRIPT_H_
#define _DATA_SCRIPT_H_

//#include <mem.h>
#include "TinyXML/tinyxml.h"
#include <string>

typedef TiXmlElement DataElement;
typedef TiXmlDocument DataDocument;

class DataScript
{
public:
    DataScript();
    ~DataScript();

    DataScript(DataScript const & _other, const char * _key=0);     // creates a DataScript pointing to a element of an existing tree (shares tree)

    //--- ---
    DataScript & operator = (DataScript const & _other);    // assigns the content of _other into mRoot (clears mRoot)
    DataScript & operator += (DataScript const & _other);   // adds recursively the content of _other to mRoot (overwrites coincidences)

    // --- ---

    bool loadFile(const char * _file);      // loads a xml based file
    bool saveFile(const char * _file);      // saves a xml based file

    // --- ---

    DataElement * getRoot();
    DataElement const * getRoot() const;

    DataScript operator[] (const char * _name);     // access to subkeys

    // --- ---
    int count() const;      // returns the number of children of the element pointed by mRoot

    DataScript operator[] (int _Nth);   // returns the nth children of the element pointed by mRoot

    // --- ---

    std::string     getString   (const char * _key, const char * _default="");
    bool            getBool     (const char * _key, bool _default=false);
    int             getInt      (const char * _key, int _default=0);
    unsigned int    getUInt     (const char * _key, unsigned int _default=0);
    float           getFloat    (const char * _key, float _default=0.f);
    double          getDouble   (const char * _key, double _default=0.0);

    std::string     getValString   (const char * _default="");
    bool            getValBool     (bool _default=false);
    int             getValInt      (int _default=0);
    unsigned int    getValUInt     (unsigned int _default=0);
    float           getValFloat    (float _default=0.f);
    double          getValDouble   (double _default=0.0);

    // --- ---

	DataScript addSection(const char * _name, bool _overwrite = false);

	void setString(const char * _key, const char * _str);
	void setBool(const char * _key, bool _val);
	void setInt(const char * _key, int _val);
	void setUInt(const char * _key, unsigned int _val);
	void setFloat(const char * _key, float _val);
    void setDouble(const char * _key, double _val);

    void addValString(const char * _str);
    void addValBool(bool _val);
    void addValInt(int _val);
    void addValUInt(unsigned int _val);
    void addValFloat(float _val);
    void addValDouble(double _val);

private:
    struct SharedNode
    {
        DataDocument * mDoc;
        int mUsers;

        //DataNode();
    };

    SharedNode * mSharedNode;
    DataElement * mRoot;

    void unlink();
    void linkTo(DataScript const & _other);

    DataScript(DataScript const & _other, DataElement * _element);
    void cloneElement(DataElement * _dest, DataElement * _src);
    void cloneElementRecursive(DataElement * _dest, DataElement * _src);
};

#endif // _DATA_SCRIPT_H_

