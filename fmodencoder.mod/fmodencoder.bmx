
SuperStrict

Module srs.fmodencoder

Import bah.fmod
Import "fmodencoder.cpp"
Import "fmodencodercodec.cpp"
Import "fmodencodercodec_mp3.cpp"
Import "fmodencodercodec_ogg.cpp"

Const SRS_FMOD_ENCODER_TYPE_NONE:Int = 0
Const SRS_FMOD_ENCODER_TYPE_MP3:Int = 1
Const SRS_FMOD_ENCODER_TYPE_OGG:Int = 2

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
		Method SetOutputFilename:Int(pFilename$z)
		Method SetEncoder:Int(eEncoderType:Int)
		Method SetOutputBitrate:Int(fBitrate:Float)
		Method StartEncoding:Int()
		Method StopEncoding:Int()
	EndType
	
	Function CreateFModEncoder:Int(ppIFModEncoder:IFModEncoder Var)
	Function DestryFModEncoder(pIFModEncoder:IFModEncoder)
EndExtern