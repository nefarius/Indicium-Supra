#SingleInstance, force
#NoEnv

#include ..\..\include\ahk\overlay.ahk

SetParam("process", "GFXTest.exe")

TextCreate("Arial", 25, false, false, 100, 100, 0xFFFFFFFF, "Hello World", true, true)
BoxCreate(200, 200, 200, 200, 0xFFFFFFFF, true)
return

~1::
ExitApp
return