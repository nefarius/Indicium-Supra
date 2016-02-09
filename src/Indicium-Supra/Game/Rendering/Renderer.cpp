#include <Utils/algorithm.h>
#include <algorithm>

#include "Renderer.h"
#include "RenderBase.h"

#include <boost/range/algorithm.hpp>
#include <boost/date_time.hpp>

Renderer::RenderObjects	Renderer::_renderObjects;
std::recursive_mutex Renderer::_mtx;

int Renderer::add(SharedRenderObject Object)
{
	std::lock_guard<std::recursive_mutex> l(_mtx);

	int id = 0;

	if(!_renderObjects.empty())
		for(;;id++)
			if(_renderObjects.find(id) == _renderObjects.end())
				break;
	
	_renderObjects[id] = Object;

	return id;
}

bool Renderer::remove(int id)
{
	std::lock_guard<std::recursive_mutex> l(_mtx);

	auto ptr = get(id);
	if (!ptr)
		return false;

	return ptr->_isMarkedForDeletion = true;
}

std::shared_ptr<RenderBase> Renderer::get(int id)
{
	if (_renderObjects.empty())
		return nullptr;

	if (_renderObjects.find(id) == _renderObjects.end())
		return nullptr;

	if (_renderObjects[id]->_isMarkedForDeletion)
		return nullptr;

	return _renderObjects[id];
}

void Renderer::draw(IDirect3DDevice9 *pDevice)
{
	std::lock_guard<std::recursive_mutex> l(_mtx);

	// Read frame rate
	{
		static DWORD dwFrames = 0;
		static boost::posix_time::ptime TimeNow;
		static boost::posix_time::ptime TimeLast = boost::posix_time::microsec_clock::local_time();
		static DWORD dwElapsedTime = 0;
	
		dwFrames++;
		TimeNow = boost::posix_time::microsec_clock::local_time();
		dwElapsedTime = (TimeNow - TimeLast).total_milliseconds() & 0xFFFFFFFF;
	
		if (dwElapsedTime >= 500)
		{
			float fFPS = (((float) dwFrames) * 1000.0f) / ((float) dwElapsedTime);
			_frameRate = (int) fFPS;
			dwFrames = 0;
			TimeLast = TimeNow;
		}
	}

	// Get frame's screen bounds
	{
		D3DVIEWPORT9 viewPort;
		pDevice->GetViewport(&viewPort);

		_width = viewPort.Width;
		_height = viewPort.Height;
	}

	if(_renderObjects.empty())
		return;

	// Delete all objects from the map which are marked for deletion
	erase_if(_renderObjects, [&](int id, SharedRenderObject obj) -> bool 
	{
		if(obj->_isMarkedForDeletion)
		{
			obj->releaseResourcesForDeletion(pDevice);
			return obj->canBeDeleted();
		}

		return false;
	});


	// Push all render objects in a vector which will be sorted later
	std::vector<SharedRenderObject> sortedObjects;
	for (auto it = _renderObjects.begin(); it != _renderObjects.end(); it++)
		sortedObjects.push_back(it->second);

	// Sort render objects by priority
	std::sort(sortedObjects.begin(), sortedObjects.end(), [](SharedRenderObject& i, SharedRenderObject& j){
		return i->priority() < j->priority();
	});

	// Process sorted render objects
	for (auto& i : sortedObjects)
	{
		if(i->_hasToBeInitialised)
		{
			if(!i->loadResource(pDevice))
				continue;

			i->_hasToBeInitialised = false;
		}

		if(i->_firstDrawAfterReset)
		{
			i->firstDrawAfterReset(pDevice);
			i->_firstDrawAfterReset = false;
		}

		if(i->_resourceChanged)
		{
			i->releaseResourcesForDeletion(pDevice);
			if(!i->loadResource(pDevice))
				continue;

			i->_resourceChanged = false;
		}

		i->draw(pDevice);
	}
}

void Renderer::reset(IDirect3DDevice9 *pDevice)
{
	std::lock_guard<std::recursive_mutex> l(_mtx);

	if(_renderObjects.empty())
		return;
	
	for(auto it = _renderObjects.begin(); it != _renderObjects.end(); it ++)
	{
		it->second->reset(pDevice);
		it->second->_firstDrawAfterReset = true;
	}
}

void Renderer::showAll()
{
	std::lock_guard<std::recursive_mutex> l(_mtx);

	if(_renderObjects.empty())
		return;

	for(auto it = _renderObjects.begin(); it != _renderObjects.end();it ++)
	{
		if(it->second->_isMarkedForDeletion)
			continue;

		it->second->show();
	}
}

void Renderer::hideAll()
{
	std::lock_guard<std::recursive_mutex> l(_mtx);

	if(_renderObjects.empty())
		return;

	for(auto it = _renderObjects.begin(); it != _renderObjects.end();it ++)
	{
		if(it->second->_isMarkedForDeletion)
			continue;

		it->second->hide();
	}
}

void Renderer::destroyAll()
{
	std::lock_guard<std::recursive_mutex> l(_mtx);

	if(_renderObjects.empty())
		return;

	for(auto it = _renderObjects.begin(); it != _renderObjects.end(); it ++)
		it->second->_isMarkedForDeletion = true;
}

int Renderer::frameRate() const
{
	return _frameRate;
}

int Renderer::screenWidth() const
{
	return _width;
}

int Renderer::screenHeight() const
{
	return _height;
}

std::recursive_mutex& Renderer::renderMutex()
{
	return _mtx;
}
