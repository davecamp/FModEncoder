
#include "fmodencodercodec_wav.h"

CFModEncoderCodecWav::CFModEncoderCodecWav(){
}

int CFModEncoderCodecWav::Init(){
	return false;
}

void CFModEncoderCodecWav::Close(){
}

int CFModEncoderCodecWav::InitCodec(int iNoOfChannels,float dwInSamplerate, float dwOutBitrate, void* pInitData, unsigned long* puiInitDataSize){
	return false;
}

int CFModEncoderCodecWav::Encode(void* pInSamples,unsigned long uiInSamplesLength, void* pEncodedSamples, unsigned long* puiEncodedSamplesLength){
	return false;
}

void CFModEncoderCodecWav::EmptyCodec(void* pEncodedSamples, unsigned long* uiEncodedSamplesLength){
}

unsigned long CFModEncoderCodecWav::EncodeChunk(void* pInSamples){
	return false;
}

