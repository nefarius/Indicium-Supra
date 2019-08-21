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


#ifndef IndiciumCoreAudio_h__
#define IndiciumCoreAudio_h__

#include "IndiciumCore.h"
#include <Audioclient.h>

typedef
_Function_class_(EVT_INDICIUM_ARC_PRE_GET_BUFFER)
VOID
EVT_INDICIUM_ARC_PRE_GET_BUFFER(
    IAudioRenderClient*             client,
    UINT32                          NumFramesRequested,
    BYTE                            **ppData,
    PINDICIUM_EVT_PRE_EXTENSION     Extension
);

typedef EVT_INDICIUM_ARC_PRE_GET_BUFFER *PFN_INDICIUM_ARC_PRE_GET_BUFFER;

typedef
_Function_class_(EVT_INDICIUM_ARC_POST_GET_BUFFER)
VOID
EVT_INDICIUM_ARC_POST_GET_BUFFER(
    IAudioRenderClient*             client,
    UINT32                          NumFramesRequested,
    BYTE                            **ppData,
    PINDICIUM_EVT_POST_EXTENSION    Extension
);

typedef EVT_INDICIUM_ARC_POST_GET_BUFFER *PFN_INDICIUM_ARC_POST_GET_BUFFER;

typedef
_Function_class_(EVT_INDICIUM_ARC_PRE_RELEASE_BUFFER)
VOID
EVT_INDICIUM_ARC_PRE_RELEASE_BUFFER(
    IAudioRenderClient*             client,
    UINT32                          NumFramesWritten,
    DWORD                           dwFlags,
    PINDICIUM_EVT_PRE_EXTENSION     Extension
);

typedef EVT_INDICIUM_ARC_PRE_RELEASE_BUFFER *PFN_INDICIUM_ARC_PRE_RELEASE_BUFFER;

typedef
_Function_class_(EVT_INDICIUM_ARC_POST_RELEASE_BUFFER)
VOID
EVT_INDICIUM_ARC_POST_RELEASE_BUFFER(
    IAudioRenderClient*             client,
    UINT32                          NumFramesWritten,
    DWORD                           dwFlags,
    PINDICIUM_EVT_POST_EXTENSION    Extension
);

typedef EVT_INDICIUM_ARC_POST_RELEASE_BUFFER *PFN_INDICIUM_ARC_POST_RELEASE_BUFFER;


typedef struct _INDICIUM_ARC_EVENT_CALLBACKS
{
    PFN_INDICIUM_ARC_PRE_GET_BUFFER         EvtIndiciumARCPreGetBuffer;
    PFN_INDICIUM_ARC_POST_GET_BUFFER        EvtIndiciumARCPostGetBuffer;
    
    PFN_INDICIUM_ARC_PRE_RELEASE_BUFFER     EvtIndiciumARCPreReleaseBuffer;
    PFN_INDICIUM_ARC_POST_RELEASE_BUFFER    EvtIndiciumARCPostReleaseBuffer;

} INDICIUM_ARC_EVENT_CALLBACKS, *PINDICIUM_ARC_EVENT_CALLBACKS;

VOID FORCEINLINE INDICIUM_ARC_EVENT_CALLBACKS_INIT(
    _Out_ PINDICIUM_ARC_EVENT_CALLBACKS Callbacks
)
{
    ZeroMemory(Callbacks, sizeof(INDICIUM_ARC_EVENT_CALLBACKS));
}

#endif // IndiciumCoreAudio_h__
