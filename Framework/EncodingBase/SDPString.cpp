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

#include "SDPString.h"
#include <stdio.h>
#include <time.h>


bool SDPString::SaveToFile(const char* pszFileName) const
{
	bool bOK = true;

	// Open file
	FILE* pSdpFile = NULL;
	pSdpFile = fopen(pszFileName, "wt");
	bOK = pSdpFile != 0;

	// Save data
	if ( bOK )
	{
		fprintf (pSdpFile,"%s", m_pSdpString.c_str());
		fclose (pSdpFile);
	}

	return bOK;
}

// ------------------------------

std::string SDPString::getMpeg4MediaSection(	unsigned int	uiVideoDestPort,
											unsigned int	uiVideoPayloadType,
											unsigned int	uiVideoRTPStreamTimeScale,
											const char*			pcAsciiHeader,
											int				iVideoAverageBitrate,
											unsigned int  	uiVideoHeight,
											unsigned int  	uiVideoWidth,
											unsigned int	 uiVideoProfileLevel )
{
	std::string pMpeg4MediaSection;

	char			pcSDPString[4096];
	sprintf (pcSDPString, "m=video %u RTP/AVP %u\n", uiVideoDestPort, uiVideoPayloadType);
	pMpeg4MediaSection += pcSDPString;
	sprintf (pcSDPString, "b=AS:%u\n", iVideoAverageBitrate/1000);
	pMpeg4MediaSection += pcSDPString;
	sprintf (pcSDPString, "a=rtpmap:%u MP4V-ES/%u\n", uiVideoPayloadType, uiVideoRTPStreamTimeScale);
	pMpeg4MediaSection += pcSDPString;
	sprintf (pcSDPString, "a=mpeg4-esid:201\n");		// previously it was 2
	pMpeg4MediaSection += pcSDPString;
	sprintf (pcSDPString, "a=cliprect:0, 0, %u, %u\n", uiVideoHeight, uiVideoWidth);
	pMpeg4MediaSection += pcSDPString;
	sprintf (pcSDPString, "a=fmtp:%u profile-level-id=%u; config=%s;\n", uiVideoPayloadType, uiVideoProfileLevel, pcAsciiHeader);
	pMpeg4MediaSection += pcSDPString;
	sprintf (pcSDPString, "a=control:trackID=1\n");
	pMpeg4MediaSection += pcSDPString;

	return pMpeg4MediaSection;
}

// ------------------------------

std::string SDPString::getH263MediaSection(	unsigned int	uiVideoDestPort,
												unsigned int	uiVideoPayloadType,
												unsigned int	uiVideoRTPStreamTimeScale,
												int				iVideoAverageBitrate)
{
	char			pcSDPString[4096];
	std::string		pH263MediaSection;

	sprintf (pcSDPString, "m=video %u RTP/AVP %u\n", uiVideoDestPort, uiVideoPayloadType);
	pH263MediaSection += pcSDPString;
	sprintf (pcSDPString, "b=AS:%u\n", iVideoAverageBitrate/1000);
	pH263MediaSection += pcSDPString;
	sprintf (pcSDPString, "a=rtpmap:%u H263-2000/%u\n", uiVideoPayloadType, uiVideoRTPStreamTimeScale);
	pH263MediaSection += pcSDPString;
	sprintf (pcSDPString, "a=fmtp:%u profile=0;level=10\n", uiVideoPayloadType);
	pH263MediaSection += pcSDPString;
	sprintf (pcSDPString, "a=control:trackID=1\n");
	pH263MediaSection += pcSDPString;

	return pH263MediaSection;
}

// ------------------------------

