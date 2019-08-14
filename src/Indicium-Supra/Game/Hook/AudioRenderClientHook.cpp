#include "AudioRenderClientHook.h"

CoreAudioHooking::AudioRenderClientHook::AudioRenderClientHook()
{
    HRESULT hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        NULL, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator),
        reinterpret_cast<void**>(&enumerator)
    );

    hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);

    hr = device->Activate(
        __uuidof(IAudioClient),
        CLSCTX_ALL,
        NULL,
        reinterpret_cast<void**>(&client)
    );

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

    hr = client->GetService(
        __uuidof(IAudioRenderClient), 
        reinterpret_cast<void**>(&arc)
    );
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
