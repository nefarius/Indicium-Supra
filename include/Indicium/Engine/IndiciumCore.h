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


#ifndef IndiciumCore_h__
#define IndiciumCore_h__


#ifdef INDICIUM_EXPORTS
#define INDICIUM_API __declspec(dllexport)
#else
#define INDICIUM_API __declspec(dllimport)
#endif

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
            const INDICIUM_D3D_VERSION GameVersion
        );

    typedef EVT_INDICIUM_GAME_HOOKED *PFN_INDICIUM_GAME_HOOKED;

    typedef
        _Function_class_(EVT_INDICIUM_GAME_UNHOOKED)
        VOID
        EVT_INDICIUM_GAME_UNHOOKED();

    typedef EVT_INDICIUM_GAME_UNHOOKED *PFN_INDICIUM_GAME_UNHOOKED;

    /**
     * \fn  INDICIUM_API PINDICIUM_ENGINE IndiciumEngineAlloc(void);
     *
     * \brief   Allocates memory for a new Indicium engine instance.
     *
     * \author  Benjamin Höglinger-Stelzer
     * \date    05.05.2019
     *
     * \returns A handle to the newly allocated engine.
     */
    INDICIUM_API PINDICIUM_ENGINE IndiciumEngineAlloc(void);

    /**
     * \fn  INDICIUM_API INDICIUM_ERROR IndiciumEngineInit( _In_ PINDICIUM_ENGINE Engine, _In_opt_ PFN_INDICIUM_GAME_HOOKED EvtIndiciumGameHooked );
     *
     * \brief   Initializes the Indicium engine. Attempts too hook into the host process's render
     *          pipeline and optionally notifies the caller once the hooks are in place.
     *
     * \author  Benjamin Höglinger-Stelzer
     * \date    05.05.2019
     *
     * \param   Engine                  The previously allocated engine handle.
     * \param   EvtIndiciumGameHooked   The optional callback invoked once the render pipeline has
     *                                  been hooked.
     *
     * \returns An INDICIUM_ERROR.
     */
    INDICIUM_API INDICIUM_ERROR IndiciumEngineInit(
        _In_
        PINDICIUM_ENGINE Engine,
        _In_opt_
        PFN_INDICIUM_GAME_HOOKED EvtIndiciumGameHooked
    );

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineShutdown( _In_ PINDICIUM_ENGINE Engine, _In_opt_ PFN_INDICIUM_GAME_UNHOOKED EvtIndiciumGameUnhooked );
     *
     * \brief   Gracefully shuts down the Indicium engine, unhooking from the render pipeline during
     *          runtime. After this call, the engine can either be initialized once again or the
     *          handle must be freed with IndiciumEngineFree.
     *
     * \author  Benjamin Höglinger-Stelzer
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
     * \author  Benjamin Höglinger-Stelzer
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

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineSetD3D9EventCallbacks( _In_ PINDICIUM_ENGINE Engine, _In_ PINDICIUM_D3D9_EVENT_CALLBACKS Callbacks );
     *
     * \brief   Registers one or more Direct3D 9(Ex) render pipeline callbacks.
     *
     * \author  Benjamin Höglinger-Stelzer
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

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineSetD3D10EventCallbacks( _In_ PINDICIUM_ENGINE Engine, _In_ PINDICIUM_D3D10_EVENT_CALLBACKS Callbacks );
     *
     * \brief   Registers one or more Direct3D 10 render pipeline callbacks.
     *
     * \author  Benjamin Höglinger-Stelzer
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

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineSetD3D11EventCallbacks( _In_ PINDICIUM_ENGINE Engine, _In_ PINDICIUM_D3D11_EVENT_CALLBACKS Callbacks );
     *
     * \brief   Registers one or more Direct3D 11 render pipeline callbacks.
     *
     * \author  Benjamin Höglinger-Stelzer
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

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineSetD3D12EventCallbacks( _In_ PINDICIUM_ENGINE Engine, _In_ PINDICIUM_D3D12_EVENT_CALLBACKS Callbacks );
     *
     * \brief   Registers one or more Direct3D 12 render pipeline callbacks.
     *
     * \author  Benjamin Höglinger-Stelzer
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

    /**
     * \fn  INDICIUM_API VOID IndiciumEngineLogDebug( _In_ LPCSTR Format, _In_opt_ ... );
     *
     * \brief   Indicium engine log debug
     *
     * \author  Benjamin Höglinger-Stelzer
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
     * \author  Benjamin Höglinger-Stelzer
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
     * \author  Benjamin Höglinger-Stelzer
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
     * \author  Benjamin Höglinger-Stelzer
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
