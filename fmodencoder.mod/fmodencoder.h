
#ifndef SRS_FMODENCODER_H
#define SRS_FMODENCODER_H

#include "..\..\bah.mod\fmod.mod\include\fmod.h"

#include "SRSFModTypes.h"
#include "ifmodencoder.h"
#include "ifmodencodercodec.h"

class CFModEncoder : public IFModEncoder{
public:
	CFModEncoder();
	
	int __cdecl Init();
	void _cdecl Close();
	
	int _cdecl SetRadioStation(char* pRadioStation);
	void _cdecl SetOutputFilename(char* pFilename);
	void _cdecl SetEncoder(SRS_FMOD_ENCODER_CODEC eEncoderType);
	void _cdecl SetOutputBitrate(float fBitrate);
	int _cdecl StartEncoding();
	int _cdecl StopEncoding();

	// called via callback
	int _cdecl IsEncoding();
	void _cdecl Encode(void* pSamples,unsigned int uiSamplesLength);

private:
	int StartRadioStation(char* pRadioStation);
	void StopRadioStation();
	
	int InitCodec();
	void CloseCodec();

	FMOD_SYSTEM* 			m_fmodSystem;
	FMOD_CHANNEL*			m_fmodChannel;
	FMOD_SOUND*				m_fmodSound;
	float					m_fInputSamplerate;
	float 					m_fOutputBitrate;
	
	char					m_OutputFilename[260];
	SRS_FMOD_ENCODER_CODEC	m_eEncoderCodec;
	IFModEncoderCodec*		m_pEncoderCodec;
	int						m_bIsEncoding;
	
	void*					m_pEncodedBuffer;
	FILE*					m_OutputFile;
};

FMOD_RESULT F_CALLBACK fmodCallback(FMOD_SOUND* sound, void* data, unsigned int datalen);

extern"C"{
	int CreateFModEncoder(IFModEncoder** ppIFModEncoder);
	int DestroyFModEncoder(IFModEncoder* pIFModEncoder);
}

#endif // SRS_FMODENCODER_H