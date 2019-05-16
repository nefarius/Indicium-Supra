#ifndef IndiciumOpenGL2_h__
#define IndiciumOpenGL2_h__

typedef
_Function_class_(EVT_INDICIUM_OGL2_SWAP_BUFFERS)
VOID
EVT_INDICIUM_OGL2_SWAP_BUFFERS(
    HDC   hDC
);

typedef EVT_INDICIUM_OGL2_SWAP_BUFFERS *PFN_INDICIUM_OGL2_SWAP_BUFFERS;


typedef struct _INDICIUM_OGL2_EVENT_CALLBACKS
{
    PFN_INDICIUM_OGL2_SWAP_BUFFERS      EvtIndiciumOGL2PreSwapBuffers;
    PFN_INDICIUM_OGL2_SWAP_BUFFERS      EvtIndiciumOGL2PostSwapBuffers;

} INDICIUM_OGL2_EVENT_CALLBACKS, *PINDICIUM_OGL2_EVENT_CALLBACKS;

/**
 * \fn  VOID FORCEINLINE INDICIUM_OGL2_EVENT_CALLBACKS_INIT( _Out_ PINDICIUM_OGL2_EVENT_CALLBACKS Callbacks )
 *
 * \brief   The OpenGL 2.x event callback collection to initialize.
 *
 * \author  Benjamin Höglinger-Stelzer
 * \date    16.05.2019
 *
 * \param   Callbacks   The callback collection.
 *
 * \returns Nothing.
 */
VOID FORCEINLINE INDICIUM_OGL2_EVENT_CALLBACKS_INIT(
    _Out_ PINDICIUM_OGL2_EVENT_CALLBACKS Callbacks
)
{
    ZeroMemory(Callbacks, sizeof(INDICIUM_OGL2_EVENT_CALLBACKS));
}

#endif // IndiciumOpenGL2_h__
