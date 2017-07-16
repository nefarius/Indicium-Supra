// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the INDICIUMFW1FONTWRAPPER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// INDICIUM_EXPORT functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef INDICIUMFW1FONTWRAPPER_EXPORTS
#define INDICIUM_EXPORT extern "C" __declspec(dllexport) VOID __cdecl
#else
#define INDICIUM_EXPORT __declspec(dllimport)
#endif

