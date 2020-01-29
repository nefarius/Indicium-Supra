/*
MIT License

Copyright (c) 2018-2019 Benjamin Höglinger

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


#include "AudioRenderClientHook.h"
#include "Exceptions.hpp"

using namespace Indicium::Core::Exceptions;

CoreAudioHooking::AudioRenderClientHook::AudioRenderClientHook()
{
	// Note: may fail if already called by host
	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	HRESULT hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator),
		nullptr,
		CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator),
		reinterpret_cast<void**>(&enumerator)
	);
	if (FAILED(hr))
	{
		throw ARCException("CoCreateInstance failed", hr);
	}

	hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
	if (FAILED(hr))
	{
		throw ARCException("Failed to get default audio endpoint", hr);
	}

	hr = device->Activate(
		__uuidof(IAudioClient),
		CLSCTX_ALL,
		nullptr,
		reinterpret_cast<void**>(&client)
	);
	if (FAILED(hr))
	{
		throw ARCException("Failed to activate IAudioClient instance", hr);
	}

	client->GetMixFormat(&pwfx);

	hr = client->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		0,
		10 * 1000 * 1000,
		0,
		pwfx,
		nullptr
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
			nullptr
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
			nullptr
		);
	}
	if (FAILED(hr))
	{
		throw ARCException("Failed to initialize IAudioClient instance", hr);
	}

	hr = client->GetService(
		__uuidof(IAudioRenderClient),
		reinterpret_cast<void**>(&arc)
	);
	if (FAILED(hr))
	{
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
	return std::vector<size_t>(*reinterpret_cast<size_t**>(arc), *reinterpret_cast<size_t**>(arc) + VTableElements);
}
