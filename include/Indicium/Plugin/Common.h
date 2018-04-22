#ifndef Common_h__
#define Common_h__

typedef enum _Direct3DVersion : char
{
    Direct3D9 = 0,
    Direct3D9Ex = 1 << 0,
    Direct3D10 = 1 << 1,
    Direct3D11 = 1 << 2,
    Direct3D12 = 1 << 3

} Direct3DVersion;

#ifndef IS_INDICIUM_SUPRA

#define INDICIUM_EXPORT extern "C" __declspec(dllexport) VOID __cdecl

INDICIUM_EXPORT indicium_plugin_init(Direct3DVersion version);

#endif

#endif // Common_h__