std::string SDPString::getH264MediaSection(	unsigned int	uiVideoDestPort,
												unsigned int	uiVideoPayloadType,
												unsigned int	uiVideoRTPStreamTimeScale,
												const char*			pcAsciiHeader,
												int				iVideoAverageBitrate,
												int				uiVideoHeight,
												int				uiVideoWidth )
{
	char			pcSDPString[4096];
	std::string	   	pH264MediaSection;

	// Media section Video
	sprintf (pcSDPString, "m=video %u RTP/AVP %u\n", uiVideoDestPort, uiVideoPayloadType);
	pH264MediaSection += pcSDPString;

	// Average bitrate
	sprintf (pcSDPString, "b=AS:%u\n", iVideoAverageBitrate/1000);
	pH264MediaSection += pcSDPString;

	// Codec Type and time scale
	sprintf (pcSDPString, "a=rtpmap:%u H264/%u\n", uiVideoPayloadType, uiVideoRTPStreamTimeScale);
	pH264MediaSection += pcSDPString;

	// Frame size
	sprintf (pcSDPString, "a=cliprect:0, 0, %u, %u\n", uiVideoHeight, uiVideoWidth);
	pH264MediaSection += pcSDPString;

	sprintf (pcSDPString, "a=framesize:%u %u-%u\n", uiVideoPayloadType, uiVideoWidth, uiVideoHeight);
	pH264MediaSection += pcSDPString;

	// Decoder config data
	sprintf (pcSDPString, "a=fmtp:%u packetization-mode=1;profile-level-id=%s;sprop-parameter-sets=%s\n", 
		uiVideoPayloadType, "42E00D", pcAsciiHeader);

	pH264MediaSection += pcSDPString;
	sprintf (pcSDPString, "a=control:trackID=1\n");
	pH264MediaSection += pcSDPString;

	return pH264MediaSection;
}

// ------------------------------

std::string SDPString::getAac3gppMediaSection(	unsigned int	uiAudioDestPort,
												unsigned int	uiAudioPayloadType,
												unsigned int	uiAudioRTPStreamTimeScale,
												const char*			pcAsciiHeader,
												int*			piAudioAverageBitrate)
{
	char			pcSDPString[4096];
	std::string	 	pAacMediaSection;

	sprintf (pcSDPString, "m=audio %u RTP/AVP %u\n", uiAudioDestPort, uiAudioPayloadType);
	pAacMediaSection += pcSDPString;
	sprintf (pcSDPString, "b=AS:%u\n", *piAudioAverageBitrate/1000);
	pAacMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=rtpmap:%u MP4A-LATM/%u\n", uiAudioPayloadType, uiAudioRTPStreamTimeScale);
	pAacMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=fmtp:%u profile-level-id=15; object=2; cpresent=0; config=%s\n", uiAudioPayloadType, pcAsciiHeader);
	pAacMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=control:trackID=2\n");
	pAacMediaSection += pcSDPString;

	return pAacMediaSection;
}

// ------------------------------

std::string SDPString::getAacIsmaMediaSection(	unsigned int	uiAudioDestPort,
												unsigned int	uiAudioPayloadType,
												unsigned int	uiAudioRTPStreamTimeScale,
												const char*			pcAsciiHeader,
												int*			piAudioAverageBitrate,
												int				uiAudioChannels)
{
	char			pcSDPString[4096];
	std::string	 	pAacMediaSection;

	sprintf (pcSDPString, "m=audio %u RTP/AVP %u\n", uiAudioDestPort, uiAudioPayloadType);
	pAacMediaSection += pcSDPString;
	sprintf (pcSDPString, "b=AS:%u\n", *piAudioAverageBitrate/1000);
	pAacMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=rtpmap:%u mpeg4-generic/%u/%u\n", uiAudioPayloadType, uiAudioRTPStreamTimeScale, uiAudioChannels);
	pAacMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=fmtp:%u streamtype=5; profile-level-id=15; mode=AAC-hbr; config=%s;  SizeLength=13; IndexLength=3; IndexDeltaLength=3; Profile=1;\n", uiAudioPayloadType, pcAsciiHeader);
	pAacMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=control:trackID=2\n");
	pAacMediaSection += pcSDPString;

	return pAacMediaSection;
}

// ------------------------------

