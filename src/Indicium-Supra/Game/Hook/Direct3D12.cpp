#include "Direct3D12.h"
#include <dxgi.h>
#include <dxgi1_4.h>
#include "Utils/Misc.h"
#include <d3d12.h>
#include "DXGI.h"


Direct3D12Hooking::Direct3D12::Direct3D12() : vtableSwapChain(nullptr), pd3dDevice(nullptr), pQueue(nullptr), pSwapChain(nullptr)
{
    auto& logger = Logger::get(LOG_REGION());

    logger.information("Acquiring VTable for ID3D12Device and IDXGISwapChain...");

    auto hModDXGI = GetModuleHandle("DXGI.dll");
    auto hModD3D12 = GetModuleHandle("D3D12.dll");

    if (hModDXGI == nullptr)
    {
        logger.error("Couldn't get handle to DXGI.dll");
        return;
    }

    if (hModD3D12 == nullptr)
    {
        logger.error("Couldn't get handle to D3D12.dll");
        return;
    }

    auto hD3D12CreateDevice = static_cast<LPVOID>(GetProcAddress(hModD3D12, "D3D12CreateDevice"));

    if (hD3D12CreateDevice == nullptr)
    {
        logger.error("Couldn't get handle to D3D12CreateDevice");
        return;
    }

    auto hCreateDXGIFactory1 = static_cast<LPVOID>(GetProcAddress(hModDXGI, "CreateDXGIFactory1"));
    if (hCreateDXGIFactory1 == nullptr)
    {
        logger.error("Couldn't get handle to CreateDXGIFactory1");
        return;
    }

    IDXGIFactory4* pFactory;
    auto hr = static_cast<HRESULT(WINAPI *)(
        REFIID,
        void**)>(hCreateDXGIFactory1)(IID_PPV_ARGS(&pFactory));

    if (FAILED(hr))
    {
        logger.error("Couldn't create DXGI factory");
        return;
    }

    auto hr12 = static_cast<HRESULT(WINAPI *)(
        IUnknown*,
        D3D_FEATURE_LEVEL,
        REFIID,
        void**)>(hD3D12CreateDevice)(
            nullptr,
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&pd3dDevice));

    if (FAILED(hr12))
    {
        logger.error("Couldn't create D3D12 device");
        return;
    }

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    hr = pd3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pQueue));

    if (FAILED(hr))
    {
        logger.error("Command queue creation failed");
        return;
    }

    DXGI_SWAP_CHAIN_DESC scDesc;
    ZeroMemory(&scDesc, sizeof(scDesc));
    scDesc.BufferCount = 2;
    scDesc.BufferDesc.Width = 0;
    scDesc.BufferDesc.Height = 0;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.RefreshRate.Numerator = 60;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow = temp_window.GetWindowHandle();
    scDesc.SampleDesc.Count = 1;
    scDesc.SampleDesc.Quality = 0;
    scDesc.Windowed = TRUE;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    hr = pFactory->CreateSwapChain(
        pQueue,
        &scDesc,
        &pSwapChain
    );

    if (FAILED(hr))
    {
        logger.error("Swap chain creation failed");
        return;
    }

    vtable = *reinterpret_cast<UINTX **>(pd3dDevice);
    vtableSwapChain = *reinterpret_cast<UINTX **>(pSwapChain);

    logger.information("VTable acquired");
}


Direct3D12Hooking::Direct3D12::~Direct3D12()
{
    auto& logger = Logger::get(LOG_REGION());

    logger.information("Releasing temporary objects");

    if (pSwapChain)
        pSwapChain->Release();

    if (pd3dDevice)
        pd3dDevice->Release();

    if (pQueue)
        pQueue->Release();
}

bool Direct3D12Hooking::Direct3D12::GetDeviceVTable(UINTX* pVTable) const
{
    // TODO: get vtable ordinals and implement
    return false;
}

bool Direct3D12Hooking::Direct3D12::GetSwapChainVTable(UINTX* pVTable) const
{
    if (vtableSwapChain)
    {
        memcpy(pVTable, vtableSwapChain, DXGIHooking::DXGI::SwapChainVTableElements * sizeof(UINTX));
        return true;
    }

    return false;
}
