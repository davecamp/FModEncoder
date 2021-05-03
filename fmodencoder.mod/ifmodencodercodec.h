
#ifndef SRS_IFMODENCODERCODEC_H
#define SRS_IFMODENCODERCODEC_H

class IFModEncoderCodec{
public:
	virtual int Init() = 0;
	virtual void Close() = 0;

	virtual int InitCodec(int iNoOfChannels,float dwInSamplerate, float dwOutBitrate, void* pInitData, unsigned long* puiInitDataSize) = 0;
	virtual void Encode(void* pInSamples,unsigned long uiInSamplesLength, void* pEncodedSamples, unsigned long* uiEncodedSamplesLength) = 0;
	virtual void EmptyCodec(void* pEncodedSamples, unsigned long* uiEncodedSamplesLength) = 0;
};

#endif