std::string SDPString::getAmrMediaSection(		unsigned int	uiAudioDestPort,
												unsigned int	uiAudioPayloadType,
												unsigned int	uiAudioRTPStreamTimeScale,
												const char*			pcAsciiHeader,
												int*			piAudioAverageBitrate)
{
	char			pcSDPString[4096];
	std::string	   	pAmrMediaSection;

	// En el .pas este valor esta harcodeado a 20000
///	piAudioAverageBitrate

	sprintf (pcSDPString, "m=audio %u RTP/AVP %u\n", uiAudioDestPort, uiAudioPayloadType);
	pAmrMediaSection += pcSDPString;
	sprintf (pcSDPString, "b=AS:%u\n", *piAudioAverageBitrate/1000);
	pAmrMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=rtpmap:%u AMR/%u\n", uiAudioPayloadType, uiAudioRTPStreamTimeScale);
	pAmrMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=fmtp:%u channels=1;octet-align=1\n", uiAudioPayloadType);
	pAmrMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=maxptime:%u\n", 20 * 8);	// 20ms per AMR sample * 8 packets per sample
	pAmrMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=control:trackID=2\n");
	pAmrMediaSection += pcSDPString;

	return pAmrMediaSection;
}

std::string SDPString::getAmrWBMediaSection(		unsigned int	uiAudioDestPort,
												unsigned int	uiAudioPayloadType,
												unsigned int	uiAudioRTPStreamTimeScale,
												const char*			pcAsciiHeader,
												int*			piAudioAverageBitrate)
{
	char			pcSDPString[4096];
	std::string	   	pAmrMediaSection;

	// En el .pas este valor esta harcodeado a 20000
///	piAudioAverageBitrate

	sprintf (pcSDPString, "m=audio %u RTP/AVP %u\n", uiAudioDestPort, uiAudioPayloadType);
	pAmrMediaSection += pcSDPString;
	sprintf (pcSDPString, "b=AS:%u\n", *piAudioAverageBitrate/1000);
	pAmrMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=rtpmap:%u AMR-WB/%u\n", uiAudioPayloadType, uiAudioRTPStreamTimeScale);
	pAmrMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=fmtp:%u channels=1;octet-align=1\n", uiAudioPayloadType);
	pAmrMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=maxptime:%u\n", 20 * 8);	// 20ms per AMR sample * 8 packets per sample
	pAmrMediaSection += pcSDPString;
	sprintf (pcSDPString, "a=control:trackID=2\n");
	pAmrMediaSection += pcSDPString;

	return pAmrMediaSection;
}


// ------------------------------

std::string SDPString::getIsmaSessionSection(	const char* pcSourceIPAdr,
												const char* pcDestIP,
												const char* pcAsciiHeader,
												int	iAverageBitrate,
												int iIsmaVersion, const char* pszSessionName)
{
	char			pcSDPString[4096];
	std::string	 	pIsmaSessionSection;

	unsigned int	uiSessionID = (unsigned int)clock();

	sprintf (pcSDPString, "v=0\n");
	pIsmaSessionSection += pcSDPString;
	sprintf (pcSDPString, "o=- %u %u IN IP4 %s\n", uiSessionID, uiSessionID+8, pcSourceIPAdr);
	pIsmaSessionSection += pcSDPString;
	sprintf (pcSDPString, "s=NomosVE Stream Encoder - %s\n",pszSessionName);   
	pIsmaSessionSection += pcSDPString;
	sprintf (pcSDPString, "c=IN IP4 %s\n", pcDestIP);
	pIsmaSessionSection += pcSDPString;
	if (1 == iIsmaVersion)
	{
		sprintf (pcSDPString, "a=isma-compliance:1,1.0,1\n");
		pIsmaSessionSection += pcSDPString;
		sprintf (pcSDPString, "a=mpeg4-iod: \"data:application/mpeg4-iod;base64,%s\"\n", pcAsciiHeader);
		pIsmaSessionSection += pcSDPString;
	}
	else if (2 == iIsmaVersion)
	{
		sprintf (pcSDPString, "a=isma-compliance:2,2.0,2.0\n");
		pIsmaSessionSection += pcSDPString;
	}
	sprintf (pcSDPString, "b=AS:%u\n", iAverageBitrate/1000);
	pIsmaSessionSection += pcSDPString;
	sprintf (pcSDPString, "a=control:*\n");
	pIsmaSessionSection += pcSDPString;
	sprintf (pcSDPString, "t=0 0\n");
	pIsmaSessionSection += pcSDPString;

	return pIsmaSessionSection;
}

