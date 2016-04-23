
' Fancy Example by Col & Grisu :D

Strict

Framework brl.system

Import brl.retro
Import brl.eventqueue

Import srs.fmodencoder
Import maxgui.drivers
Import maxgui.xpmanifest

' set up the FMODEncoder
Global NetRadio:IFMODEncoder
CreateFModEncoder(NetRadio)

NetRadio.Init()
NetRadio.SetRadioStation("http://relay.181.fm:8030")




' GUI
Global winMain:TGadget = CreateWindow("Basic Audio Encoder",0,0,600,200,Null,WINDOW_DEFAULT|WINDOW_CENTER ~ WINDOW_MENU)
Global boxEncoderQuality:TGadget=CreateComboBox(150,50,70,25,winMain)
SetGadgetLayout boxEncoderQuality, Null, Null, EDGE_ALIGNED, Null

' compressions
Global Mp3_Compressions:Int[] = [64, 80, 96, 112, 128, 160, 192, 224, 256, 320]
Global Ogg_Compressions:Float[] = [0.005, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]

For Local i:Int = 0 Until 10
	Local Bitrate:String = RSet(String(Mp3_Compressions[i])+" kb",7)
	AddGadgetItem boxEncoderQuality,Bitrate, GADGETITEM_NORMAL
Next



' file
Global butSelectOutfile:TGadget = CreateButton("Select output file:",20,20,110,25,winMain)
Global txtOutFilename:TGadget = CreateTextArea(130,20,430,25,winMain,TEXTAREA_READONLY)
Global butEncode:TGadget = CreateButton("Start encoding",460,50,100,25,winMain)
DisableGadget(butEncode)

Global butOgg:TGadget = CreateButton("Ogg",20,50,50,25,winMain,BUTTON_RADIO)
Global butMp3:TGadget = CreateButton("Mp3",90,50,50,25,winMain,BUTTON_RADIO)
Global prgEncodingBar:TGadget = CreateProgBar(20,90,540,25,winMain)
Local style:Int = GetWindowLongW(TWindowsGadget(prgEncodingBar)._hwnd, GWL_STYLE)
SetWindowLongW(TWindowsGadget(prgEncodingBar)._hwnd , GWL_STYLE, style | PBS_MARQUEE)
SendMessageW(TWindowsGadget(prgEncodingBar)._hwnd, PBM_SETMARQUEE, 0, 50)

Global IsEncoding:Int

' Default to OGG 192Kb
SetButtonState(butOgg,True)
SelectGadgetItem(boxEncoderQuality,6)

' mp3 available?
If FileType(AppDir+"\libmp3lame.dll")=1 Then 
	SetStatusText ( winMain, "Mp3 encoding via LAME - www.mp3dev.org ( lame.sourceforge.net )" )
Else
	SetStatusText ( winMain, "No Mp3 LAME encoder found. Try www.mp3dev.org ( lame.sourceforge.net ) ?" )
	DisableGadget( butMp3 )
EndIf


' main
Repeat
	WaitEvent
	
	Local ev:TEvent = CurrentEvent
	Select ev.id
	Case EVENT_GADGETACTION
		Select ev.source
		Case butSelectOutfile
			OnSelectOutfilePressed()
					
		Case butEncode
			OnEncodeButtonPressed()
			
		EndSelect
			
	Case EVENT_WINDOWCLOSE
		OnWindowClose()
		End

	EndSelect
Forever



Function OnSelectOutfilePressed()
	Local Filter:String
	If ButtonState(butOGG) Filter = "ogg"
	If ButtonState(butMp3) Filter = "mp3"
	
	Local Outfilepath:String = RequestFile("Save As",Filter+":"+Filter,True,"")
	If Outfilepath <> ""
		SetGadgetText(txtOutFilename,Outfilepath)
		EnableGadget(butEncode)
	EndIf
EndFunction

Function OnWindowClose()
	If IsEncoding NetRadio.StopEncoding()
	NetRadio.Close()
EndFunction

Function OnEncodeButtonPressed()
	If IsEncoding
		StopEncoding()
	Else
		StartEncoding()
	EndIf
EndFunction

Function StartEncoding()
	' bitrate
	Local Bitrate:Float
	Local BitrateIndex = SelectedGadgetItem(boxEncoderQuality)
	
	If ButtonState(butMp3)
		Bitrate = Mp3_Compressions[BitrateIndex]
		NetRadio.SetEncoder(SRS_FMOD_ENCODER_TYPE_MP3)
	EndIf		
	If ButtonState(butOgg)
		Bitrate = Ogg_Compressions[BitrateIndex]
		NetRadio.SetEncoder(SRS_FMOD_ENCODER_TYPE_OGG)
	EndIf
	
	NetRadio.SetOutputBitrate(Bitrate)
	
	' filename
	Local OutFilename$ = GadgetText(txtOutFilename)
	NetRadio.SetOutputFilename(OutFilename)
	
	' We're good to go now
	IsEncoding = NetRadio.StartEncoding()
	If Not IsEncoding Return
	IsEncoding = True
	
	SetGadgetText(butEncode,"Stop encoding")
	
	SendMessageW(TWindowsGadget(prgEncodingBar)._hwnd,PBM_SETMARQUEE,True,0)
	
	DisableGadget(butOgg)
	DisableGadget(butMp3)
	DisableGadget(butSelectOutfile)
	DisableGadget(txtOutfilename)
	DisableGadget(boxEncoderQuality)
EndFunction

Function StopEncoding()
	IsEncoding = False
	NetRadio.StopEncoding()

	SetGadgetText(butEncode,"Start encoding")
	
	SendMessageW(TWindowsGadget(prgEncodingBar)._hwnd,PBM_SETMARQUEE,False,0)
	RedrawGadget(prgEncodingBar)
	
	EnableGadget(butOgg)
	EnableGadget(butMp3)
	EnableGadget(butSelectOutfile)
	EnableGadget(txtOutfilename)
	EnableGadget(boxEncoderQuality)
EndFunction