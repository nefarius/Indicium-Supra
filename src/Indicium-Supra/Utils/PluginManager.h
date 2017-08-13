#pragma once

#include <vector>
#include <map>

#include "IndiciumPlugin.h"

#define POCO_NO_UNWINDOWS
#include <Poco/SharedLibrary.h>
#include <Poco/Mutex.h>

using Poco::SharedLibrary;
using Poco::FastMutex;
using Poco::ScopedLock;

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

class PluginManager
{
	TCHAR m_DllPath[MAX_PATH];
    std::vector<SharedLibrary*> plugins;
    FastMutex mPlugins;

    std::map<std::string, std::vector<LPVOID>> _fpMap;
    std::vector<std::string> exports = { "Present", "Reset", "EndScene", "ResizeTarget" };

    bool findStringIC(const std::string& strHaystack, const std::string& strNeedle) const;
public:
	PluginManager();
	~PluginManager();

	void load();
	void unload();
	void present(IID guid, LPVOID unknown, Direct3DVersion version);
	void reset(IID guid, LPVOID unknown, Direct3DVersion version);
    void endScene(IID guid, LPVOID unknown, Direct3DVersion version);
    void resizeTarget(IID guid, LPVOID unknown, Direct3DVersion version);
};

