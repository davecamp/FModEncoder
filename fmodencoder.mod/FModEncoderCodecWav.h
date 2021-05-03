#ifndef SRS_FMODENCODERCODEC_WAV_H
#define SRS_FMODENCODERCODEC_WAV_H

#include "riff.h"
#include "ifmodencodercodec.h"

class FModEncoderCodecWav : public IFModEncoderCodec{
public:
	FModEncoderCodecWav();
	int Init();
	void Close();

	int InitCodec(int iNoOfChannels,float dwInSamplerate, float dwOutBitrate, void* pInitData, unsigned long* puiInitDataSize);
	void Encode(void* pInSamples,unsigned long uiInSamplesLength, void* pEncodedSamples, unsigned long* puiEncodedSamplesLength);
	void EmptyCodec(void* pEncodedSamples, unsigned long* uiEncodedSamplesLength);
	
private:
	WAVHEADER		m_wavHeader;
};

#endif
