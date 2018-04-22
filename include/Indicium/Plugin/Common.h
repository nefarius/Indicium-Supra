#ifndef Common_h__
#define Common_h__

enum Direct3DVersion
{
    Direct3D9 = 0,
    Direct3D9Ex = 1 << 0,
    Direct3D10 = 1 << 1,
    Direct3D11 = 1 << 2,
    Direct3D12 = 1 << 3
};

#ifdef __cplusplus

inline Direct3DVersion operator|(Direct3DVersion lhs, Direct3DVersion rhs)
{
    return static_cast<Direct3DVersion>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline Direct3DVersion operator&(Direct3DVersion lhs, Direct3DVersion rhs)
{
    return static_cast<Direct3DVersion>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

#endif

#ifndef IS_INDICIUM_SUPRA

#define INDICIUM_EXPORT extern "C" __declspec(dllexport) VOID __cdecl

INDICIUM_EXPORT indicium_plugin_init(Direct3DVersion version);

#endif

#endif // Common_h__
