#pragma once

#include "Windows.h"

#include <type_traits>

enum class CallConvention{
	stdcall_t, cdecl_t
};

template<CallConvention cc, typename retn, typename convention, typename ...args> struct convention;

template<typename retn, typename ...args>
struct convention<CallConvention::stdcall_t, retn, args...> {
	typedef retn(__stdcall *type)(args...);
};

template<typename retn, typename ...args>
struct convention<CallConvention::cdecl_t, retn, args...> {
	typedef retn(__cdecl *type)(args...);
};


template<CallConvention cc, typename retn, typename ...args> class Hook
{
	typedef typename convention<cc, retn, args...>::type type;

	type _orig;
	type _detour;

	bool _isApplied;
public:
	Hook() : _isApplied(false), _orig(0), _detour(0) { }

	template<typename T>
	Hook(T pFunc, type detour) : apply<T>(pFunc, detour) { }

	~Hook(){
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