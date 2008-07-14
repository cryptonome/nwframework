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


#include "PchEncodingBase.h"

#include "EncodeUtils.h"
#include <stdio.h>
#include <string.h>

//********************************************************************
//
//********************************************************************
namespace EncodeUtils
{

// ------------------------------------
const char pcBase64Digit [] =
{
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
	'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
	'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
	'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
};

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void encodeHex(char *pBinData, unsigned int uiBinDataSize, char **pAsciiData, unsigned int *uiAsciiDataSize)
{
	*uiAsciiDataSize = uiBinDataSize*2+1; // +1 for termination with zero byte

	*pAsciiData = new char[*uiAsciiDataSize];

	for (unsigned int i=0; i<uiBinDataSize; i++)
	{
		sprintf((*pAsciiData)+i*2, "%02x", (unsigned char)(pBinData[i]));
	}
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void encodeBase64 (const char* pi8BinData,  unsigned int uiBinDataSize, char** ppcBase64Data, unsigned int* puiBase64DataSize)
{
	unsigned int	uiBufPos;
	unsigned int	i;
	unsigned char	uiSrc0;
	unsigned char	uiSrc1;
	unsigned char	uiSrc2;
	char*			pcDest;

	pcDest = new char [(((uiBinDataSize + 2) * 4) / 3) + 1];

	uiBufPos = 0;
	for (i = 0; i < uiBinDataSize / 3; i++)
	{
		uiSrc0 = (unsigned char)pi8BinData[3*i];
		uiSrc1 = (unsigned char)pi8BinData[3*i+1];
		uiSrc2 = (unsigned char)pi8BinData[3*i+2];

		pcDest[uiBufPos++] = pcBase64Digit [uiSrc0 >> 2];
		pcDest[uiBufPos++] = pcBase64Digit [((uiSrc0 & 0x03) << 4) | (uiSrc1 >> 4)];
		pcDest[uiBufPos++] = pcBase64Digit [((uiSrc1 & 0x0f) << 2) | (uiSrc2 >> 6)];
		pcDest[uiBufPos++] = pcBase64Digit [uiSrc2 & 0x3f];
	}

	if (1 == uiBinDataSize % 3) 
	{
		uiSrc0 = (unsigned char)pi8BinData[uiBinDataSize-1];
		pcDest[uiBufPos++] = pcBase64Digit[uiSrc0 >> 2];
		pcDest[uiBufPos++] = pcBase64Digit[((uiSrc0 & 0x03) << 4)];
		pcDest[uiBufPos++] = '=';
		pcDest[uiBufPos++] = '=';
	}
	else if (2 == uiBinDataSize % 3) 
	{
		uiSrc0 = (unsigned char)pi8BinData[uiBinDataSize-2];
		uiSrc1 = (unsigned char)pi8BinData[uiBinDataSize-1];
		pcDest[uiBufPos++] = pcBase64Digit[uiSrc0 >> 2];
		pcDest[uiBufPos++] = pcBase64Digit[((uiSrc0 & 0x03) << 4) | (uiSrc1 >> 4)];
		pcDest[uiBufPos++] = pcBase64Digit[((uiSrc1 & 0x0f) << 2)];
		pcDest[uiBufPos++] = '=';
	}

	pcDest[uiBufPos] = '\0';

	*puiBase64DataSize	= uiBufPos;
	*ppcBase64Data		= pcDest;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool decodeAVCSampleHeader (char* pcInfo, unsigned int uiInfoLength, char* pcAsciiHeader, unsigned int* puiAsciiHeaderSize)
{
	bool bOK = true;


	unsigned int uiBuffersize = *puiAsciiHeaderSize;

	*puiAsciiHeaderSize = 0;

/*	unsigned char ui8ConfigVersion			= pcInfo [0];			// configuration version
	unsigned char ui8AVCProfileIndication	= pcInfo [1];
	unsigned char ui8ProfileCompatibility	= pcInfo [2];
	unsigned char ui8AVCLevelIndication		= pcInfo [3];
	unsigned char ui8LengthSize				= pcInfo [4];*/

	int iLength = 5;

	unsigned char ui8NumOfSPS				= pcInfo [iLength++] & 0x1f;
	unsigned char ui8SPSIdx = 0;

	if ((pcAsciiHeader != NULL) && (uiBuffersize >= 1))
	{
		strcpy (pcAsciiHeader, "");
	}

	char* pcHeaderAsciiData = NULL;
	unsigned int uiAsciiDataSize = 0;

	// SPS header
	while (ui8SPSIdx < ui8NumOfSPS)
	{
		unsigned char uiSPSLength = 0;
		uiSPSLength = (uiSPSLength << 8) | pcInfo [iLength++];
		uiSPSLength = (uiSPSLength << 8) | pcInfo [iLength++];

		encodeBase64 (pcInfo + iLength, uiSPSLength, &pcHeaderAsciiData, &uiAsciiDataSize);
		iLength += uiSPSLength;

		if ((pcAsciiHeader != NULL) && (uiBuffersize >= *puiAsciiHeaderSize + 1))
		{
			if (ui8SPSIdx > 0)
			{
				strcat (pcAsciiHeader, ",");
			}
			strncat (pcAsciiHeader, pcHeaderAsciiData, uiAsciiDataSize);
		}
		else
		{
			bOK = false;
		}

		*puiAsciiHeaderSize	+= uiAsciiDataSize;
		*puiAsciiHeaderSize	+= (ui8SPSIdx > 0) ? 1 : 0;

		delete [] pcHeaderAsciiData;
		pcHeaderAsciiData = NULL;
		uiAsciiDataSize = 0;

		ui8SPSIdx++;
	}

	unsigned char ui8NumOfPPS	= pcInfo [iLength++];
	unsigned char ui8PPSIdx = 0;

	// PPS header
	while (ui8PPSIdx < ui8NumOfPPS)
	{
		unsigned char uiSPSLength = 0;
		uiSPSLength = (uiSPSLength << 8) | pcInfo [iLength++];
		uiSPSLength = (uiSPSLength << 8) | pcInfo [iLength++];

		encodeBase64 (pcInfo + iLength, uiSPSLength, &pcHeaderAsciiData, &uiAsciiDataSize);
		if ((pcAsciiHeader != NULL) && (uiBuffersize >= *puiAsciiHeaderSize))
		{
			strcat (pcAsciiHeader, ",");
			strncat (pcAsciiHeader, pcHeaderAsciiData, uiAsciiDataSize);
		}
		else
		{
			bOK = false;
		}

		*puiAsciiHeaderSize	+= uiAsciiDataSize + 1;

		delete [] pcHeaderAsciiData;
		pcHeaderAsciiData = NULL;
		uiAsciiDataSize = 0;

		ui8PPSIdx++;
	}

	return bOK;
}


} // EncodeUtils

