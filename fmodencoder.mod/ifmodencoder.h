
#ifndef SRS_IFMODENCODER_H
#define SRS_IFMODENCODER_H

#include "..\..\bah.mod\fmod.mod\include\fmod.h"
#include "SRSFModTypes.h"

class IFModEncoder{
public:
	virtual int __cdecl Init() = 0;
	virtual void __cdecl Close() = 0;
	
	virtual int __cdecl SetRadioStation(char* pRadioStation) = 0;
	virtual void __cdecl SetOutputFilename(char* pFilename) = 0;
	virtual void __cdecl SetEncoder(SRS_FMOD_ENCODER_CODEC eEncoderType) = 0;
	virtual void __cdecl SetOutputBitrate(float fBitrate) = 0;
	virtual int __cdecl StartEncoding() = 0;
	virtual int __cdecl StopEncoding() = 0;
	virtual int __cdecl IsEncoding() = 0;
	virtual void __cdecl Encode(void* pSamples,unsigned int uiSamplesLength) = 0;
};

#endif