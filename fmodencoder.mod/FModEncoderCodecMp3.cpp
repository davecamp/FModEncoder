
#include <windows.h>
#include "FModEncoderCodecMp3.h"

FModEncoderCodecMp3::FModEncoderCodecMp3():
m_hLameDll(0),
m_pLame(0),
m_fnLameInit(0),
m_fnLameClose(0),
m_fnLameSetNumberChannels(0),
m_fnLameSetInSamplerate(0),
m_fnLameSetBitrate(0),
m_fnLameSetMode(0),
m_fnLameInitParams(0),
m_fnLameEncodeBufferInterleaved(0),
m_fnLameFlushEncoder(0){
}

int FModEncoderCodecMp3::Init(){
	if(!InitLameDll()) return false;
	
	m_pLame = m_fnLameInit();
	if(!m_pLame){
		Close();
		return false;
	}
	
	return true;
}

void FModEncoderCodecMp3::Close(){
	FreeLibrary(m_hLameDll);
	m_hLameDll = 0;
}

int FModEncoderCodecMp3::InitCodec(int iNoOfChannels,float fInSamplerate, float fOutBitrate, void* pInitData, unsigned long* puiInitDataSize){
	MPEG_mode mode = STEREO;
	fInSamplerate *= (.5f * iNoOfChannels);
	if(iNoOfChannels == 1) mode = MONO;
	
	m_fnLameSetNumberChannels(m_pLame,iNoOfChannels);
	m_fnLameSetInSamplerate(m_pLame,static_cast<int>(fInSamplerate));
	m_fnLameSetBitrate(m_pLame,fOutBitrate);
	m_fnLameSetMode(m_pLame, mode);
	m_fnLameInitParams(m_pLame);

	*puiInitDataSize = 0;
	return true;
}

void FModEncoderCodecMp3::Encode(void* pInSamples,unsigned long uiInSamplesLength, void* pEncodedSamples, unsigned long* uiEncodedSamplesLength){
	short int* psSamples = static_cast<short int*>(pInSamples);
	BYTE* pbEncodedSamples = static_cast<BYTE*>(pEncodedSamples);

	*uiEncodedSamplesLength = m_fnLameEncodeBufferInterleaved(m_pLame, psSamples, uiInSamplesLength/4, pbEncodedSamples, 0);
};

void FModEncoderCodecMp3::EmptyCodec(void* pEncodedSamples, unsigned long* uiEncodedSamplesLength){
	BYTE* pbEncodedSamples = static_cast<BYTE*>(pEncodedSamples);

	*uiEncodedSamplesLength = m_fnLameFlushEncoder(m_pLame,pbEncodedSamples,0);
}

int FModEncoderCodecMp3::InitLameDll(){
	m_hLameDll = LoadLibrary("libmp3lame.dll");
	if(!m_hLameDll) return false;

	m_fnLameInit = (lame_global_flags*(__cdecl *)(void))GetProcAddress(m_hLameDll,"lame_init");
	m_fnLameClose = (int(__cdecl *)(lame_global_flags*))GetProcAddress(m_hLameDll,"lame_close");
	m_fnLameSetNumberChannels = (int(__cdecl *)(lame_global_flags*,int))GetProcAddress(m_hLameDll,"lame_set_num_channels");
	m_fnLameSetInSamplerate = (int(__cdecl *)(lame_global_flags*,int))GetProcAddress(m_hLameDll,"lame_set_in_samplerate");
	m_fnLameSetBitrate = (int(__cdecl *)(lame_global_flags*,int))GetProcAddress(m_hLameDll,"lame_set_brate");
	m_fnLameSetMode = (int(__cdecl *)(lame_global_flags*,MPEG_mode))GetProcAddress(m_hLameDll,"lame_set_mode");
	m_fnLameInitParams = (int(__cdecl *)(lame_global_flags*))GetProcAddress(m_hLameDll,"lame_init_params");
	m_fnLameEncodeBufferInterleaved = (int(__cdecl *)(lame_global_flags*,short int pcm[],int numSamples,unsigned char* pMp3Buffer,int Mp3BufferSize))GetProcAddress(m_hLameDll,"lame_encode_buffer_interleaved");
	m_fnLameFlushEncoder = (int(__cdecl *)(lame_global_flags*,unsigned char* pMp3Buffer,int Mp3BufferSize))GetProcAddress(m_hLameDll,"lame_encode_flush");

	if(	!m_fnLameInit ) return false;
	if( !m_fnLameClose ) return false;
	if( !m_fnLameSetNumberChannels) return false;
	if( !m_fnLameSetInSamplerate) return false;
	if( !m_fnLameSetBitrate ) return false;
	if( !m_fnLameSetMode ) return false;
	if( !m_fnLameInitParams ) return false;
	if( !m_fnLameEncodeBufferInterleaved ) return false;
	if( !m_fnLameFlushEncoder ) return false;
	return true;
}







