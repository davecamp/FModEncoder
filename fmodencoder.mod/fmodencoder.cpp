
#include <stdio.h>

#include "..\..\bah.mod\fmod.mod\include\fmod_errors.h"
#include "fmodencoder.h"
#include "fmodencodercodec_mp3.h"
#include "fmodencodercodec_ogg.h"
#include "fmodencodercodec_wav.h"

#define CHECK_FMOD_ERROR( fmodResult ) if(fmodResult != FMOD_OK){ printf("%s",FMOD_ErrorString(fmodResult)); StopRadioStation(); return false; }
int	g_iLock = 0;


int CreateFModEncoder(IFModEncoder** ppFModEncoder){
	if(!ppFModEncoder) return false;
	
	CFModEncoder* pEncoder = new CFModEncoder();
	*ppFModEncoder = pEncoder;

	return true;
}

int DestroyFModEncoder(IFModEncoder* pIFModEncoder){
	if(pIFModEncoder == 0) return false;
	
	pIFModEncoder->Close();
	return true;
}

CFModEncoder::CFModEncoder():
m_fmodSystem(0),
m_fmodChannel(0),
m_fmodSound(0),
m_fInputSamplerate(0.0f),
m_fOutputBitrate(0.0f),
m_eEncoderCodec(SRS_FMOD_ENCODER_CODEC_NONE),
m_pEncoderCodec(0),
m_bIsEncoding(false){
	memset(m_OutputFilename,0,sizeof(m_OutputFilename));
}

int CFModEncoder::Init(){
	CHECK_FMOD_ERROR( FMOD_System_Create(&m_fmodSystem) )
	CHECK_FMOD_ERROR( FMOD_System_Init(m_fmodSystem,32,FMOD_INIT_NORMAL,0) )
	
	m_pEncodedBuffer = malloc(16384);
	return true;
}

void CFModEncoder::Close(){
	StopRadioStation();
	if(m_fmodSystem) FMOD_System_Release(m_fmodSystem);

	free(m_pEncodedBuffer);
}

int CFModEncoder::SetRadioStation(char* pRadioStation){
	StopRadioStation();
	if(pRadioStation == 0) return true;

	return StartRadioStation(pRadioStation);
}

void CFModEncoder::SetOutputFilename(char* pFilename){
	strncpy(m_OutputFilename,pFilename,sizeof(m_OutputFilename)-1);
}

void CFModEncoder::SetEncoder(SRS_FMOD_ENCODER_CODEC eEncoderCodec){
	m_eEncoderCodec = eEncoderCodec;	
}

void CFModEncoder::SetOutputBitrate(float fBitrate){
	m_fOutputBitrate = fBitrate;
}
	
int CFModEncoder::StartEncoding(){
	if(!m_fmodSystem) return false;
	if(!InitCodec()) return false;

	int iNoChannels = 0;
	FMOD_Sound_GetFormat(m_fmodSound,0,0,&iNoChannels,0);
	
	m_OutputFile = fopen64(m_OutputFilename,"wb");
	if(!m_OutputFile) return false;
	
	unsigned long uiEncodedBufferSize = 0;
	if(!m_pEncoderCodec->InitCodec(iNoChannels, m_fInputSamplerate, m_fOutputBitrate, m_pEncodedBuffer, &uiEncodedBufferSize)){
		delete m_pEncoderCodec;
		m_pEncoderCodec = 0;
		return false;
	}

	if(uiEncodedBufferSize) fwrite(m_pEncodedBuffer,sizeof(char),uiEncodedBufferSize,m_OutputFile);

	m_bIsEncoding = true;
	return true;
}

int CFModEncoder::StopEncoding(){
	while (!__sync_bool_compare_and_swap(&g_iLock,0,1)){}

	unsigned long uiEncodedLength = 0;
	m_bIsEncoding = false;
	
	do{
		m_pEncoderCodec->EmptyCodec(m_pEncodedBuffer,&uiEncodedLength);
		if(uiEncodedLength) fwrite(m_pEncodedBuffer,sizeof(char),uiEncodedLength,m_OutputFile);
	}while(uiEncodedLength>0);
	
	fclose(m_OutputFile);
	
	CloseCodec();
	
	g_iLock = 0;
	return true;
}
	
int CFModEncoder::StartRadioStation(char* pRadioStation){
	if(!m_fmodSystem) return false;
		
	FMOD_CREATESOUNDEXINFO soundexinfo;
	memset(&soundexinfo,0,sizeof(FMOD_CREATESOUNDEXINFO));
	soundexinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	soundexinfo.pcmreadcallback = fmodCallback;

	CHECK_FMOD_ERROR( FMOD_System_CreateSound(m_fmodSystem,pRadioStation,FMOD_HARDWARE|FMOD_CREATESTREAM,&soundexinfo,&m_fmodSound) )
	CHECK_FMOD_ERROR( FMOD_Sound_SetUserData(m_fmodSound,this) )
	CHECK_FMOD_ERROR( FMOD_System_PlaySound(m_fmodSystem, FMOD_CHANNEL_FREE, m_fmodSound, 0, &m_fmodChannel) )
	CHECK_FMOD_ERROR( FMOD_Channel_GetFrequency(m_fmodChannel,&m_fInputSamplerate) )
	
	return true;
}

void CFModEncoder::StopRadioStation(){
	if(m_fmodChannel){
		FMOD_Channel_Stop(m_fmodChannel);
		m_fmodChannel = 0;
	}
	
	if(m_fmodSound){
		FMOD_Sound_Release(m_fmodSound);
		m_fmodSound = 0;
	}
}

int CFModEncoder::InitCodec(){
	if(!m_fmodSystem) return false;
	if(!m_fmodChannel) return false;

	switch(m_eEncoderCodec){
	case SRS_FMOD_ENCODER_CODEC_MP3:
		m_pEncoderCodec = new CFModEncoderCodecMp3();
		break;
		
	case SRS_FMOD_ENCODER_CODEC_OGG:
		m_pEncoderCodec = new CFModEncoderCodecOgg();
		break;
		
	case SRS_FMOD_ENCODER_CODEC_WAV:
		m_pEncoderCodec = new CFModEncoderCodecWav();
		break;

	default:
		return false;
		
	}
	
	if(!m_pEncoderCodec->Init()){
		CloseCodec();
		return false;
	}

	return true;
}

void CFModEncoder::CloseCodec(){
	m_pEncoderCodec->Close();
	delete m_pEncoderCodec;
	m_pEncoderCodec = 0;
}

void CFModEncoder::Encode(void* pSamples, unsigned int uiSamplesLength){
	if(m_pEncoderCodec){
		unsigned long uiEncodedLength = 0;
		m_pEncoderCodec->Encode(pSamples,uiSamplesLength,m_pEncodedBuffer,&uiEncodedLength);
		if(uiEncodedLength ) fwrite(m_pEncodedBuffer,1,uiEncodedLength,m_OutputFile);
	}
}

inline int CFModEncoder::IsEncoding(){
	return m_bIsEncoding;
}

FMOD_RESULT F_CALLBACK fmodCallback(FMOD_SOUND* pSound,void* data, unsigned int datalen){
	while (!__sync_bool_compare_and_swap(&g_iLock,0,1)){}
	
	void* pUserData = 0;
	IFModEncoder* pEncoder = 0;
	
	FMOD_Sound_GetUserData(pSound, &pUserData);
	pEncoder = static_cast<IFModEncoder*>(pUserData);

	if(pEncoder && pEncoder->IsEncoding()){
		pEncoder->Encode(data, datalen);
	}
	
	g_iLock = 0;
	return FMOD_OK;
}











