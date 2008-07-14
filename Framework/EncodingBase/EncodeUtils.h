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

