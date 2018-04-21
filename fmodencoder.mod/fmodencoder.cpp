
#include "FModEncoder.h"
#include "FModEncoderCodecMp3.h"
#include "FModEncoderCodecOgg.h"
#include "FModEncoderCodecWav.h"

extern IFModEncoder* g_pEncoder;
extern int g_iLock;

extern"C"{
	IFModEncoder* CreateEncoder(){
		return new FModEncoder();
	}
	
	void DestroyEncoder(IFModEncoder* pEncoder){
		delete pEncoder;
	}
}

FModEncoder::FModEncoder() :
m_pCodec(0),
m_pOutputFile(0){
	memset(&m_EncodedBuffer,0,sizeof(m_EncodedBuffer));
}

int FModEncoder::InitEncoder(FMOD_ENCODER_TYPE eEncoderType,char* pOutputFilename,int iNoChannels,float fInputSamplerate,float fOutputBitrate){
	if(m_pCodec){
		delete(m_pCodec);
	}
	
	switch(eEncoderType){
	case FMOD_ENCODER_TYPE_MP3:
		m_pCodec = new FModEncoderCodecMp3();
		break;
		
	case FMOD_ENCODER_TYPE_OGG:
		m_pCodec = new FModEncoderCodecOgg();
		break;
		
	case FMOD_ENCODER_TYPE_WAV:
		m_pCodec = new FModEncoderCodecWav();
		break;
		
	default:
		return 0;
	}

	if (!m_pCodec->Init()){
		delete m_pCodec;
		m_pCodec = 0;
		return 0;
	}
	
	m_pOutputFile = fopen64(pOutputFilename,"wb");
	if(!m_pOutputFile){
		delete m_pCodec;
		m_pCodec = 0;
		return 0;
	}

	unsigned long uiEncodedBufferSize = 0;
	if(!m_pCodec->InitCodec(iNoChannels, fInputSamplerate, fOutputBitrate, &m_EncodedBuffer, &uiEncodedBufferSize)){
		delete m_pCodec;
		m_pCodec = 0;
		return false;
	}

	if(uiEncodedBufferSize) fwrite(m_EncodedBuffer,sizeof(char),uiEncodedBufferSize,m_pOutputFile);
	return true;
}

void FModEncoder::StartEncoding(){
	while (!__sync_bool_compare_and_swap(&g_iLock,0,1)){}
	g_pEncoder = this;
	g_iLock = 0;
}

void FModEncoder::StopEncoding(){
	while (!__sync_bool_compare_and_swap(&g_iLock,0,1)){}
	g_pEncoder = 0;
	g_iLock = 0;

	fclose(m_pOutputFile);
}

void FModEncoder::Encode(void* pSamples, unsigned int uiSamplesLength){
	if(m_pCodec){
		unsigned long uiEncodedLength = 0;
		m_pCodec->Encode(pSamples,uiSamplesLength,&m_EncodedBuffer,&uiEncodedLength);
		if(uiEncodedLength ) fwrite(m_EncodedBuffer,1,uiEncodedLength,m_pOutputFile);
	}

}


