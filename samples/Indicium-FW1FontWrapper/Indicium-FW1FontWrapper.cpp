// Indicium-FW1FontWrapper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FW1FontWrapper.h"
#include "../../include/IndiciumPlugin.h"

#ifdef _M_IX86
#pragma comment(lib, "x86\\FW1FontWrapper.lib")
#else
#pragma comment(lib, "x64\\FW1FontWrapper.lib")
#endif
#pragma comment(lib, "dxguid.lib")


INDICIUM_EXPORT Present(IID guid, LPVOID unknown, Direct3DVersion version)
{
    if (!IS_DIRECT3D11(version)) return;
    
    // get swapchain
    auto chain = static_cast<IDXGISwapChain*>(unknown);

    static ID3D11DeviceContext* ctx = nullptr;
    static ID3D11Device* dev = nullptr;

    // get device
    chain->GetDevice(__uuidof(dev), reinterpret_cast<void**>(&dev));

    // get device context
    dev->GetImmediateContext(&ctx);

    static IFW1Factory *pFW1Factory = nullptr;
    static HRESULT hResult;
    if (!pFW1Factory)
        hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);

    static IFW1FontWrapper *pFontWrapper = nullptr;
    if (!pFontWrapper)
        hResult = pFW1Factory->CreateFontWrapper(dev, L"Arial", &pFontWrapper);

    static auto x = 1280.0f;

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

    //pFontWrapper->Release();
    //pFW1Factory->Release();
}
