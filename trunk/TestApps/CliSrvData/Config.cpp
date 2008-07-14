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
#include "PchNomosVE.h"


#include "Config.h"
#include "DataScript.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Config::init(const char* _fileName, const char* _pathProgram)
{
	bool bOK = true;

	done();

	mPathProgram = _pathProgram;

	// Full path Ini file
	char szIniFile[1024];
	sprintf(szIniFile,"%s\\%s",mPathProgram.c_str(),_fileName);
	mFileName = szIniFile;

	load(mFileName.c_str());

	mInit = true;
	if ( !bOK )
		done();

	return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Config::done()
{
	if ( mInit)
	{
		mInit = false;
	}
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Config::load(const char* _fileName)
{
	bool bOK = true;

	// Load from file
	DataScript* dataScript = new DataScript();
	bOK = dataScript->loadFile(_fileName);
	// TODO: El loadFile deja el dataScript corrupto si no encuentra el fichero
	//       Por ahora utilizamos un dataScript dinamico para poder recrearlo
	//		 si falla la carga 
	if ( !bOK )
	{
		delete dataScript;
		dataScript = new DataScript();
	}

	// Data
	{
		// Paths
		DataScript PathData = (*dataScript)["PATHS"];

		mPathSDP = PathData.getString("SDP", mPathProgram.c_str());
		mPathProjects = PathData.getString("PROJECTS", mPathProgram.c_str());
	}

	delete dataScript;

	return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Config::save(const char* _fileName)
{
	bool bOK = true;

	DataScript dataScript;

	// Data
	if ( bOK)
	{
		// Paths
		DataScript PathData = dataScript.addSection("PATHS");

		PathData.setString("SDP", mPathSDP.c_str());
		PathData.setString("PROJECTS", mPathProjects.c_str());
	}

	// Save to file
	if ( bOK )
		bOK = dataScript.saveFile(_fileName);

	return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Config::save()
{
	bool bOK = true;

	bOK = save(mFileName.c_str());

	return bOK;
}

