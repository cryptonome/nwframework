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
