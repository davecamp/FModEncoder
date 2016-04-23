
#ifndef SRS_FMODENCODERCODEC_MP3_H
#define SRS_FMODENCODERCODEC_MP3_H

#include "ifmodencodercodec.h"

#include <windows.h>
#include "lame_enc.h"

class CFModEncoderCodecMp3 : public IFModEncoderCodec{
public:
	CFModEncoderCodecMp3();
	
	int Init();
	void Close();

	int InitCodec(int iNoOfChannels,float dwInSamplerate, float dwOutBitrate, void* pInitData, unsigned long* puiInitDataSize);
	int Encode(void* pInSamples,unsigned long uiInSamplesLength, void* pEncodedSamples, unsigned long* uiEncodedSamplesLength);
	void EmptyCodec(void* pEncodedSamples, unsigned long* uiEncodedSamplesLength);
	
private:
	int InitLameDll();
	void DeinitLameDll();
	
	HMODULE m_hLameDll;
	
	lame_global_flags*	m_pLame;
	lame_global_flags*	(__cdecl *m_fnLameInit)(void);
	int					(__cdecl *m_fnLameClose)(lame_global_flags*);
	int					(__cdecl *m_fnLameSetNumberChannels)(lame_global_flags*,int);
	int					(__cdecl *m_fnLameSetInSamplerate)(lame_global_flags*,int);
	int					(__cdecl *m_fnLameSetBitrate)(lame_global_flags*,int);
	int					(__cdecl *m_fnLameSetMode)(lame_global_flags*,MPEG_mode);
	int					(__cdecl *m_fnLameInitParams)(lame_global_flags*);
	int					(__cdecl *m_fnLameEncodeBufferInterleaved)(lame_global_flags*,short int pcm[],int numSamples,unsigned char* pMp3Buffer,int Mp3BufferSize);
	int					(__cdecl *m_fnLameFlushEncoder)(lame_global_flags*,unsigned char* pMp3Buffer,int Mp3BufferSize);
};

#endif

