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


#ifndef SDPSTRING_H_
#define SDPSTRING_H_

#include <string>

// Video Data
enum ESDPSGVideoCodec
{
    eSDPSGVC_None = 0,
    eSDPSGVC_Mpeg4,
    eSDPSGVC_H263,
    eSDPSGVC_H264,
};

// Audio Data
enum ESDPSGAudioCodec
{
    eSDPSGAC_None = 0,
    eSDPSGAC_Aac,
    eSDPSGAC_Amr,
    eSDPSGAC_AmrWB
};

struct TSGVideoCodecData
{
    // General
    ESDPSGVideoCodec	eCodec;
    unsigned int 		uiVideoDestPort;
    unsigned int 		uiVideoPayloadType;
    unsigned int 		uiVideoRTPStreamTimeScale;
    int 				iVideoAverageBitrate;
    std::string			pcAsciiHeader;

    // Mpeg4 y H264
    unsigned int 		uiVideoHeight;
    unsigned int	    uiVideoWidth;

    // Mpeg4
    unsigned int	    uiVideoProfileLevel;
};

struct TSGAudioCodecData
{
    // General
    ESDPSGAudioCodec		eCodec;
    unsigned int 			uiAudioDestPort;
    unsigned int 			uiAudioPayloadType;
    unsigned int 			uiAudioRTPStreamTimeScale;
    std::string 			pcAsciiHeader;
    int						iAudioAverageBitrate;

    // Aac
    unsigned int			uiAudioChannels;
};

// Stream Data
enum ESDPSGStreamType
{
    eSDPSGST_Isma10,
    eSDPSGST_Isma20,
    eSDPSGST_3gpp,
};

struct TSGSessionInfo
{
    bool			UseSessionInfo;
    std::string		sSessionInfo;
    std::string		sURL;
    std::string		sEmail;
    std::string 	sPhoneNumber;
};

struct TSGStreamData
{
    ESDPSGStreamType  	eStreamType;
    std::string			pcSourceIPAdr;
    std::string			pcDestIP;
    std::string   		sSessionName;
    std::string			pcAsciiHeader;
    TSGVideoCodecData	VideoCodecData;
    TSGAudioCodecData	AudioCodecData;
    TSGSessionInfo		SessionInfo;
};

//********************************************************************
//
//********************************************************************
class SDPString
{
public:
    SDPString (const TSGStreamData& StreamData) { m_StreamData = StreamData; GenerateSDPString(); }
    ~SDPString () {}

    const char*			GetString		() const { return m_pSdpString.c_str(); }
    bool				SaveToFile		(const char* pszFileName) const;

private:

    //bool 		writeSDPFile			(	const char* pszDstName );
    std::string	getMpeg4MediaSection	(	unsigned int	uiVideoDestPort,
                                            unsigned int	uiVideoPayloadType,
                                            unsigned int	uiVideoRTPStreamTimeScale,
                                            const char*		pcAsciiHeader,
                                            int				iVideoAverageBitrate,
                                            unsigned int	uiVideoHeight,
                                            unsigned int 	uiVideoWidth,
                                            unsigned int	uiVideoProfileLevel);
    std::string 	getH263MediaSection		(	unsigned int	uiVideoDestPort,
                                            unsigned int	uiVideoPayloadType,
                                            unsigned int	uiVideoRTPStreamTimeScale,
                                            int				iVideoAverageBitrate);
    std::string 	getH264MediaSection		(	unsigned int	uiVideoDestPort,
                                            unsigned int	uiVideoPayloadType,
                                            unsigned int	uiVideoRTPStreamTimeScale,
                                            const char*	 	pcAsciiHeader,
                                            int				iVideoAverageBitrate,
                                            int				uiVideoHeight,
                                            int				uiVideoWidth);
    std::string 	getAac3gppMediaSection	(	unsigned int	uiAudioDestPort,
                                            unsigned int	uiAudioPayloadType,
                                            unsigned int	uiAudioRTPStreamTimeScale,
                                            const char*	  	pcAsciiHeader,
                                            int*			piAudioAverageBitrate);
    std::string 	getAacIsmaMediaSection	(	unsigned int	uiAudioDestPort,
                                            unsigned int	uiAudioPayloadType,
                                            unsigned int	uiAudioRTPStreamTimeScale,
                                            const char*		pcAsciiHeader,
                                            int*			piAudioAverageBitrate,
                                            int				uiAudioChannels);
    std::string 	getAmrMediaSection		( 	unsigned int	uiAudioDestPort,
                                            unsigned int	uiAudioPayloadType,
                                            unsigned int	uiAudioRTPStreamTimeScale,
                                            const char*		pcAsciiHeader,
                                            int*			piAudioAverageBitrate);
    std::string 	getAmrWBMediaSection		( 	unsigned int	uiAudioDestPort,
                                            unsigned int	uiAudioPayloadType,
                                            unsigned int	uiAudioRTPStreamTimeScale,
                                            const char*		pcAsciiHeader,
                                            int*			piAudioAverageBitrate);
    std::string 	getIsmaSessionSection	(	const char* 	pcSourceIPAdr,
                                            const char* 	pcDestIP,
                                            const char* 	pcAsciiHeader,
                                            int				iAverageBitrate,
                                            int 			iIsmaVersion,
                                            const char* 	pszSessionName);
    std::string	get3gppSessionSection	(	const char*		pcSourceIPAdr,
                                            const char*		pcDestIPAdr,
                                            int				iAverageBitrate,
                                            const char* 	pszSessionName);
    std::string getGlobalSessionSection (TSGSessionInfo& sessionInfo);
    
    void 		GenerateSDPString		();


    std::string			m_pSdpString;
    TSGStreamData		m_StreamData;
};

#endif
