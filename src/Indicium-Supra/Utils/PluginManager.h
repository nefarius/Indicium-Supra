#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <vector>

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
    
    bool findStringIC(const std::string& strHaystack, const std::string& strNeedle) const;
public:
	PluginManager();
	~PluginManager();

	void load();
	void unload();
	void present(IID guid, LPVOID unknown);
	void reset(IID guid, LPVOID unknown);
    void endScene(IID guid, LPVOID unknown);
    void resizeTarget(IID guid, LPVOID unknown);
};

