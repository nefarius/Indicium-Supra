#pragma once

#include "Windows.h"
#include <MinHook.h>

#include <type_traits>

#include <Poco/Logger.h>
using Poco::Logger;

enum class CallConvention
{
	stdcall_t,
	cdecl_t
};

template <CallConvention cc, typename retn, typename convention, typename ...args>
struct convention;

template <typename retn, typename ...args>
struct convention<CallConvention::stdcall_t, retn, args...>
{
	typedef retn (__stdcall *type)(args ...);
};

template <typename retn, typename ...args>
struct convention<CallConvention::cdecl_t, retn, args...>
{
	typedef retn (__cdecl *type)(args ...);
};

template <typename T>
inline MH_STATUS MH_CreateHookEx(LPVOID pTarget, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHook(pTarget, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

template <CallConvention cc, typename retn, typename ...args>
class Hook
{
	typedef typename convention<cc, retn, args...>::type type;

	size_t _orig;
	type _trampoline;
	type _detour;

	bool _isApplied;

    Logger& logger = Logger::get(typeid(this).name());
public:
	Hook() : _orig(0), _trampoline(0), _detour(0), _isApplied(false)
	{
	}

	template <typename T>
	Hook(T pFunc, type detour) : _orig(0), _isApplied(false), apply<T>(pFunc, detour)
	{
	}

	~Hook()
	{
		remove();
	}

	template <typename T>
	void apply(T pFunc, type detour)
	{
		_detour = detour;
		_orig = static_cast<size_t>(pFunc);

		if (MH_CreateHookEx((PBYTE)pFunc, (PBYTE)_detour, &_trampoline) != MH_OK)
		{
			logger.fatal("Couldn't create hook");
			return;
		}

		if (MH_EnableHook((PBYTE)pFunc) != MH_OK)
		{
			logger.fatal("Couldn't enable hook");
			return;
		}

		logger.information("Hook created and enabled");

		_isApplied = true;
	}

	bool remove()
	{
		if (!_isApplied)
			return false;

		_isApplied = false;
		return MH_DisableHook((PBYTE)_orig) == MH_OK;
	}

	retn callOrig(args ... p)
	{
		return _trampoline(p...);
	}

	bool isApplied() const
	{
		return _isApplied;
	}
};

