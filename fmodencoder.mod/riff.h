
#ifndef SRS_RIFF_H
#define SRS_RIFF_H

#define FCC(ch4)	((((unsigned long)(ch4) & 0xFF) << 24) | \
					(((unsigned long)(ch4) & 0xFF00) << 8) | \
					(((unsigned long)(ch4) & 0xFF0000) >> 8) | \
					(((unsigned long)(ch4) & 0xFF000000) >> 24))

typedef struct _RIFFCHUNK{
	unsigned long fcc;
	unsigned long Size;
} RIFFCHUNK;

typedef struct _RIFFLIST{
	unsigned long fcc;
	unsigned long Size;
	unsigned long fccListType;
} RIFFLIST;

typedef struct _WAVCHUNK{
	unsigned short AudioFormat;
	unsigned short NoOfChannels;
	unsigned long Samplerate;
	unsigned long Byterate;
	unsigned short BlockAlign;
	unsigned short BitsPerSample;
} WAVCHUNK;

typedef struct _WAVHEADER{
	RIFFLIST 	ChunkId;
	RIFFCHUNK	SubChunk;
	WAVCHUNK	WavChunk;
	RIFFCHUNK	DataChunk;
} WAVHEADER;

#endif