
#ifndef SRS_FMODENCODERCODEC_OGG_H
#define SRS_FMODENCODERCODEC_OGG_H

#include "ifmodencodercodec.h"
#include "libvorbis-1.3.7/include/vorbis/vorbisenc.h"

class FModEncoderCodecOgg : public IFModEncoderCodec{
public:
	FModEncoderCodecOgg();
	int Init();
	void Close();

	int InitCodec(int iNoOfChannels,float dwInSamplerate, float dwOutBitrate, void* pInitData, unsigned long* puiInitDataSize);
	void Encode(void* pInSamples,unsigned long uiInSamplesLength, void* pEncodedSamples, unsigned long* puiEncodedSamplesLength);
	void EmptyCodec(void* pEncodedSamples, unsigned long* uiEncodedSamplesLength);
	
private:
	unsigned long EncodeChunk(void* pInSamples);

	vorbis_info			m_info;
	vorbis_dsp_state	m_state;
	vorbis_block		m_block;
	vorbis_comment		m_comment;
	ogg_stream_state	m_stream;
	ogg_page			m_page;
	ogg_packet			m_packet;
	
	int					m_channels;
};

#endif

