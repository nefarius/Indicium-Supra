#include "DirectInput8.h"
#include <wbemidl.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")


//-----------------------------------------------------------------------------
// Enum each PNP device using WMI and check each device ID to see if it contains 
// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it’s an XInput device
// Unfortunately this information can not be found by just using DirectInput.
// Checking against a VID/PID of 0x028E/0x045E won't find 3rd party or future 
// XInput devices.
//
// This function stores the list of xinput devices in a linked list 
// at g_pXInputDeviceList, and IsXInputDevice() searchs that linked list
//-----------------------------------------------------------------------------
HRESULT DirectInput8Hooking::DirectInput8::SetupForIsXInputDevice()
{
	IWbemServices* pIWbemServices = nullptr;
	IEnumWbemClassObject* pEnumDevices = nullptr;
	IWbemLocator* pIWbemLocator = nullptr;
	IWbemClassObject* pDevices[20] = { 0 };
	BSTR bstrDeviceID = nullptr;
	BSTR bstrClassName = nullptr;
	BSTR bstrNamespace = nullptr;
	DWORD uReturned = 0;
	bool bCleanupCOM = false;
	UINT iDevice = 0;
	VARIANT var;
	HRESULT hr;

	// CoInit if needed
	hr = CoInitialize(nullptr);
	bCleanupCOM = SUCCEEDED(hr);

	// Create WMI
	hr = CoCreateInstance(__uuidof(WbemLocator),
		nullptr,
		CLSCTX_INPROC_SERVER,
		__uuidof(IWbemLocator),
		(LPVOID*)&pIWbemLocator);
	if (FAILED(hr) || pIWbemLocator == nullptr)
		goto LCleanup;

	// Create BSTRs for WMI
	bstrNamespace = SysAllocString(L"\\\\.\\root\\cimv2"); if (bstrNamespace == nullptr) goto LCleanup;
	bstrDeviceID = SysAllocString(L"DeviceID");           if (bstrDeviceID == nullptr)  goto LCleanup;
	bstrClassName = SysAllocString(L"Win32_PNPEntity");    if (bstrClassName == nullptr) goto LCleanup;

	// Connect to WMI 
	hr = pIWbemLocator->ConnectServer(bstrNamespace, nullptr, nullptr, 0L,
		0L, nullptr, nullptr, &pIWbemServices);
	if (FAILED(hr) || pIWbemServices == nullptr)
		goto LCleanup;

	// Switch security level to IMPERSONATE
	(void)CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, 0);

	// Get list of Win32_PNPEntity devices
	hr = pIWbemServices->CreateInstanceEnum(bstrClassName, 0, nullptr, &pEnumDevices);
	if (FAILED(hr) || pEnumDevices == nullptr)
		goto LCleanup;

	// Loop over all devices
	for (;;)
	{
		// Get 20 at a time
		hr = pEnumDevices->Next(10000, 20, pDevices, &uReturned);
		if (FAILED(hr))
			goto LCleanup;
		if (uReturned == 0)
			break;

		for (iDevice = 0; iDevice < uReturned; iDevice++)
		{
			if (!pDevices[iDevice])
				continue;

			// For each device, get its device ID
			hr = pDevices[iDevice]->Get(bstrDeviceID, 0L, &var, nullptr, nullptr);
			if (SUCCEEDED(hr) && var.vt == VT_BSTR && var.bstrVal != nullptr)
			{
				// Check if the device ID contains "IG_".  If it does, then it’s an XInput device
				// Unfortunately this information can not be found by just using DirectInput 
				if (wcsstr(var.bstrVal, L"IG_"))
				{
					// If it does, then get the VID/PID from var.bstrVal
					DWORD dwPid = 0, dwVid = 0;
					WCHAR* strVid = wcsstr(var.bstrVal, L"VID_");
					if (strVid && swscanf(strVid, L"VID_%4X", &dwVid) != 1)
						dwVid = 0;
					WCHAR* strPid = wcsstr(var.bstrVal, L"PID_");
					if (strPid && swscanf(strPid, L"PID_%4X", &dwPid) != 1)
						dwPid = 0;

					DWORD dwVidPid = MAKELONG(dwVid, dwPid);

					// Add the VID/PID to a linked list
					XINPUT_DEVICE_NODE* pNewNode = new XINPUT_DEVICE_NODE;
					if (pNewNode)
					{
						pNewNode->dwVidPid = dwVidPid;
						pNewNode->pNext = pXInputDeviceList;
						pXInputDeviceList = pNewNode;
					}
				}
			}
			SAFE_RELEASE(pDevices[iDevice]);
		}
	}

LCleanup:
	if (bstrNamespace)
		SysFreeString(bstrNamespace);
	if (bstrDeviceID)
		SysFreeString(bstrDeviceID);
	if (bstrClassName)
		SysFreeString(bstrClassName);
	for (iDevice = 0; iDevice < 20; iDevice++)
		SAFE_RELEASE(pDevices[iDevice]);
	SAFE_RELEASE(pEnumDevices);
	SAFE_RELEASE(pIWbemLocator);
	SAFE_RELEASE(pIWbemServices);

	return hr;
}

