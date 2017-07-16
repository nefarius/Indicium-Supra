#pragma once

#define DIRECTINPUT_VERSION 0x0800
#define _CRT_SECURE_NO_DEPRECATE
#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#pragma warning(push)
#pragma warning(disable:6000 28251)
#include <dinput.h>
#pragma warning(pop)

#include <dinputd.h>

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=nullptr; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }

#include <Poco/Logger.h>
using Poco::Logger;

namespace DirectInput8Hooking
{
	enum DirectInputDevice8FunctionOrdinals : short
	{
		QueryInterface = 0,
		AddRef,
		Release,
		GetCapabilities,
		EnumObjects,
		GetProperty,
		SetProperty,
		Acquire,
		Unacquire,
		GetDeviceState,
		GetDeviceData,
		SetDataFormat,
		SetEventNotification,
		SetCooperativeLevel,
		GetObjectInfo,
		GetDeviceInfo,
		RunControlPanel,
		Initialize,
		CreateEffect,
		EnumEffects,
		GetEffectInfo,
		GetForceFeedbackState,
		SendForceFeedbackCommand,
		EnumCreatedEffectObjects,
		Escape,
		Poll,
		SendDeviceData,
		EnumEffectsInFile,
		WriteEffectToFile,
		BuildActionMap,
		SetActionMap,
		GetImageInfo
	};

	class DirectInput8
	{
#ifdef _M_IX86
		UINT32 *vtable;
#else
		UINT64 *vtable;
#endif

		struct DI_ENUM_CONTEXT
		{
			LPVOID pMyself;
			DIJOYCONFIG* pPreferredJoyCfg;
			bool bPreferredJoyCfgValid;
		};

		struct XINPUT_DEVICE_NODE
		{
			DWORD dwVidPid;
			XINPUT_DEVICE_NODE* pNext;
		};

		HRESULT SetupForIsXInputDevice();
		bool IsXInputDevice(const GUID* pGuidProductFromDirectInput) const;
		void CleanupForIsXInputDevice() const;
		
		static BOOL CALLBACK StaticEnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
		BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

		XINPUT_DEVICE_NODE*     pXInputDeviceList;
		LPDIRECTINPUTDEVICE8    pJoystick = nullptr;
		LPDIRECTINPUT8          pDI = nullptr;

        Logger& logger = Logger::get(typeid(this).name());
	public:
		DirectInput8();
		~DirectInput8();
		static const int VTableElements = 32;
#ifdef _M_IX86
		bool GetVTable(UINT32 *pVTable) const;
#else
		bool GetVTable(UINT64 *pVTable) const;
#endif
	};
}

