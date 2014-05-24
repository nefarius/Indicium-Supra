#SingleInstance, force
#NoEnv

#include ..\..\include\ahk\overlay.ahk

; http://www.unknowncheats.me/forum/general-programming-and-reversing/105528-gfxtest-lightweight-graphics-testing-application.html
; Thanks to evolution536 and learn_more for this test application :)
SetParam("process", "GFXTest.exe")

TextCreate("Arial", 25, false, false, 100, 100, 0xFFFFFFFF, "Hello {ffff00}World", true, true)
BoxCreate(200, 200, 200, 200, 0xFFFFFFFF, true)
return

~1::
ExitApp
return