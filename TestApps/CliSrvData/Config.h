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
#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>


//********************************************************************
//
//********************************************************************
class Config
{
public:
	Config() : mInit(false) {}
	~Config() { done(); }

	bool init(const char* _fileName, const char* _pathProgram);
	void done();

	// Paths
	const char* getPathProgram() const { return mPathProgram.c_str(); }
	const char* getPathSDP() const { return mPathSDP.c_str(); }
	void setPathSDP(const char* _path) { mPathSDP = _path; }
	const char* getPathProjects() const { return mPathProjects.c_str(); }
	void setPathProjects(const char* _path) { mPathProjects = _path; }

	bool save();

private:
	bool load(const char* _fileName);
	bool save(const char* _fileName);

	bool mInit;

	std::string mFileName;

	std::string	mPathProgram;
	std::string mPathSDP;
	std::string mPathProjects;
};

#endif





