SuperStrict

Import bah.fmod
Import pub.win32

Module srs.fmodencoder

' For the sake of the example app I kept the volume on - Col"

ModuleInfo "2020.09.17:"
ModuleInfo " - Module modifications and updates - Grisu"

ModuleInfo "2018.04.21 :"
ModuleInfo " - Corrected a bug of not closing the encoded when finished - Col"
ModuleInfo " - Allow encoding of mono input files - Col"
ModuleInfo " - Module modifications and updates - Grisu"

ModuleInfo "LD_OPTS: -march=i686"

Import "libogg-1.3.4/include/*.h"
Import "libogg-1.3.4/src/bitwise.c"
Import "libogg-1.3.4/src/framing.c"
Import "libvorbis-1.3.7/include/vorbis/*.h"
Import "libvorbis-1.3.7/include/*.h"
Import "libvorbis-1.3.7/lib/*.h"
Import "libvorbis-1.3.7/lib/bitrate.c"
Import "libvorbis-1.3.7/lib/block.c"
Import "libvorbis-1.3.7/lib/codebook.c"
Import "libvorbis-1.3.7/lib/envelope.c"
Import "libvorbis-1.3.7/lib/floor0.c" 
Import "libvorbis-1.3.7/lib/floor1.c"
Import "libvorbis-1.3.7/lib/info.c"
Import "libvorbis-1.3.7/lib/lpc.c"
Import "libvorbis-1.3.7/lib/lsp.c"
Import "libvorbis-1.3.7/lib/mapping0.c"
Import "libvorbis-1.3.7/lib/mdct.c"
Import "libvorbis-1.3.7/lib/psy.c"
Import "libvorbis-1.3.7/lib/registry.c"
Import "libvorbis-1.3.7/lib/res0.c"
Import "libvorbis-1.3.7/lib/sharedbook.c"
Import "libvorbis-1.3.7/lib/smallft.c"
Import "libvorbis-1.3.7/lib/synthesis.c"
Import "libvorbis-1.3.7/lib/vorbisfile.c"
Import "libvorbis-1.3.7/lib/window.c"
Import "libvorbis-1.3.7/lib/analysis.c"
Import "libvorbis-1.3.7/lib/vorbisenc.c"

Import "FModCallback.cpp"
Import "FModEncoder.cpp"
Import "FModEncoderCodecOgg.cpp"
Import "FModEncoderCodecMp3.cpp"
Import "FModEncoderCodecWav.cpp"

Type TFMODCreateSoundExInfo2
	Field cbsize:Int = SizeOf(TFMODCreateSoundExInfo2)
	Field length:Int
	Field fileoffset:Int
	Field numchannels:Int
	Field defaultfrequency:Int
	Field format:Int
	Field decodebuffersize:Int
	Field initialsubsound:Int
	Field numsubsounds:Int
	Field inclusionlist:Int Ptr
	Field inclusionlistnum:Int
	Field pcmreadcallback:Byte Ptr
	Field pcmsetposcallback:Byte Ptr
	Field nonblockcallback:Byte Ptr
	Field dlsname:Byte Ptr
	Field encryptionkey:Byte Ptr
	Field maxpolyphony:Int
	Field userdata:Byte Ptr
	Field suggestedsoundtype:Int
	Field useropen:Byte Ptr
	Field userclose:Byte Ptr
	Field userread:Byte Ptr
	Field userseek:Byte Ptr
	Field userasyncread:Byte Ptr
	Field userasynccancel:Byte Ptr
	Field speakermap:Int
	Field initialsoundgroup:Int Ptr
	Field initialseekposition:Int
	Field initialseekpostype:Int
	Field ignoresetfilesystem:Int
	Field cddaforceaspi:Int
	Field audioqueuepolicy:Int
	Field minmidigranularity:Int
	Field nonblockthreadid:Int
EndType

Const FMOD_ENCODER_MP3:Int = 1
Const FMOD_ENCODER_OGG:Int = 2
Const FMOD_ENCODER_WAV:Int = 3

Extern "C"
	Type IFModEncoder
		Method InitEncoder:Int(EncoderType:Int,OutputFilename$z,NoOfChannels:Int,InputSamplerate:Float,OutputBitrate:Float)
		Method StartEncoding()
		Method StopEncoding()
	EndType
	
	Function CreateEncoder:IFModEncoder()
	Function DestroyEncoder(IEncoder:IFModEncoder)
	
	Function FModPCMCallback:Int(FmodSound:Byte Ptr, data:Byte Ptr, dataLen:Int)"Win32"
EndExtern

Type TFMODController
	Field fmSystem:TFMODSystem
	Field fmSound:TFMODSound
	Field fmChannel:TFMODChannel
	Field fmInFrequency:Float
	Field fmInChannelCount:Int
	
	Field Encoder:IFModEncoder

	Method _CreateStreamURL2:TFMODSound(fmSystem:TFMODSystem,URL:String,Mode:Int)
		Local sound:TFMODSound
		Local ret:Int
		
		Local s:Byte Ptr
		' 16-bit chars?
		If Mode & FMOD_UNICODE Then
			s = URL.ToWString()
		Else
			s = URL.ToCString()
		End If
	
		Local exInfo:TFMODCreateSoundExInfo2 = New TFMODCreateSoundExInfo2
		exInfo.pcmreadcallback = FModPCMCallback
		sound = TFMODSound._create(bmx_FMOD_System_CreateSound(fmSystem.systemPtr, s, Mode, exInfo, Varptr ret))
		MemFree(s)
		
		Return sound
	EndMethod

	Method Create:TFMODController()
        'If fmSystem = Null Then 'fmSystem = New TFMODSystem.Create()		
 		 fmSystem = New TFMODSystem.Create()	'R100 removed	
         GCCollect()
 		 fmSystem.Init(32)
 		 Return Self
		'EndIf 
	EndMethod
	
	Method TuneTo:Int(RadioStation:String)
		CloseStation()

 		fmSound = _CreateStreamURL2(fmSystem,RadioStation, FMOD_HARDWARE | FMOD_2D | FMOD_CREATESTREAM) ' Changed parameter!

 		If Not fmSound Return False

 		fmSound.GetSoundChannels(fmInChannelCount)
 		fmChannel = fmSystem.PlaySound(0,fmSound)
        fmChannel.setvolume(0)  ' changed!
 		fmChannel.GetFrequency(fmInFrequency)
 
 		Encoder = CreateEncoder()

?debug ' added for extra debug-info
        DebugLog "Encoder created!!! :D" 
?
 		Return True
	EndMethod
	
	Method InitEncoder:Int(EncoderType:Int,OutputFilename$,OutputBitrate:Float)
		If Not Encoder Return False
		Return Encoder.InitEncoder(EncoderType,OutputFilename,fmInChannelCount,fmInFrequency,OutputBitrate)
	EndMethod
	
	Method StartEncoding()
		If Not Encoder Return
		Encoder.StartEncoding()
	EndMethod
	
	Method StopEncoding()
		If Not Encoder Return
		Encoder.StopEncoding()
	EndMethod
	
	Method CloseStation()
		If fmChannel Then fmChannel.Stop() 
		If fmSound Then fmSound.SoundRelease()
         GCCollect()
	EndMethod
	
	Method Close()
		CloseStation()
	EndMethod
EndType


