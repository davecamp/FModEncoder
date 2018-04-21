
#ifndef SRS_FMODENCODER_H
#define SRS_FMODENCODER_H

#include "IFModEncoder.h"
#include "IFModEncoderCodec.h"
#include <cstdio>

class FModEncoder : public IFModEncoder{
public:
	FModEncoder();

	int __cdecl InitEncoder(FMOD_ENCODER_TYPE eEncoderType,char* pOutputFilename,int iNoChannels,float fInputSamplerate,float fOutputBitrate);
	void __cdecl StartEncoding();
	void __cdecl StopEncoding();
	void __cdecl Encode(void* pSamples,unsigned int uiSamplesLength);
	
private:
	IFModEncoderCodec* m_pCodec;
	char 	m_EncodedBuffer[16384];
	FILE*	m_pOutputFile;
};

#endif

