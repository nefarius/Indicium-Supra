#include "AudioRenderClientHook.h"
#include "Exceptions.hpp"

using namespace Indicium::Core::Exceptions;

CoreAudioHooking::AudioRenderClientHook::AudioRenderClientHook()
{
    HRESULT hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        NULL, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator),
        reinterpret_cast<void**>(&enumerator)
    );
    if (FAILED(hr)) {
        throw ARCException("CoCreateInstance failed", hr);
    }

    hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
    if (FAILED(hr)) {
        throw ARCException("Failed to get default audio endpoint", hr);
    }

    hr = device->Activate(
        __uuidof(IAudioClient),
        CLSCTX_ALL,
        NULL,
        reinterpret_cast<void**>(&client)
    );
    if (FAILED(hr)) {
        throw ARCException("Failed to activate IAudioClient instance", hr);
    }

    client->GetMixFormat(&pwfx);

    hr = client->Initialize(
        AUDCLNT_SHAREMODE_SHARED,
        0,
        10 * 1000 * 1000,
        0,
        pwfx,
        NULL
    );
    if ((S_OK != hr) && (pwfx->nSamplesPerSec != 48000))
    {
        pwfx->nSamplesPerSec = 48000;
        hr = client->Initialize(
            AUDCLNT_SHAREMODE_SHARED,
            0,
            10 * 1000 * 1000,
            0,
            pwfx,
            NULL
        );
    }
    else if ((S_OK != hr) && (pwfx->nSamplesPerSec != 44100))
    {
        pwfx->nSamplesPerSec = 44100;
        hr = client->Initialize(
            AUDCLNT_SHAREMODE_SHARED,
            0,
            10 * 1000 * 1000,
            0,
            pwfx,
            NULL
        );
    }
    if (FAILED(hr)) {
        throw ARCException("Failed to initialize IAudioClient instance", hr);
    }

    hr = client->GetService(
        __uuidof(IAudioRenderClient),
        reinterpret_cast<void**>(&arc)
    );
    if (FAILED(hr)) {
        throw ARCException("Failed to request IAudioRenderClient service", hr);
    }
}

CoreAudioHooking::AudioRenderClientHook::~AudioRenderClientHook()
{
    if (arc)
        arc->Release();
    if (client)
        client->Release();
    if (device)
        device->Release();
    if (enumerator)
        enumerator->Release();
    if (pwfx)
        CoTaskMemFree(pwfx);
}

std::vector<size_t> CoreAudioHooking::AudioRenderClientHook::vtable() const
{
    size_t vtbl[VTableElements];
    memcpy(vtbl, *reinterpret_cast<size_t **>(arc), VTableElements * sizeof(size_t));
    return std::vector<size_t>(vtbl, vtbl + sizeof vtbl / sizeof vtbl[0]);
}
