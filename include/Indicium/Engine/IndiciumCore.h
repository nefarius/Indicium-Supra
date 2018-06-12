#ifndef IndiciumCore_h__
#define IndiciumCore_h__

#define INDICIUM_API

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum _INDICIUM_ERRORS
    {
        INDICIUM_ERROR_NONE = 0x20000000,
        INDICIUM_ERROR_WHATEVER = 0xE0000001
    } INDICIUM_ERROR;

    typedef struct _INDICIUM_ENGINE *PINDICIUM_ENGINE;

    typedef struct _INDICIUM_D3D9_EVENT_CALLBACKS *PINDICIUM_D3D9_EVENT_CALLBACKS;
    typedef struct _INDICIUM_D3D10_EVENT_CALLBACKS *PINDICIUM_D3D10_EVENT_CALLBACKS;
    typedef struct _INDICIUM_D3D11_EVENT_CALLBACKS *PINDICIUM_D3D11_EVENT_CALLBACKS;
    typedef struct _INDICIUM_D3D12_EVENT_CALLBACKS *PINDICIUM_D3D12_EVENT_CALLBACKS;

    INDICIUM_API PINDICIUM_ENGINE IndiciumEngineAlloc(void);

    INDICIUM_API VOID IndiciumEngineSetD3D9EventCallbacks(
        _In_
        PINDICIUM_ENGINE Engine,
        _In_
        PINDICIUM_D3D9_EVENT_CALLBACKS Callbacks
    );

    INDICIUM_API VOID IndiciumEngineSetD3D10EventCallbacks(
        _In_
        PINDICIUM_ENGINE Engine,
        _In_
        PINDICIUM_D3D10_EVENT_CALLBACKS Callbacks
    );

    INDICIUM_API VOID IndiciumEngineSetD3D11EventCallbacks(
        _In_
        PINDICIUM_ENGINE Engine,
        _In_
        PINDICIUM_D3D11_EVENT_CALLBACKS Callbacks
    );

    INDICIUM_API VOID IndiciumEngineSetD3D12EventCallbacks(
        _In_
        PINDICIUM_ENGINE Engine,
        _In_
        PINDICIUM_D3D12_EVENT_CALLBACKS Callbacks
    );

#ifdef __cplusplus
}
#endif

#endif // IndiciumCore_h__
