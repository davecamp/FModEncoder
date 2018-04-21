#ifndef SRS_IFMODENCODER_H
#define SRS_IFMODENCODER_H

#include "SRSFModEncoderTypes.h"

class IFModEncoder{
public:
	virtual int __cdecl InitEncoder(FMOD_ENCODER_TYPE eEncoderType,char* pOutputFilename,int iNoChannels,float fInputSamplerate,float fOutputBitrate) = 0;
	virtual void __cdecl StartEncoding() = 0;
	virtual void __cdecl StopEncoding() = 0;
	virtual void __cdecl Encode(void* pSamples,unsigned int uiSamplesLength) = 0;
};

#endif