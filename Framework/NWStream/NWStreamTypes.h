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
#ifndef NWSTREAMTYPES_H_
#define NWSTREAMTYPES_H_

//********************************************************************
//
//********************************************************************
enum ENWStreamType
{
    NWSTREAM_TYPE_MEDIA = 0,
};

enum ENWStreamSubType
{
    // Media
    NWSTREAM_SUBTYPE_MEDIA_VIDEO = 0,
    NWSTREAM_SUBTYPE_MEDIA_AUDIO,
    NWSTREAM_SUBTYPE_MEDIA_DS_MEDIASAMPLE,
};

enum ENWStreamMediaType
{
    NWSTREAM_MEDIATYPE_VIDEO = 0,
    NWSTREAM_MEDIATYPE_AUDIO,
    NWSTREAM_MEDIATYPE_UNKNOWN,
};

#endif
