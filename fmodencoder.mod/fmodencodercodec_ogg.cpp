
#include <cstring>
#include "fmodencodercodec_ogg.h"

CFModEncoderCodecOgg::CFModEncoderCodecOgg(){
}

int CFModEncoderCodecOgg::Init(){
	return true;
}

void CFModEncoderCodecOgg::Close(){
	ogg_stream_clear(&m_stream);
	vorbis_comment_clear(&m_comment);
	vorbis_dsp_clear(&m_state);
	vorbis_block_clear(&m_block);
	vorbis_info_clear(&m_info);
}

int CFModEncoderCodecOgg::InitCodec(int iNoOfChannels,float dwInSamplerate, float dwOutBitrate, void* pInitData, unsigned long* puiInitDataSize){
	vorbis_info_init(&m_info);
	if(vorbis_encode_init_vbr(&m_info,iNoOfChannels,dwInSamplerate,dwOutBitrate) != 0) return false;

	vorbis_comment_init(&m_comment);

	if(vorbis_analysis_init(&m_state,&m_info) != 0) return false;
	if(vorbis_block_init(&m_state,&m_block) != 0)	return false;
	
	ogg_stream_init(&m_stream,0);
	
	ogg_packet header;
	ogg_packet header_comm;
	ogg_packet header_code;
	
	vorbis_analysis_headerout(&m_state,&m_comment,&header,&header_comm,&header_code);
	ogg_stream_packetin(&m_stream,&header); //automatically placed in its own page 
	ogg_stream_packetin(&m_stream,&header_comm);
	ogg_stream_packetin(&m_stream,&header_code);

	int iOffset = 0;
	while(ogg_stream_flush(&m_stream,&m_page)){
		memcpy(pInitData + iOffset,m_page.header,m_page.header_len);
		iOffset += m_page.header_len;

		memcpy(pInitData + iOffset,m_page.body,m_page.body_len);
		iOffset += m_page.body_len;
	}

	*puiInitDataSize = iOffset;
	return true;
}

int CFModEncoderCodecOgg::Encode(void* pInSamples,unsigned long uiInSamplesLength, void* pEncodedSamples, unsigned long* uiEncodedSamplesLength){
	float **buffer=vorbis_analysis_buffer(&m_state,uiInSamplesLength);
	short* psInSamples = static_cast<short*>(pInSamples);

	for( int i = 0; i < uiInSamplesLength; i++ ){
		buffer[0][i] = *psInSamples++ / 32768.f;
		buffer[1][i] = *psInSamples++ / 32768.f;
	}
	vorbis_analysis_wrote(&m_state,uiInSamplesLength/4);

	*uiEncodedSamplesLength = EncodeChunk(pEncodedSamples);
	return true;
}

unsigned long CFModEncoderCodecOgg::EncodeChunk(void* pEncodedSamples){
	int iEncodedBytes = 0;
	while(vorbis_analysis_blockout(&m_state,&m_block)==1){

		vorbis_analysis(&m_block,NULL);
		vorbis_bitrate_addblock(&m_block);
			
		while(vorbis_bitrate_flushpacket(&m_state,&m_packet)){
			ogg_stream_packetin(&m_stream,&m_packet);
			
			if(!ogg_stream_pageout(&m_stream,&m_page)) break;
			
			memcpy( pEncodedSamples + iEncodedBytes, m_page.header, m_page.header_len);
			iEncodedBytes += m_page.header_len;
			
			memcpy( pEncodedSamples + iEncodedBytes, m_page.body, m_page.body_len);
			iEncodedBytes += m_page.body_len;
		}
	}
	return iEncodedBytes;
}

void CFModEncoderCodecOgg::EmptyCodec(void* pEncodedSamples, unsigned long* uiEncodedSamplesLength){
	vorbis_analysis_wrote(&m_state,0);
	
	int iOutCount = 0;
	while(!ogg_page_eos(&m_page)){
		iOutCount += EncodeChunk(pEncodedSamples);
	}
	*uiEncodedSamplesLength = iOutCount;
}
