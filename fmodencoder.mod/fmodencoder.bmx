
SuperStrict

Module srs.fmodencoder

Import bah.fmod

Import "libogg-1.3.2/include/*.h"
Import "libogg-1.3.2/src/bitwise.c"
Import "libogg-1.3.2/src/framing.c"
Import "libvorbis-1.3.5/include/vorbis/*.h"
Import "libvorbis-1.3.5/include/*.h"
Import "libvorbis-1.3.5/lib/*.h"
Import "libvorbis-1.3.5/lib/bitrate.c"
Import "libvorbis-1.3.5/lib/block.c"
Import "libvorbis-1.3.5/lib/codebook.c"
Import "libvorbis-1.3.5/lib/envelope.c"
Import "libvorbis-1.3.5/lib/floor0.c" 
Import "libvorbis-1.3.5/lib/floor1.c"
Import "libvorbis-1.3.5/lib/info.c"
Import "libvorbis-1.3.5/lib/lpc.c"
Import "libvorbis-1.3.5/lib/lsp.c"
Import "libvorbis-1.3.5/lib/mapping0.c"
Import "libvorbis-1.3.5/lib/mdct.c"
Import "libvorbis-1.3.5/lib/psy.c"
Import "libvorbis-1.3.5/lib/registry.c"
Import "libvorbis-1.3.5/lib/res0.c"
Import "libvorbis-1.3.5/lib/sharedbook.c"
Import "libvorbis-1.3.5/lib/smallft.c"
Import "libvorbis-1.3.5/lib/synthesis.c"
Import "libvorbis-1.3.5/lib/vorbisfile.c"
Import "libvorbis-1.3.5/lib/window.c"
Import "libvorbis-1.3.5/lib/analysis.c"
Import "libvorbis-1.3.5/lib/vorbisenc.c"

Import "fmodencoder.cpp"
Import "fmodencodercodec_wav.cpp"
Import "fmodencodercodec_mp3.cpp"
Import "fmodencodercodec_ogg.cpp"

Const SRS_FMOD_ENCODER_TYPE_NONE:Int = 0
Const SRS_FMOD_ENCODER_TYPE_MP3:Int = 1
Const SRS_FMOD_ENCODER_TYPE_OGG:Int = 2
Const SRS_FMOD_ENCODER_TYPE_WAV:Int = 3

Extern"C"
	Rem
	?bmxng
	Interface IFModEncoder
		Method Init:Int()
		Method Close()
		
		Method SetRadioStation:Int(RadioStation$z)
		Method SetOutputFilename:Int(pFilename$z)
		Method SetEncoder:Int(eEncoderType:Int)
		Method SetOutputBitrate:Int(fBitrate:Float)
		Method StartEncoding:Int()
		Method StopEncoding:Int()
	EndInterface
	?
	EndRem
	?Not bmxng
	Type IFModEncoder
		Method Init:Int()
		Method Close()
		
		Method SetRadioStation:Int(RadioStation$z)
		Method SetOutputFilename(pFilename$z)
		Method SetEncoder(eEncoderType:Int)
		Method SetOutputBitrate(fBitrate:Float)
		Method StartEncoding:Int()
		Method StopEncoding:Int()
	EndType
	?

	Function CreateFModEncoder:Int(ppIFModEncoder:IFModEncoder Var)
	Function DestryFModEncoder(pIFModEncoder:IFModEncoder)
EndExtern