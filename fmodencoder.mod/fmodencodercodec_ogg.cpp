
#include "fmodencodercodec_ogg.h"

CFModEncoderCodecOgg::CFModEncoderCodecOgg(){
}

int CFModEncoderCodecOgg::Init(){
	return false;
}

void CFModEncoderCodecOgg::Close(){
}

int CFModEncoderCodecOgg::InitCodec(int iNoOfChannels,float dwInSamplerate, float dwOutBitrate, void* pInitData, unsigned long* puiInitDataSize){
	return false;
}

int CFModEncoderCodecOgg::Encode(void* pInSamples,unsigned long uiInSamplesLength, void* pEncodedSamples, unsigned long* uiEncodedSamplesLength){
	return false;
}

void CFModEncoderCodecOgg::EmptyCodec(void* pEncodedSamples, unsigned long* uiEncodedSamplesLength){
}
