#pragma once

#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <vector>

namespace CoreAudioHooking
{
    enum AudioRenderClientVTbl : short
    {
        // IUnknown
        QueryInterface = 0,
        AddRef = 1,
        Release = 2,

        // IAudioRenderClient
        GetBuffer = 3,
        ReleaseBuffer = 4
    };

    class AudioRenderClientHook
    {
        IMMDeviceEnumerator *enumerator{};
        IMMDevice *device{};
        IAudioClient *client{};
        WAVEFORMATEX *pwfx{};
        IAudioRenderClient *arc{};

    public:
        AudioRenderClientHook();
        ~AudioRenderClientHook();

        static const int VTableElements = 5;

        std::vector<size_t> vtable() const;
    };
};
