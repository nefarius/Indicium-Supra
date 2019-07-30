/*
MIT License

Copyright (c) 2018 Benjamin H�glinger

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


#ifndef IndiciumCore_h__
#define IndiciumCore_h__


#ifdef INDICIUM_DYNAMIC
#ifdef INDICIUM_EXPORTS
#define INDICIUM_API __declspec(dllexport)
#else
#define INDICIUM_API __declspec(dllimport)
#endif
#else
#define INDICIUM_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum _INDICIUM_ERRORS
    {
        INDICIUM_ERROR_NONE = 0x20000000,
        INDICIUM_ERROR_INVALID_ENGINE_HANDLE = 0xE0000001,
        INDICIUM_ERROR_CREATE_THREAD_FAILED = 0xE0000002,
        INDICIUM_ERROR_ENGINE_ALLOCATION_FAILED = 0xE0000003,
        INDICIUM_ERROR_ENGINE_ALREADY_ALLOCATED = 0xE0000004,
        INDICIUM_ERROR_INVALID_HMODULE_HANDLE = 0xE0000005,
        INDICIUM_ERROR_REFERENCE_INCREMENT_FAILED = 0xE0000006,

    } INDICIUM_ERROR;

    typedef enum _INDICIUM_D3D_VERSION {
        IndiciumDirect3DVersionUnknown = 0,
        IndiciumDirect3DVersion9 = 1 << 0,
        IndiciumDirect3DVersion10 = 1 << 1,
        IndiciumDirect3DVersion11 = 1 << 2,
        IndiciumDirect3DVersion12 = 1 << 3
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
            PINDICIUM_ENGINE EngineHandle,
            const INDICIUM_D3D_VERSION GameVersion
        );

    typedef EVT_INDICIUM_GAME_HOOKED *PFN_INDICIUM_GAME_HOOKED;

    typedef
        _Function_class_(EVT_INDICIUM_GAME_UNHOOKED)
        VOID
        EVT_INDICIUM_GAME_UNHOOKED(
            PINDICIUM_ENGINE EngineHandle
        );

    typedef EVT_INDICIUM_GAME_UNHOOKED *PFN_INDICIUM_GAME_UNHOOKED;

    typedef struct _INDICIUM_ENGINE_CONFIG
    {
        //
        // Event callback invoked once a requested render API has been hooked successfully
        // 
        PFN_INDICIUM_GAME_HOOKED EvtIndiciumGameHooked;

        //
        // Event callback invoked prior to unhooking the render API
        // 
        PFN_INDICIUM_GAME_UNHOOKED EvtIndiciumGamePreUnhook;

        //
        // Event callback invoked after render API has been unhooked
        // 
        PFN_INDICIUM_GAME_UNHOOKED EvtIndiciumGamePostUnhook;

        struct
        {
            //
            // Enables detection and hooking of Direct3D 9 render pipeline, if used by host process
            // 
            BOOL HookDirect3D9;

            //
            // Enables detection and hooking of Direct3D 10 render pipeline, if used by host process
            // 
            BOOL HookDirect3D10;

            //
            // Enables detection and hooking of Direct3D 11 render pipeline, if used by host process
            // 
            BOOL HookDirect3D11;

            //
            // Enables detection and hooking of Direct3D 12 render pipeline, if used by host process
            // 
            BOOL HookDirect3D12;
        };

        struct
        {
            BOOL EnableLogging;

            PCSTR LogFilePath;
        };

    } INDICIUM_ENGINE_CONFIG, *PINDICIUM_ENGINE_CONFIG;

    /**
     * \fn  VOID FORCEINLINE INDICIUM_ENGINE_CONFIG_INIT( PINDICIUM_ENGINE_CONFIG EngineConfig )
     *
     * \brief   Initializes an INDICIUM_ENGINE_CONFIG struct.
     *
     * \author  Benjamin H�glinger-Stelzer
     * \date    30.07.2019
     *
     * \param   EngineConfig    The engine configuration.
     *
     * \returns Nothing.
     */
    VOID FORCEINLINE INDICIUM_ENGINE_CONFIG_INIT(
        PINDICIUM_ENGINE_CONFIG EngineConfig
    )
    {
        ZeroMemory(EngineConfig, sizeof(INDICIUM_ENGINE_CONFIG));

        EngineConfig->EnableLogging = TRUE;
        EngineConfig->LogFilePath = "%TEMP%\\Indicium-Supra.log";
    }

    INDICIUM_API INDICIUM_ERROR IndiciumEngineCreate(
        _In_ HMODULE HostInstance,
        _In_ PINDICIUM_ENGINE_CONFIG EngineConfig,
        _Out_opt_ PINDICIUM_ENGINE* Engine
    );

    INDICIUM_API INDICIUM_ERROR IndiciumEngineDestroy(
        _In_ HMODULE HostInstance
    );

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineShutdown( _In_ PINDICIUM_ENGINE Engine, _In_opt_ PFN_INDICIUM_GAME_UNHOOKED EvtIndiciumGameUnhooked );
     *
     * \brief   Gracefully shuts down the Indicium engine, unhooking from the render pipeline during
     *          runtime. After this call, the engine can either be initialized once again or the
     *          handle must be freed with IndiciumEngineFree.
     *
     * \author  Benjamin H�glinger-Stelzer
     * \date    05.05.2019
     *
     * \param   Engine                  The engine handle.
     * \param   EvtIndiciumGameUnhooked The optional callback invoked once the render pipeline has
     *                                  been unhooked.
     *
     * \returns Nothing.
     */
    INDICIUM_API VOID IndiciumEngineShutdown(
        _In_
        PINDICIUM_ENGINE Engine,
        _In_opt_
        PFN_INDICIUM_GAME_UNHOOKED EvtIndiciumGameUnhooked
    );

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineFree( _In_ PINDICIUM_ENGINE Engine );
     *
     * \brief   Frees memory allocated by an Indicium engine handle.
     *
     * \author  Benjamin H�glinger-Stelzer
     * \date    05.05.2019
     *
     * \param   Engine  The engine handle to free.
     *
     * \returns Nothing.
     */
    INDICIUM_API VOID IndiciumEngineFree(
        _In_
        PINDICIUM_ENGINE Engine
    );

