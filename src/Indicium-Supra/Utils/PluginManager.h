#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <mutex>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

class PluginManager
{
	TCHAR m_DllPath[MAX_PATH];
	std::vector<std::string> m_pluginPaths;
	std::vector<HMODULE> m_pluginMods;
	std::vector<LPVOID> m_presentFuncs;
	std::vector<LPVOID> m_resetFuncs;
	std::mutex m_mutex;

	bool findStringIC(const std::string& strHaystack, const std::string& strNeedle) const;
public:
	PluginManager();
	~PluginManager();

	void refresh();
	void load();
	void unload();
	void present(IID guid, LPVOID unknown);
	void reset(IID guid, LPVOID unknown);
};

