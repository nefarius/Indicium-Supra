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

#pragma once

#include "Windows.h"
#include <MinHook.h>
#include <Poco/Exception.h>

#include <type_traits>


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
        MH_STATUS ret;

		if ((ret = MH_CreateHookEx((PBYTE)pFunc, (PBYTE)_detour, &_trampoline)) != MH_OK)
		{
            throw Poco::ApplicationException("Couldn't create hook", ret);
		}

		if ((ret = MH_EnableHook((PBYTE)pFunc)) != MH_OK)
		{
			throw Poco::ApplicationException("Couldn't enable hook", ret);
		}

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

