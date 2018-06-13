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

#include <Indicium\Engine\IndiciumCore.h>
#include <Indicium\Engine\IndiciumDirect3D11.h>

#ifdef _M_IX86
#pragma comment(lib, "x86\\FW1FontWrapper.lib")
#else
#pragma comment(lib, "x64\\FW1FontWrapper.lib")
#endif
#pragma comment(lib, "dxguid.lib")


PINDICIUM_ENGINE engine = nullptr;

EVT_INDICIUM_D3D11_PRESENT EvtIndiciumD3D11Present;


/**
 * \fn  BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
 *
 * \brief   Your typical DLL entry point function. We're not doing much here since a special
 *          initialization routine gets called upon getting loaded by Indicium-Supra.
 *
 * \author  Benjamin "Nefarius" Höglinger
 * \date    05.05.2018
 *
 * \param   hInstance   The instance.
 * \param   dwReason    The reason.
 * \param   parameter3  The third parameter.
 *
 * \return  A WINAPI.
 */
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
    //
    // We don't need to get notified in thread attach- or detachments
    // 
    DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

    INDICIUM_D3D11_EVENT_CALLBACKS d3d11;
    INDICIUM_D3D11_EVENT_CALLBACKS_INIT(&d3d11);
    d3d11.EvtIndiciumD3D11PrePresent = EvtIndiciumD3D11Present;

    INDICIUM_ERROR err;

    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:

        if (!engine)
        {
            engine = IndiciumEngineAlloc();

            IndiciumEngineSetD3D11EventCallbacks(engine, &d3d11);

            err = IndiciumEngineInit(engine, NULL);
        }

        break;
    case DLL_PROCESS_DETACH:

        if (engine)
        {
            IndiciumEngineShutdown(engine);
            IndiciumEngineFree(engine);
        }

        break;
    default:
        break;
    }

    return TRUE;
}

void EvtIndiciumD3D11Present(
    IDXGISwapChain  *pSwapChain,
    UINT            SyncInterval,
    UINT            Flags
)
{
    static ID3D11DeviceContext* ctx = nullptr;
    static ID3D11Device* dev = nullptr;

    D3D11_DEVICE_CONTEXT_FROM_SWAPCHAIN(pSwapChain, &dev, &ctx);

    static IFW1Factory *pFW1Factory = nullptr;
    static HRESULT hResult;
    if (!pFW1Factory)
        hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);

    static IFW1FontWrapper *pFontWrapper = nullptr;
    if (!pFontWrapper)
        hResult = pFW1Factory->CreateFontWrapper(dev, L"Arial", &pFontWrapper);

    static auto x = 1280.0f;

    //
    // Very crude marquee mechanism for demonstration
    // 
    if (x <= -1280.0f)x = 1280.0f;

    pFontWrapper->DrawString(
        ctx,
        L"Injected via Indicium-Supra by Nefarius",// String
        50.0f,// Font size
        x--,// X position
        30.0f,// Y position
        0xff0099ff,// Text color, 0xAaBbGgRr
        FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
    );

    //
    // TODO: don't release and re-create per frame as it's expensive
    // 
    // pFontWrapper->Release();
    // pFontWrapper = nullptr;
    // pFW1Factory->Release();
    // pFW1Factory = nullptr;
}