#ifndef INDICIUM_NO_D3D9

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineSetD3D9EventCallbacks( _In_ PINDICIUM_ENGINE Engine, _In_ PINDICIUM_D3D9_EVENT_CALLBACKS Callbacks );
     *
     * \brief   Registers one or more Direct3D 9(Ex) render pipeline callbacks.
     *
     * \author  Benjamin H�glinger-Stelzer
     * \date    05.05.2019
     *
     * \param   Engine      The engine handle.
     * \param   Callbacks   The callback collection to register.
     *
     * \returns Nothing.
     */
    INDICIUM_API VOID IndiciumEngineSetD3D9EventCallbacks(
        _In_
        PINDICIUM_ENGINE Engine,
        _In_
        PINDICIUM_D3D9_EVENT_CALLBACKS Callbacks
    );

#endif

#ifndef INDICIUM_NO_D3D10

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineSetD3D10EventCallbacks( _In_ PINDICIUM_ENGINE Engine, _In_ PINDICIUM_D3D10_EVENT_CALLBACKS Callbacks );
     *
     * \brief   Registers one or more Direct3D 10 render pipeline callbacks.
     *
     * \author  Benjamin H�glinger-Stelzer
     * \date    05.05.2019
     *
     * \param   Engine      The engine handle.
     * \param   Callbacks   The callback collection to register.
     *
     * \returns A VOID.
     */
    INDICIUM_API VOID IndiciumEngineSetD3D10EventCallbacks(
        _In_
        PINDICIUM_ENGINE Engine,
        _In_
        PINDICIUM_D3D10_EVENT_CALLBACKS Callbacks
    );

#endif

#ifndef INDICIUM_NO_D3D11

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineSetD3D11EventCallbacks( _In_ PINDICIUM_ENGINE Engine, _In_ PINDICIUM_D3D11_EVENT_CALLBACKS Callbacks );
     *
     * \brief   Registers one or more Direct3D 11 render pipeline callbacks.
     *
     * \author  Benjamin H�glinger-Stelzer
     * \date    05.05.2019
     *
     * \param   Engine      The engine handle.
     * \param   Callbacks   The callback collection to register.
     *
     * \returns A VOID.
     */
    INDICIUM_API VOID IndiciumEngineSetD3D11EventCallbacks(
        _In_
        PINDICIUM_ENGINE Engine,
        _In_
        PINDICIUM_D3D11_EVENT_CALLBACKS Callbacks
    );

#endif

#ifndef INDICIUM_NO_D3D12

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineSetD3D12EventCallbacks( _In_ PINDICIUM_ENGINE Engine, _In_ PINDICIUM_D3D12_EVENT_CALLBACKS Callbacks );
     *
     * \brief   Registers one or more Direct3D 12 render pipeline callbacks.
     *
     * \author  Benjamin H�glinger-Stelzer
     * \date    05.05.2019
     *
     * \param   Engine      The engine handle.
     * \param   Callbacks   The callback collection to register.
     *
     * \returns A VOID.
     */
    INDICIUM_API VOID IndiciumEngineSetD3D12EventCallbacks(
        _In_
        PINDICIUM_ENGINE Engine,
        _In_
        PINDICIUM_D3D12_EVENT_CALLBACKS Callbacks
    );

#endif

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineLogDebug( _In_ LPCSTR Format, _In_opt_ ... );
     *
     * \brief   Indicium engine log debug
     *
     * \author  Benjamin H�glinger-Stelzer
     * \date    16.05.2019
     *
     * \param   Format  Describes the format to use.
     * \param   ...     The ...
     *
     * \returns A VOID.
     */
    INDICIUM_API VOID IndiciumEngineLogDebug(
        _In_
        LPCSTR Format,
        _In_opt_
        ...
    );

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineLogInfo( _In_ LPCSTR Format, _In_opt_ ... );
     *
     * \brief   Indicium engine log information
     *
     * \author  Benjamin H�glinger-Stelzer
     * \date    16.05.2019
     *
     * \param   Format  Describes the format to use.
     * \param   ...     The ...
     *
     * \returns A VOID.
     */
    INDICIUM_API VOID IndiciumEngineLogInfo(
        _In_
        LPCSTR Format,
        _In_opt_
        ...
    );

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineLogWarning( _In_ LPCSTR Format, _In_opt_ ... );
     *
     * \brief   Indicium engine log warning
     *
     * \author  Benjamin H�glinger-Stelzer
     * \date    16.05.2019
     *
     * \param   Format  Describes the format to use.
     * \param   ...     The ...
     *
     * \returns A VOID.
     */
    INDICIUM_API VOID IndiciumEngineLogWarning(
        _In_
        LPCSTR Format,
        _In_opt_
        ...
    );

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineLogError( _In_ LPCSTR Format, _In_opt_ ... );
     *
     * \brief   Indicium engine log error
     *
     * \author  Benjamin H�glinger-Stelzer
     * \date    16.05.2019
     *
     * \param   Format  Describes the format to use.
     * \param   ...     The ...
     *
     * \returns A VOID.
     */
    INDICIUM_API VOID IndiciumEngineLogError(
        _In_
        LPCSTR Format,
        _In_opt_
        ...
    );

#ifdef __cplusplus
}
#endif

#endif // IndiciumCore_h__
