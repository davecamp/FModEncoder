
#ifndef SRS_IFMODENCODER_H
#define SRS_IFMODENCODER_H

#include "..\..\bah.mod\fmod.mod\include\fmod.h"
#include "SRSFModTypes.h"

class IFModEncoder{
public:
	virtual int Init() = 0;
	virtual void Close() = 0;
	
	virtual int SetRadioStation(char* pRadioStation) = 0;
	virtual int SetOutputFilename(char* pFilename) = 0;
	virtual int SetEncoder(SRS_FMOD_ENCODER_CODEC eEncoderType) = 0;
	virtual int SetOutputBitrate(float fBitrate) = 0;
	virtual int StartEncoding() = 0;
	virtual int StopEncoding() = 0;
	virtual int IsEncoding() = 0;
	virtual void Encode(void* pSamples,unsigned int uiSamplesLength) = 0;
};

#endif