// ------------------------------

std::string SDPString::get3gppSessionSection(	const char*			pcSourceIPAdr,
													const char*			pcDestIPAdr,
													int				iAverageBitrate, const char* pszSessionName)
{
	unsigned int	uiSessionID = (unsigned int)clock();

	char			pcSDPString[4096];
	std::string			p3gppSessionSection;

	sprintf (pcSDPString, "v=0\n");
	p3gppSessionSection += pcSDPString;
	sprintf (pcSDPString, "o=- %u %u IN IP4 %s\n", uiSessionID, uiSessionID+8, pcSourceIPAdr);
	p3gppSessionSection += pcSDPString;
	sprintf (pcSDPString, "s=NomosVE Stream Encoder - %s\n",pszSessionName);
	p3gppSessionSection += pcSDPString;
	sprintf (pcSDPString, "c=IN IP4 %s\n", pcDestIPAdr);//0.0.0.0\n");
	p3gppSessionSection += pcSDPString;
	sprintf (pcSDPString, "b=AS:%u\n", iAverageBitrate/1000);
	p3gppSessionSection += pcSDPString;
	sprintf (pcSDPString, "a=control:*\n");
	p3gppSessionSection += pcSDPString;
	sprintf (pcSDPString, "a=range:npt=now-\n");
	p3gppSessionSection += pcSDPString;
	sprintf (pcSDPString, "a=recvonly\n");
	p3gppSessionSection += pcSDPString;
	sprintf (pcSDPString, "t=0 0\n");
	p3gppSessionSection += pcSDPString;

	return p3gppSessionSection;
}

// ------------------------------

std::string SDPString::getGlobalSessionSection (TSGSessionInfo& sessionInfo)
{
	char			pcSDPString[4096];
	std::string GlobalSessionSection;

	if( sessionInfo.UseSessionInfo )
	{
		sprintf (pcSDPString, "i=%s\n",sessionInfo.sSessionInfo.c_str());
		GlobalSessionSection += pcSDPString;
		sprintf (pcSDPString, "u=%s\n",sessionInfo.sURL.c_str());
		GlobalSessionSection += pcSDPString;
		sprintf (pcSDPString, "e=%s\n",sessionInfo.sEmail.c_str());
		GlobalSessionSection += pcSDPString;
		sprintf (pcSDPString, "p=%s\n",sessionInfo.sPhoneNumber.c_str());
		GlobalSessionSection += pcSDPString;
	}
	return GlobalSessionSection;
}


// ------------------------------

