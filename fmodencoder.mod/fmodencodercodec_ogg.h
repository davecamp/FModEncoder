
#ifndef SRS_FMODENCODERCODEC_OGG_H
#define SRS_FMODENCODERCODEC_OGG_H

#include "ifmodencodercodec.h"

class CFModEncoderCodecOgg : public IFModEncoderCodec{
public:
	CFModEncoderCodecOgg();
	int Init();
	void Close();

	int InitCodec(int iNoOfChannels,float dwInSamplerate, float dwOutBitrate, void* pInitData, unsigned long* puiInitDataSize);
	int Encode(void* pInSamples,unsigned long uiInSamplesLength, void* pEncodedSamples, unsigned long* uiEncodedSamplesLength);
	void EmptyCodec(void* pEncodedSamples, unsigned long* uiEncodedSamplesLength);
};

#endif

