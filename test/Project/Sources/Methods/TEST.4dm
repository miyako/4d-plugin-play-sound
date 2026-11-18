//%attributes = {}
$file:=Get 4D folder:C485(Current resources folder:K5:16)+"sample.mp3"

$info:=process sound file ($file;sound action open)  //info is returned only on windows

$info:=process sound file ($file;sound action play)

$info:=process sound file ($file;sound action pause)

$info:=process sound file ($file;sound action resume)

If (Is macOS:C1572)
	
	
	Repeat 
		
	Until (process sound file ($file;sound playing))
	
	
End if 

$info:=process sound file ($file;sound action stop)

$info:=process sound file ($file;sound action close)