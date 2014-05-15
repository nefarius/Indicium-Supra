#pragma once

#include "windows.h"
#include <detours.h>

#include <type_traits>

enum class convention_type{
	stdcall_t, cdecl_t
};

template<convention_type tp, typename retn, typename convention, typename ...args> struct convention;

template<typename retn, typename ...args>
struct convention<convention_type::stdcall_t, retn, args...> {
	typedef retn(__stdcall *type)(args...);
};

template<typename retn, typename ...args>
struct convention<convention_type::cdecl_t, retn, args...> {
	typedef retn(__cdecl *type)(args...);
};


template<convention_type tp, typename retn, typename ...args> class Hook
{
	typedef typename convention<tp, retn, args...>::type type;

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