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





