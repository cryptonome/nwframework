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


#ifndef _NW_RTTI_H_
#define _NW_RTTI_H_

typedef intptr_t NWRttiTypeId;

//----------------------------------------------------------------------------
// The RTTI itself
//----------------------------------------------------------------------------
template <class T> class NWRtti
{
public:
    static inline NWRttiTypeId getTypeId();

private:
    static int mDummyVar;
};

template <class T> /*static*/ int NWRtti<T>::mDummyVar = 0;

template <class T> /*static*/ inline NWRttiTypeId NWRtti<T>::getTypeId()
{
    ++mDummyVar;
    return (NWRttiTypeId)&getTypeId;
}

#define RTTI_TYPE_ID(T) static inline NWRttiTypeId getTypeId() {return NWRtti<T>::getTypeId();} // use this macro inside classes yo want to have TypeId

//----------------------------------------------------------------------------
// Helper class to inherit from by classes exporting interfaces
//----------------------------------------------------------------------------
class NWInterfaceProvider
{
public:
    template <class T> inline T * getInterface();

protected:
    virtual void * getInterfaceFromID(NWRttiTypeId _typeId)=0;      // derived class must implement this function
};

template <class T> inline T * NWInterfaceProvider::getInterface()
{
    return (T *) getInterfaceFromID(NWRtti<T>::getTypeId());
}

#endif //_NW_RTTI_H_

//****************************************************************************
// Example
//****************************************************************************

//----------------------------------------------------------------------------
// Some interfaces we want share
//----------------------------------------------------------------------------
//struct IEncoder
//{
//    virtual unsigned int getEncodingCaps()=0;
//    ...
//
//    RTTI_TYPE_ID(IEncoder)
//};
//
//struct IVideoEncoder
//{
//    virtual unsigned int getVideoEncoderCaps()=0;
//    ...
//
//    RTTI_TYPE_ID(IVideoEncoder)
//}
//
//struct IVideoEncoderH264
//{
//    virtual int getVideoCodecH264KeyFramePeriod()=0;
//    ...
//
//    RTTI_TYPE_ID(IVideoEncoderH264)
//}

//----------------------------------------------------------------------------
// The exporter (or it's base class if any) inherits from NWInterfaceProvider
//----------------------------------------------------------------------------
//class VideoEncoderH264 : public NWInterfaceProvider
//{
//public:
//    ...
//
//protected:
//    virtual void * getInterfaceFromID(NWRttiTypeId _iid);     // <--- Implements this virtual fn to provide the valid interfaces
//
//private:
//    ...
//};

//----------------------------------------------------------------------------
// Exporter class fn (this class inherits from the interface):
//----------------------------------------------------------------------------
///*virtual*/ void * VideoEncoderH264::getInterfaceFromID(NWRttiTypeId _iid)
//{
//    if(_iid == IVideoEncoderH264::getTypeId())
//    {
//        return static_cast<IVideoEncoderH264*>(this);
//    }
//    else if(_iid == IVideoEncoder::getTypeId())
//    {
//        return static_cast<IVideoEncoder*>(this);
//    }
//    else if(_iid == IEncoder::getTypeId())
//    {
//        return static_cast<IEncoder*>(this);
//    }
//
//    return NULL; // if no base class just return NULL
//    //return Inherited::getInterfaceFromID(_iid); // in case that the base class has some other exported interfaces
//}

//----------------------------------------------------------------------------
// External use
//----------------------------------------------------------------------------
//void test()
//{
//    VideoEncoderH264 encoderObj;
//
//    encoderObj.init();
//
//    IEncoder * encoder = encoderObj.getInterface<IEncoder>();
//    if(encoder)
//    {
//        unsigned int caps = encoder->getEncodingCaps();
//    }
//
//    IVideoEncoder * videoEncoder = encoderObj.getInterface<IVideoEncoder>();
//    if(videoEncoder)
//    {
//        unsigned int caps = videoEncoder->getVideoEncoderCaps();
//    }
//
//    IVideoEncoderH264 * videoEncoderH264 = encoderObj.getInterface<IVideoEncoderH264>();
//    if(videoEncoderH264)
//    {
//        int h264KeyFramePeriod = videoEncoderH264->getVideoCodecH264KeyFramePeriod();
//    }
//
//    encoderObj.shutdown();
//}