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

#include "PluginManager.h"
#include "Misc.h"
#include "Global.h"

#include <algorithm>

#define POCO_NO_UNWINDOWS
#include <Poco/Logger.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/Glob.h>

using Poco::Logger;
using Poco::Path;
using Poco::File;
using Poco::Glob;


PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
}

void PluginManager::unload()
{
    auto& logger = Logger::get(LOG_REGION());

    logger.information("Unloading plugins...");

    ScopedLock<FastMutex> lock(_plugins_lock);

    //
    // TODO: implement!
    // 

    /*
    // discard function pointers
    for (const auto& symbol : exports)
    {
        _fpMap[symbol].clear();
    }

    // unload libraries
    for (auto it = plugins.begin(); it != plugins.end();)
    {
        if ((*it)->isLoaded())
            (*it)->unload();
        delete *it;
        it = plugins.erase(it);
    }
    */

    logger.information("Finished unloading plugins");
}

void PluginManager::load(Direct3DVersion version)
{
    //
    // Fetch libraries with names ending in ".Plugin.dll"
    // 
    std::set<std::string> files;
    const Path pluginDir(GlobalState::instance().rootPath(), "*.Plugin.dll");
    Glob::glob(pluginDir, files);

    std::set<std::string>::iterator it = files.begin();
    for (; it != files.end(); ++it)
    {
        auto plugin = new Plugin;

        //
        // Load library
        // 
        plugin->plugin_module = new SharedLibrary;
        plugin->plugin_module->load(*it);

        //
        // Grab initialization function
        // 
        plugin->init = static_cast<fp_init>(plugin->plugin_module->getSymbol("indicium_plugin_init"));

        //
        // Initialize plugin library with D3D version
        // 
        const auto compatible = plugin->init(version);

        if (!compatible)
        {
            plugin->plugin_module->unload();
            delete plugin->plugin_module;
            continue;
        }

        //
        // Different versions require different function pointers
        // 
        switch (version)
        {
        case Direct3D9:
            plugin->function_ptrs.d3d9.present = static_cast<fp_d3d9_present>(plugin
                ->plugin_module->getSymbol(
                    "indicium_plugin_d3d9_present"));
            plugin->function_ptrs.d3d9.reset = static_cast<fp_d3d9_reset>(plugin
                ->plugin_module->getSymbol(
                    "indicium_plugin_d3d9_reset"));
            plugin->function_ptrs.d3d9.endscene = static_cast<fp_d3d9_endscene>(plugin
                ->plugin_module->getSymbol(
                    "indicium_plugin_d3d9_endscene"));
            plugin->function_ptrs.d3d9.presentex = static_cast<fp_d3d9_presentex>(plugin
                ->plugin_module->getSymbol(
                    "indicium_plugin_d3d9_presentex"));
            plugin->function_ptrs.d3d9.resetex = static_cast<fp_d3d9_resetex>(plugin
                ->plugin_module->getSymbol(
                    "indicium_plugin_d3d9_resetex"));
            break;
        case Direct3D10:
            plugin->function_ptrs.dxgi.present = static_cast<fp_dxgi_present>(plugin
                ->plugin_module->getSymbol(
                    "indicium_plugin_d3d10_present"));
            plugin->function_ptrs.dxgi.resizetarget = static_cast<fp_dxgi_resizetarget>(plugin
                ->plugin_module->getSymbol(
                    "indicium_plugin_d3d10_resizetarget"));
            break;
        case Direct3D11:
            plugin->function_ptrs.dxgi.present = static_cast<fp_dxgi_present>(plugin
                ->plugin_module->getSymbol(
                    "indicium_plugin_d3d11_present"));
            plugin->function_ptrs.dxgi.resizetarget = static_cast<fp_dxgi_resizetarget>(plugin
                ->plugin_module->getSymbol(
                    "indicium_plugin_d3d11_resizetarget"));
            break;
        case Direct3D12:
            plugin->function_ptrs.dxgi.present = static_cast<fp_dxgi_present>(plugin
                ->plugin_module->getSymbol(
                    "indicium_plugin_d3d12_present"));
            plugin->function_ptrs.dxgi.resizetarget = static_cast<fp_dxgi_resizetarget>(plugin
                ->plugin_module->getSymbol(
                    "indicium_plugin_d3d12_resizetarget"));
            break;
        }

        _plugins.emplace_back(plugin);
    }
}

void PluginManager::d3d9_present(LPDIRECT3DDEVICE9 pDevice, const RECT* pSourceRect, const RECT* pDestRect,
    HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
    for (auto& plugin : _plugins)
    {
        plugin->function_ptrs.d3d9.present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    }
}

void PluginManager::d3d9_reset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    for (auto& plugin : _plugins)
    {
        plugin->function_ptrs.d3d9.reset(pDevice, pPresentationParameters);
    }
}

void PluginManager::d3d9_endscene(LPDIRECT3DDEVICE9 pDevice)
{
    for (auto& plugin : _plugins)
    {
        plugin->function_ptrs.d3d9.endscene(pDevice);
    }
}

void PluginManager::d3d9_presentex(LPDIRECT3DDEVICE9EX pDevice, const RECT* pSourceRect, const RECT* pDestRect,
    HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags)
{
    for (auto& plugin : _plugins)
    {
        plugin->function_ptrs.d3d9.presentex(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
    }
}

void PluginManager::d3d9_resetex(LPDIRECT3DDEVICE9EX pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters,
    D3DDISPLAYMODEEX* pFullscreenDisplayMode)
{
    for (auto& plugin : _plugins)
    {
        plugin->function_ptrs.d3d9.resetex(pDevice, pPresentationParameters, pFullscreenDisplayMode);
    }
}

void PluginManager::d3d10_present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    for (auto& plugin : _plugins)
    {
        plugin->function_ptrs.dxgi.present(pSwapChain, SyncInterval, Flags);
    }
}

void PluginManager::d3d10_resizetarget(IDXGISwapChain* pSwapChain, const DXGI_MODE_DESC* pNewTargetParameters)
{
    for (auto& plugin : _plugins)
    {
        plugin->function_ptrs.dxgi.resizetarget(pSwapChain, pNewTargetParameters);
    }
}

void PluginManager::d3d11_present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    for (auto& plugin : _plugins)
    {
        plugin->function_ptrs.dxgi.present(pSwapChain, SyncInterval, Flags);
    }
}

void PluginManager::d3d11_resizetarget(IDXGISwapChain* pSwapChain, const DXGI_MODE_DESC* pNewTargetParameters)
{
    for (auto& plugin : _plugins)
    {
        plugin->function_ptrs.dxgi.resizetarget(pSwapChain, pNewTargetParameters);
    }
}

void PluginManager::d3d12_present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    for (auto& plugin : _plugins)
    {
        plugin->function_ptrs.dxgi.present(pSwapChain, SyncInterval, Flags);
    }
}

void PluginManager::d3d12_resizetarget(IDXGISwapChain* pSwapChain, const DXGI_MODE_DESC* pNewTargetParameters)
{
    for (auto& plugin : _plugins)
    {
        plugin->function_ptrs.dxgi.resizetarget(pSwapChain, pNewTargetParameters);
    }
}