void SDPString::GenerateSDPString()
{
	std::string			pSessionSection;
	std::string			pVideoSection;
	std::string			pAudioSection;
	std::string			pGlobalSessionSection;

	int					iAverageBitrate = 0;

	m_pSdpString = "";

	// Video Section String
	TSGVideoCodecData* pVideoCodecData = &m_StreamData.VideoCodecData;
	if (pVideoCodecData->eCodec != eSDPSGVC_None)
	{
		int iVideoAverageBitrate = pVideoCodecData->iVideoAverageBitrate;
		switch(pVideoCodecData->eCodec)
		{
			case eSDPSGVC_Mpeg4:
				pVideoSection = getMpeg4MediaSection(pVideoCodecData->uiVideoDestPort,
													pVideoCodecData->uiVideoPayloadType,
													pVideoCodecData->uiVideoRTPStreamTimeScale,
													pVideoCodecData->pcAsciiHeader.c_str(),
													iVideoAverageBitrate,
													pVideoCodecData->uiVideoHeight,
													pVideoCodecData->uiVideoWidth,
													pVideoCodecData->uiVideoProfileLevel);
				break;
			case eSDPSGVC_H263:
				pVideoSection = getH263MediaSection(pVideoCodecData->uiVideoDestPort,
													pVideoCodecData->uiVideoPayloadType,
													pVideoCodecData->uiVideoRTPStreamTimeScale,
													iVideoAverageBitrate);
				break;
			case eSDPSGVC_H264:
				pVideoSection = getH264MediaSection(pVideoCodecData->uiVideoDestPort,
													pVideoCodecData->uiVideoPayloadType,
													pVideoCodecData->uiVideoRTPStreamTimeScale,
													pVideoCodecData->pcAsciiHeader.c_str(),
													iVideoAverageBitrate,
													pVideoCodecData->uiVideoHeight,
													pVideoCodecData->uiVideoWidth);
				break;
		}

		iAverageBitrate += iVideoAverageBitrate;
	}

	// Audio Section String
	TSGAudioCodecData* pAudioCodecData = &m_StreamData.AudioCodecData;
	if (pAudioCodecData->eCodec != eSDPSGAC_None)
	{
		switch(pAudioCodecData->eCodec)
		{
			case eSDPSGAC_Aac:
				{
					switch(m_StreamData.eStreamType)
					{
						case eSDPSGST_Isma10:
						case eSDPSGST_Isma20:
								pAudioSection = getAacIsmaMediaSection(pAudioCodecData->uiAudioDestPort, pAudioCodecData->uiAudioPayloadType, pAudioCodecData->uiAudioRTPStreamTimeScale, pAudioCodecData->pcAsciiHeader.c_str(), &pAudioCodecData->iAudioAverageBitrate, pAudioCodecData->uiAudioChannels);
							break;
						case eSDPSGST_3gpp:
								pAudioSection = getAac3gppMediaSection(pAudioCodecData->uiAudioDestPort, pAudioCodecData->uiAudioPayloadType, pAudioCodecData->uiAudioRTPStreamTimeScale, pAudioCodecData->pcAsciiHeader.c_str(), &pAudioCodecData->iAudioAverageBitrate);
							break;
					}
				}
				break;
			case eSDPSGAC_Amr:
					pAudioSection = getAmrMediaSection(pAudioCodecData->uiAudioDestPort, pAudioCodecData->uiAudioPayloadType, pAudioCodecData->uiAudioRTPStreamTimeScale, pAudioCodecData->pcAsciiHeader.c_str(), &pAudioCodecData->iAudioAverageBitrate);
				break;
			case eSDPSGAC_AmrWB:
					pAudioSection = getAmrWBMediaSection(pAudioCodecData->uiAudioDestPort, pAudioCodecData->uiAudioPayloadType, pAudioCodecData->uiAudioRTPStreamTimeScale, pAudioCodecData->pcAsciiHeader.c_str(), &pAudioCodecData->iAudioAverageBitrate);
				break;

		}

		iAverageBitrate += pAudioCodecData->iAudioAverageBitrate;
	}

	// Session Section string
	switch(m_StreamData.eStreamType)
	{
		case eSDPSGST_Isma10:
			pSessionSection = getIsmaSessionSection(m_StreamData.pcSourceIPAdr.c_str(), m_StreamData.pcDestIP.c_str(), m_StreamData.pcAsciiHeader.c_str(), iAverageBitrate, 1, m_StreamData.sSessionName.c_str());
			break;
		case eSDPSGST_Isma20:
			pSessionSection = getIsmaSessionSection(m_StreamData.pcSourceIPAdr.c_str(), m_StreamData.pcDestIP.c_str(), m_StreamData.pcAsciiHeader.c_str(), iAverageBitrate, 2, m_StreamData.sSessionName.c_str());
			break;
		case eSDPSGST_3gpp:
			pSessionSection = get3gppSessionSection(m_StreamData.pcSourceIPAdr.c_str(), m_StreamData.pcDestIP.c_str(), iAverageBitrate, m_StreamData.sSessionName.c_str());
			break;
	}

	// Global Session Section
	pGlobalSessionSection = getGlobalSessionSection(m_StreamData.SessionInfo);

	m_pSdpString = pSessionSection;
	m_pSdpString += pGlobalSessionSection;
	m_pSdpString += pVideoSection;
	m_pSdpString += pAudioSection;
}


// ------------------------------
