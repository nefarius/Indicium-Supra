#include "PluginManager.h"
#include <Shlwapi.h>
#include <algorithm>

#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

namespace fs = ::boost::filesystem;


bool PluginManager::findStringIC(const std::string& strHaystack, const std::string& strNeedle) const
{
	auto it = std::search(
		strHaystack.begin(), strHaystack.end(),
		strNeedle.begin(), strNeedle.end(),
		[](char ch1, char ch2)
		{
			return ::toupper(ch1) == ::toupper(ch2);
		}
	);
	return (it != strHaystack.end());
}

PluginManager::PluginManager()
{
	GetModuleFileName(reinterpret_cast<HINSTANCE>(&__ImageBase), m_DllPath, _countof(m_DllPath));
	PathRemoveFileSpec(m_DllPath);
}


PluginManager::~PluginManager()
{
}

void PluginManager::refresh()
{
	m_pluginPaths.clear();

	fs::path dllRoot(m_DllPath);
	fs::recursive_directory_iterator it(dllRoot);
	fs::recursive_directory_iterator endit;

	while (it != endit)
	{
		if (fs::is_regular_file(*it) && findStringIC(it->path().filename().string(), ".Plugin.dll"))
		{
			m_pluginPaths.push_back(it->path().string());
		}

		++it;
	}
}

void PluginManager::load()
{
	this->refresh();

	m_mutex.lock();

	for (const auto& path : m_pluginPaths)
	{
		auto hMod = LoadLibrary(path.c_str());

		if (hMod == nullptr)
		{
			BOOST_LOG_TRIVIAL(error) << "Couldn't load Indicium plugin from file " << path;
			continue;
		}

		auto present = static_cast<LPVOID>(GetProcAddress(hMod, "Present"));

		if (present == nullptr)
		{
			BOOST_LOG_TRIVIAL(error) << "Couldn't locate !Present(...) in " << path;
			FreeLibrary(hMod);
			continue;
		}

		m_pluginMods.push_back(hMod);
		m_presentFuncs.push_back(present);
	}

	m_mutex.unlock();
}

void PluginManager::unload()
{
	m_mutex.lock();

	// TODO: implement!

	m_mutex.unlock();
}

void PluginManager::present(IID guid, LPVOID unknown)
{
	m_mutex.try_lock();

	for (const auto& func : m_presentFuncs)
	{
		static_cast<VOID(__cdecl*)(IID, LPVOID)>(func)(guid, unknown);
	}

	m_mutex.unlock();
}

void PluginManager::reset(IID guid, LPVOID unknown)
{
	m_mutex.try_lock();

	for (const auto& func : m_resetFuncs)
	{
		static_cast<VOID(__cdecl*)(IID, LPVOID)>(func)(guid, unknown);
	}

	m_mutex.unlock();
}

