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
#include "PchNWStream.h"

#include "NWStreamMedia.h"
#include "NWStreamBlockMedia.h"
#include "NWStreamGroup.h"

// Tiempo que se utiliza para mostrar un assert para detectar
// la desincronizacion entre streams de tipo media que existen
// en el mismo StreamGroup. 
// El tiempo representa la diferencia de tiempos maxima, en milisegundos
const int MAX_TIME_DIFFERENCE_BETWEEN_STREAMS_MS = 3000; // ms

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamMedia::NWStreamMedia() : Inherited(),
    mLastReadTime(-1)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamMedia::init(ENWStreamSubType _subType, ENWStreamMediaType _mediaType)
{
    bool bOK = true;

    if (!isOk())
    {
        bOK = Inherited::init(NWSTREAM_TYPE_MEDIA, _subType);

        mMediaType = _mediaType;
        mLastReadTime = -1;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamMedia::done()
{
    if (isOk())
    {

        Inherited::done();
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
/*INWStreamBlock* NWStreamMedia::readBlock(bool _checkThread)
{
    INWStreamBlock* streamBlock = Inherited::readBlock(_checkThread);

#ifdef _DEBUG
    if ( streamBlock )
        checkMediaBlockSync(streamBlock);
#endif

    return streamBlock;
}*/

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
/*void NWStreamMedia::checkMediaBlockSync(INWStreamBlock* _streamBlock)
{
    NWStreamBlockMedia* mediaBlock = (NWStreamBlockMedia*)_streamBlock;

    if ( !mediaBlock->IsEnd() )
    {
        // Check the difference between times
        u64 timeBlock = mediaBlock->getTime();
        u64 lastTimeRead = getLastReadTimeFromMediaStreams();
        if ( lastTimeRead != -1 )
        {
            u64 difference = 0;
            if ( timeBlock > lastTimeRead )
                difference = timeBlock - lastTimeRead;
            else
                difference = lastTimeRead - timeBlock;

            ASSERT(difference < (MAX_TIME_DIFFERENCE_BETWEEN_STREAMS_MS*10000));
            //LOG("difference (%d)",difference);
        }
        else
        {
            ASSERT(mLastReadTime == -1);
        }

        if ( timeBlock >= mLastReadTime || mLastReadTime == -1 )
            mLastReadTime = timeBlock;
    }
}*/

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
/*u64 NWStreamMedia::getLastReadTimeFromMediaStreams()
{
    u64 lastReadTime = mLastReadTime;

    int streams = mStreamGroupRead->getNumStreams();

    for ( int i = 0 ; i < streams ; ++i )
    {
        INWStream* stream = mStreamGroupRead->getStream(i);
        if ( stream->getType() == NWSTREAM_TYPE_MEDIA )
        {
            NWStreamMedia* streamMedia = (NWStreamMedia*)stream;
            
            u64 streamLastReadTime = streamMedia->mLastReadTime;
            if ( streamLastReadTime > lastReadTime && streamLastReadTime != -1  )
                lastReadTime = streamMedia->mLastReadTime;
        }
    }

    return lastReadTime;
}*/


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
ENWStreamMediaType NWStreamMedia::getMediaType()
{
    return mMediaType;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamMedia::setMediaType(ENWStreamMediaType _mediaType)
{
    mMediaType = _mediaType;
}