//-----------------------------------------------------------------------------
// Returns true if the DirectInput device is also an XInput device.
// Call SetupForIsXInputDevice() before, and CleanupForIsXInputDevice() after
//-----------------------------------------------------------------------------
bool DirectInput8Hooking::DirectInput8::IsXInputDevice(const GUID* pGuidProductFromDirectInput) const
{
	// Check each xinput device to see if this device's vid/pid matches
	XINPUT_DEVICE_NODE* pNode = pXInputDeviceList;
	while (pNode)
	{
		if (pNode->dwVidPid == pGuidProductFromDirectInput->Data1)
			return true;
		pNode = pNode->pNext;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Cleanup needed for IsXInputDevice()
//-----------------------------------------------------------------------------
void DirectInput8Hooking::DirectInput8::CleanupForIsXInputDevice() const
{
	// Cleanup linked list
	XINPUT_DEVICE_NODE* pNode = pXInputDeviceList;
	while (pNode)
	{
		XINPUT_DEVICE_NODE* pDelete = pNode;
		pNode = pNode->pNext;
		SAFE_DELETE(pDelete);
	}
}

BOOL DirectInput8Hooking::DirectInput8::StaticEnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext)
{
	auto pEnumContext = reinterpret_cast<DI_ENUM_CONTEXT*>(pContext);
	auto myself = reinterpret_cast<DirectInput8*>(pEnumContext->pMyself);

	return myself->EnumJoysticksCallback(pdidInstance, pContext);
}

BOOL DirectInput8Hooking::DirectInput8::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext)
{
	auto pEnumContext = reinterpret_cast<DI_ENUM_CONTEXT*>(pContext);
	HRESULT hr;

	logger.information("Joystick found");

#ifdef TEST
	if (IsXInputDevice(&pdidInstance->guidProduct))
	{
        logger.information("Current device is XInput device");
		return DIENUM_CONTINUE;
	}
#endif

#ifdef TEST
	// Skip anything other than the perferred joystick device as defined by the control panel.  
	// Instead you could store all the enumerated joysticks and let the user pick.
	if (pEnumContext->bPreferredJoyCfgValid &&
		!IsEqualGUID(pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance))
		return DIENUM_CONTINUE;
#endif

	// Obtain an interface to the enumerated joystick.
	hr = pDI->CreateDevice(pdidInstance->guidInstance, &pJoystick, nullptr);

	// If it failed, then we can't use this joystick. (Maybe the user unplugged
	// it while we were in the middle of enumerating it.)
	if (FAILED(hr))
		return DIENUM_CONTINUE;

#ifdef _M_IX86
	vtable = *reinterpret_cast<UINT32 **>(pJoystick);
#else
	vtable = *reinterpret_cast<UINT64 **>(pJoystick);
#endif

	logger.information("VTable acquired");

	// Stop enumeration. Note: we're just taking the first joystick we get. You
	// could store all the enumerated joysticks and let the user pick.
	return DIENUM_STOP;
}

DirectInput8Hooking::DirectInput8::DirectInput8() : vtable(nullptr)
{
	logger.information("Acquiring VTable for IDirectInputDevice8...");

	HRESULT hr;

	// Register with the DirectInput subsystem and get a pointer
	// to a IDirectInput interface we can use.
	// Create a DInput object
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&pDI, nullptr)))
	{
		logger.error("Couldn't create DirectInput device");
		return;
	}

	//SetupForIsXInputDevice();

	DIJOYCONFIG PreferredJoyCfg = { 0 };
	DI_ENUM_CONTEXT enumContext;
	enumContext.pMyself = this;
	enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
	enumContext.bPreferredJoyCfgValid = false;

	IDirectInputJoyConfig8* pJoyConfig = nullptr;
	if (FAILED(hr = pDI->QueryInterface(IID_IDirectInputJoyConfig8, reinterpret_cast<void**>(&pJoyConfig))))
	{
        logger.error("Couldn't query IID_IDirectInputJoyConfig8 interface");
		return;
	}

	PreferredJoyCfg.dwSize = sizeof(PreferredJoyCfg);
	if (SUCCEEDED(pJoyConfig->GetConfig(0, &PreferredJoyCfg, DIJC_GUIDINSTANCE))) // This function is expected to fail if no joystick is attached
		enumContext.bPreferredJoyCfgValid = true;
	SAFE_RELEASE(pJoyConfig);

    logger.information("Enumerating devices");

	if (FAILED(hr = pDI->EnumDevices(DI8DEVCLASS_GAMECTRL,
		StaticEnumJoysticksCallback,
		&enumContext, DIEDFL_ATTACHEDONLY)))
	{
        logger.error("Couldn't enumerate devices");
		return;
	}

	//CleanupForIsXInputDevice();

	// Make sure we got a joystick
	if (!pJoystick)
	{
		logger.warning("No joystick device found");
		return;
	}
}


DirectInput8Hooking::DirectInput8::~DirectInput8()
{
	logger.information("Releasing temporary objects");

	// Unacquire the device one last time just in case 
	// the app tried to exit while the device is still acquired.
	if (pJoystick)
		pJoystick->Unacquire();

	// Release any DirectInput objects.
	SAFE_RELEASE(pJoystick);
	SAFE_RELEASE(pDI);
}

#ifdef _M_IX86
bool DirectInput8Hooking::DirectInput8::GetVTable(UINT32 *pVTable) const
{
	if (vtable)
	{
		memcpy(pVTable, vtable, VTableElements * sizeof(UINT32));
		return true;
	}

	return false;
}
#else
bool DirectInput8Hooking::DirectInput8::GetVTable(UINT64 *pVTable) const
{
	if (vtable)
	{
		memcpy(pVTable, vtable, VTableElements * sizeof(UINT64));
		return true;
	}

	return false;
}
#endif
