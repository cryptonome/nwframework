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
#ifndef ENCODEUTILS_H_
#define ENCODEUTILS_H_

//********************************************************************
//
//********************************************************************
namespace EncodeUtils
{
    void    encodeHex(char *pBinData, unsigned int uiBinDataSize, char **pAsciiData, unsigned int *uiAsciiDataSize);
	void    encodeBase64(const char* pi8BinData, unsigned int uiBinDataSize, char** ppcBase64Data, unsigned int* puiBase64DataSize);

	bool    decodeAVCSampleHeader (char* pcInfo, unsigned int uiInfoLength, char* pcAsciiHeader, unsigned int* puiAsciiHeaderSize);
};

#endif

