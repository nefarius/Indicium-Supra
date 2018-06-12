#ifndef IndiciumCore_h__
#define IndiciumCore_h__

#define INDICIUM_API

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum _INDICIUM_ERRORS
    {
        INDICIUM_ERROR_NONE = 0x20000000,
        INDICIUM_ERROR_INVALID_ENGINE_HANDLE = 0xE0000001,
        INDICIUM_ERROR_CREATE_THREAD_FAILED = 0xE0000002
    } INDICIUM_ERROR;

    typedef enum _INDICIUM_D3D_VERSION {
        IndiciumDirect3DVersion9 = 0,
        IndiciumDirect3DVersion10 = 1 << 0,
        IndiciumDirect3DVersion11 = 1 << 1,
        IndiciumDirect3DVersion12 = 1 << 2
    } INDICIUM_D3D_VERSION, *PINDICIUM_D3D_VERSION;

    //
    // Indicium engine handle
    // 
    typedef struct _INDICIUM_ENGINE *PINDICIUM_ENGINE;

    //
    // Forward declarations of version-specific callbacks
    // 
    typedef struct _INDICIUM_D3D9_EVENT_CALLBACKS *PINDICIUM_D3D9_EVENT_CALLBACKS;
    typedef struct _INDICIUM_D3D10_EVENT_CALLBACKS *PINDICIUM_D3D10_EVENT_CALLBACKS;
    typedef struct _INDICIUM_D3D11_EVENT_CALLBACKS *PINDICIUM_D3D11_EVENT_CALLBACKS;
    typedef struct _INDICIUM_D3D12_EVENT_CALLBACKS *PINDICIUM_D3D12_EVENT_CALLBACKS;

    typedef
        _Function_class_(EVT_INDICIUM_GAME_HOOKED)
        VOID
        EVT_INDICIUM_GAME_HOOKED(
            const INDICIUM_D3D_VERSION GameVersion
        );

    typedef EVT_INDICIUM_GAME_HOOKED *PFN_EVT_INDICIUM_GAME_HOOKED;

    INDICIUM_API PINDICIUM_ENGINE IndiciumEngineAlloc(void);

    INDICIUM_API INDICIUM_ERROR IndiciumEngineInit(
        _In_
        PINDICIUM_ENGINE Engine,
        _In_
        PFN_EVT_INDICIUM_GAME_HOOKED EvtIndiciumGameHooked
    );

    INDICIUM_API VOID IndiciumEngineShutdown(
        _In_
        PINDICIUM_ENGINE Engine
    );

    INDICIUM_API VOID IndiciumEngineFree(
        _In_
        PINDICIUM_ENGINE Engine
    );

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
