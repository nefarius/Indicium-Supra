#pragma once

#include "Windows.h"
#include <detours.h>

#include <type_traits>

enum class CCallConvention{
	stdcall_t, cdecl_t
};

template<CCallConvention cc, typename retn, typename convention, typename ...args> struct convention;

template<typename retn, typename ...args>
struct convention<CCallConvention::stdcall_t, retn, args...> {
	typedef retn(__stdcall *type)(args...);
};

template<typename retn, typename ...args>
struct convention<CCallConvention::cdecl_t, retn, args...> {
	typedef retn(__cdecl *type)(args...);
};


template<CCallConvention cc, typename retn, typename ...args> class CHook
{
	typedef typename convention<cc, retn, args...>::type type;

	type _orig;
	type _detour;

	bool _isApplied;
public:
	CHook() : _isApplied(false), _orig(0), _detour(0) { }

	template<typename T>
	CHook(T pFunc, type detour) : apply<T>(pFunc, detour) { }

	~CHook(){
		remove();
	}

	template<typename T>
	void apply(T pFunc, type detour)
	{
		_detour = detour;
		_orig = (type) DetourFunction((PBYTE) pFunc, (PBYTE) _detour);
		_isApplied = true;
	}

	bool remove()
	{
		if (!_isApplied)
			return false;

		_isApplied = false;
		return DetourRemove((PBYTE) _orig, (PBYTE) _detour) > 0;
	}

	retn callOrig(args... p)
	{
		return _orig(p...);
	}

	const bool isApplied() const {
		return _isApplied;
	}
};