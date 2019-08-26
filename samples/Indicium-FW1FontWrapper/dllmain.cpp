/*
MIT License

Copyright (c) 2018 Benjamin Höglinger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "FW1FontWrapper.h"

#include <Indicium\Engine\IndiciumDirect3D11.h>
#include <cassert>

#ifdef _M_IX86
#pragma comment(lib, "x86\\FW1FontWrapper.lib")
#else
#pragma comment(lib, "x64\\FW1FontWrapper.lib")
#endif
#pragma comment(lib, "dxguid.lib")


typedef struct _FW1_CTX
{
	ID3D11DeviceContext* ctx;
	ID3D11Device* dev;
	IFW1Factory *pFW1Factory;
	IFW1FontWrapper *pFontWrapper;

} FW1_CTX, *PFW1_CTX;

EVT_INDICIUM_GAME_HOOKED EvtIndiciumGameHooked;
EVT_INDICIUM_D3D11_PRE_PRESENT EvtIndiciumD3D11PrePresent;
EVT_INDICIUM_GAME_UNHOOKED EvtIndiciumGamePostUnhooked;

/**
 * \fn	BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
 *
 * \brief	Your typical DLL entry point function. We're not doing much here since a special
 * 			initialization routine gets called upon getting loaded by Indicium-Supra.
 *
 * \author	Benjamin "Nefarius" Höglinger
 * \date	05.05.2018
 *
 * \param	hInstance 	The instance.
 * \param	dwReason  	The reason.
 * \param	parameter3	The third parameter.
 *
 * \returns	A WINAPI.
 */
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	//
	// We don't need to get notified in thread attach- or detachments
	// 
	DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

	INDICIUM_ENGINE_CONFIG cfg;
	INDICIUM_ENGINE_CONFIG_INIT(&cfg);

	// Only attempt to detect and hook D3D11
	cfg.Direct3D.HookDirect3D11 = TRUE;
	// Called once game as been hooked
	cfg.EvtIndiciumGameHooked = EvtIndiciumGameHooked;
	// Called after hooks have been removed
	cfg.EvtIndiciumGamePostUnhook = EvtIndiciumGamePostUnhooked;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:

		//
		// Bootstrap the engine. Allocates resources, establishes hooks etc.
		// 
		(void)IndiciumEngineCreate(
			static_cast<HMODULE>(hInstance),
			&cfg,
			NULL
		);

		break;
	case DLL_PROCESS_DETACH:

		//
		// Tears down the engine. Graceful shutdown, frees resources etc.
		// 
		(void)IndiciumEngineDestroy(static_cast<HMODULE>(hInstance));

		break;
	default:
		break;
	}

	return TRUE;
}

//
// Essential game functions successfully hooked, do further bootstrapping here
// 
void EvtIndiciumGameHooked(
	PINDICIUM_ENGINE EngineHandle,
	const INDICIUM_D3D_VERSION GameVersion
)
{
	//
	// At this stage we assume D3D11
	// 
	assert(GameVersion == IndiciumDirect3DVersion11);

	PFW1_CTX pCtx = nullptr;

	//
	// Allocate context memory
	// 
	assert(IndiciumEngineAllocCustomContext(
		EngineHandle,
		(PVOID*)&pCtx,
		sizeof(FW1_CTX)
	) == INDICIUM_ERROR_NONE);

	INDICIUM_D3D11_EVENT_CALLBACKS d3d11;
	INDICIUM_D3D11_EVENT_CALLBACKS_INIT(&d3d11);
	d3d11.EvtIndiciumD3D11PrePresent = EvtIndiciumD3D11PrePresent;

	// Begin invoking render hook callbacks
	IndiciumEngineSetD3D11EventCallbacks(EngineHandle, &d3d11);
}

//
// Game unloading, hooks are removed
// 
void EvtIndiciumGamePostUnhooked(PINDICIUM_ENGINE EngineHandle)
{
	const PFW1_CTX pCtx = PFW1_CTX(IndiciumEngineGetCustomContext(EngineHandle));

	if (pCtx->pFontWrapper)
		pCtx->pFontWrapper->Release();
	if (pCtx->pFW1Factory)
		pCtx->pFW1Factory->Release();
}

//
// Present is about to get called
// 
void EvtIndiciumD3D11PrePresent(
	IDXGISwapChain					*pSwapChain,
	UINT							SyncInterval,
	UINT							Flags,
	PINDICIUM_EVT_PRE_EXTENSION     Extension
)
{
	// Grab shared context
	const PFW1_CTX pCtx = PFW1_CTX(Extension->Context);
	ID3D11Device *pDeviceTmp;

	if (!pCtx->pFW1Factory)
		(void)FW1CreateFactory(FW1_VERSION, &pCtx->pFW1Factory);

	/*
	 * Swapchain associated Device and Context pointers can become invalid
	 * when the host process destroys and re-creates them (like RetroArch
	 * does when switching cores) so compare to ones grabbed earlier and
	 * re-request both if necessary.
	 */
	if (FAILED(D3D11_DEVICE_FROM_SWAPCHAIN(pSwapChain, &pDeviceTmp))) {
		IndiciumEngineLogError("Failed to get device pointer from swapchain");
		return;
	}
	else if (pCtx->dev != pDeviceTmp) {
		D3D11_DEVICE_IMMEDIATE_CONTEXT_FROM_SWAPCHAIN(
			pSwapChain,
			&pCtx->dev,
			&pCtx->ctx
		);

		(void)pCtx->pFW1Factory->CreateFontWrapper(pCtx->dev, L"Arial", &pCtx->pFontWrapper);
	}

	pCtx->pFontWrapper->DrawString(
		pCtx->ctx,
		L"Injected via Indicium-Supra by Nefarius",// String
		50.0f,// Font size
		15.0f,// X position
		30.0f,// Y position
		0xff0099ff,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}
