#include <Utils/Windows.h>
#include <Utils/Hook.h>
#include <Utils/Pattern.h>
#include <Utils/NamedPipeServer.h>

#include "Game.h"
#include "Messagehandler.h"

#include "Rendering/Renderer.h"

#include <d3dx9.h>

CHook<CCallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *> g_presentHook;
CHook<CCallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS *> g_resetHook;

CRenderer g_pRenderer;

void initGame()
{
	HMODULE hMod = NULL;

	while ((hMod = GetModuleHandle("d3d9.dll")) == NULL)
		Sleep(200);

	DWORD *vtbl = 0;
	DWORD dwDevice = findPattern((DWORD) hMod, 0x128000, (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");
	memcpy(&vtbl, (void *) (dwDevice + 0x2), 4);

	g_presentHook.apply(vtbl[17], [](LPDIRECT3DDEVICE9 dev, CONST RECT * a1, CONST RECT * a2, HWND a3, CONST RGNDATA *a4) -> HRESULT
	{
		__asm pushad
		g_pRenderer.Draw(dev);
		__asm popad

		return g_presentHook.callOrig(dev, a1, a2, a3, a4);
	});

	g_resetHook.apply(vtbl[16], [](LPDIRECT3DDEVICE9 dev, D3DPRESENT_PARAMETERS *pp) -> HRESULT
	{
		__asm pushad
		g_pRenderer.Reset(dev);
		__asm popad

		return g_resetHook.callOrig(dev, pp);
	});

	typedef std::map< PipeMessages::ePipeMessages, boost::function<void(CBitStream&, CBitStream&)> > MessagePaketHandler;
	MessagePaketHandler PaketHandler;


	PaketHandler[PipeMessages::TextCreate] = boost::bind(TextCreate, _1, _2);
	PaketHandler[PipeMessages::TextDestroy] = boost::bind(TextDestroy, _1, _2);
	PaketHandler[PipeMessages::TextSetShadow] = boost::bind(TextSetShadow, _1, _2);
	PaketHandler[PipeMessages::TextSetShown] = boost::bind(TextSetShown, _1, _2);
	PaketHandler[PipeMessages::TextSetColor] = boost::bind(TextSetColor, _1, _2);
	PaketHandler[PipeMessages::TextSetPos] = boost::bind(TextSetPos, _1, _2);
	PaketHandler[PipeMessages::TextSetString] = boost::bind(TextSetString, _1, _2);
	PaketHandler[PipeMessages::TextUpdate] = boost::bind(TextUpdate, _1, _2);

	PaketHandler[PipeMessages::BoxCreate] = boost::bind(BoxCreate, _1, _2);
	PaketHandler[PipeMessages::BoxDestroy] = boost::bind(BoxDestroy, _1, _2);
	PaketHandler[PipeMessages::BoxSetShown] = boost::bind(BoxSetShown, _1, _2);
	PaketHandler[PipeMessages::BoxSetBorder] = boost::bind(BoxSetBorder, _1, _2);
	PaketHandler[PipeMessages::BoxSetBorderColor] = boost::bind(BoxSetBorderColor, _1, _2);
	PaketHandler[PipeMessages::BoxSetColor] = boost::bind(BoxSetColor, _1, _2);
	PaketHandler[PipeMessages::BoxSetHeight] = boost::bind(BoxSetHeight, _1, _2);
	PaketHandler[PipeMessages::BoxSetPos] = boost::bind(BoxSetPos, _1, _2);
	PaketHandler[PipeMessages::BoxSetWidth] = boost::bind(BoxSetWidth, _1, _2);

	PaketHandler[PipeMessages::LineCreate] = boost::bind(LineCreate, _1, _2);
	PaketHandler[PipeMessages::LineDestroy] = boost::bind(LineDestroy, _1, _2);
	PaketHandler[PipeMessages::LineSetShown] = boost::bind(LineSetShown, _1, _2);
	PaketHandler[PipeMessages::LineSetColor] = boost::bind(LineSetColor, _1, _2);
	PaketHandler[PipeMessages::LineSetWidth] = boost::bind(LineSetWidth, _1, _2);
	PaketHandler[PipeMessages::LineSetPos] = boost::bind(LineSetPos, _1, _2);

	PaketHandler[PipeMessages::ImageCreate] = boost::bind(ImageCreate, _1, _2);
	PaketHandler[PipeMessages::ImageDestroy] = boost::bind(ImageDestroy, _1, _2);
	PaketHandler[PipeMessages::ImageSetShown] = boost::bind(ImageSetShown, _1, _2);
	PaketHandler[PipeMessages::ImageSetAlign] = boost::bind(ImageSetAlign, _1, _2);
	PaketHandler[PipeMessages::ImageSetPos] = boost::bind(ImageSetPos, _1, _2);
	PaketHandler[PipeMessages::ImageSetRotation] = boost::bind(ImageSetRotation, _1, _2);

	PaketHandler[PipeMessages::DestroyAllVisual] = boost::bind(DestroyAllVisual, _1, _2);
	PaketHandler[PipeMessages::ShowAllVisual] = boost::bind(ShowAllVisual, _1, _2);
	PaketHandler[PipeMessages::HideAllVisual] = boost::bind(HideAllVisual, _1, _2);

	PaketHandler[PipeMessages::GetFrameRate] = boost::bind(GetFrameRate, _1, _2);
	PaketHandler[PipeMessages::GetScreenSpecs] = boost::bind(GetScreenSpecs, _1, _2);

	new CNamedPipeServer("Overlay_Server", [&](CBitStream& bsIn, CBitStream& bsOut)
	{
		BITSTREAM_READ(bsIn, short, eMessage);

		try
		{
			auto it = PaketHandler.find((PipeMessages::ePipeMessages)eMessage);
			if (it == PaketHandler.end())
				return;

			if (!PaketHandler[(PipeMessages::ePipeMessages)eMessage])
				return;

			PaketHandler[(PipeMessages::ePipeMessages)eMessage](bsIn, bsOut);
		}
		catch (...)
		{

		}
	});

	while (true){
		Sleep(100);
	}
}
