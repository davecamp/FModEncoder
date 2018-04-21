#include <cstring>
#include "FModEncoderCodecWav.h"

FModEncoderCodecWav::FModEncoderCodecWav(){
	memset(&m_wavHeader,0,sizeof(WAVHEADER));
}

int FModEncoderCodecWav::Init(){
	return true;
}

void FModEncoderCodecWav::Close(){
}

int FModEncoderCodecWav::InitCodec(int iNoOfChannels,float dwInSamplerate, float dwOutBitrate, void* pInitData, unsigned long* puiInitDataSize){
	// assume 16 bit?
	m_wavHeader.ChunkId.fcc = FCC('RIFF');
	m_wavHeader.ChunkId.fccListType = FCC('WAVE');
	
	m_wavHeader.SubChunk.fcc = FCC('fmt ');
	m_wavHeader.SubChunk.Size = 16;
	
	m_wavHeader.WavChunk.AudioFormat = 1;
	m_wavHeader.WavChunk.NoOfChannels = iNoOfChannels;
	m_wavHeader.WavChunk.Samplerate = (unsigned long)dwInSamplerate;
	m_wavHeader.WavChunk.Byterate = (unsigned long)dwInSamplerate * iNoOfChannels * 16 / 8;
	m_wavHeader.WavChunk.BlockAlign = iNoOfChannels * 16 / 8;
	m_wavHeader.WavChunk.BitsPerSample = 16;

	m_wavHeader.DataChunk.fcc = FCC('data');

	*puiInitDataSize = sizeof(WAVHEADER);
	memcpy(pInitData,&m_wavHeader,sizeof(WAVHEADER));

	return true;
}

int FModEncoderCodecWav::Encode(void* pInSamples,unsigned long uiInSamplesLength, void* pEncodedSamples, unsigned long* puiEncodedSamplesLength){
	memcpy(pEncodedSamples,pInSamples,uiInSamplesLength);
	*puiEncodedSamplesLength = uiInSamplesLength;

	return true;
}

void FModEncoderCodecWav::EmptyCodec(void* pEncodedSamples, unsigned long* uiEncodedSamplesLength){
	// nothing to do here
}


