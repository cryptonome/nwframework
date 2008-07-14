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

