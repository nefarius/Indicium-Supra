#include <Utils/algorithm.h>

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

	if(_renderObjects.empty())
		return false;

	if(_renderObjects.find(id) == _renderObjects.end())
		return false;

	auto ptr = _renderObjects[id];
	if(ptr->_isMarkedForDeletion)
		return false;

	ptr->destroy();
	return true;
}

void Renderer::draw(IDirect3DDevice9 *pDevice)
{
	std::lock_guard<std::recursive_mutex> l(_mtx);

	static DWORD dwFrames = 0;
	static boost::posix_time::ptime TimeNow;
	static boost::posix_time::ptime TimeLast = boost::posix_time::microsec_clock::local_time();
	static DWORD dwElapsedTime = 0;

	dwFrames++;
	TimeNow = boost::posix_time::microsec_clock::local_time();
	dwElapsedTime = (TimeNow - TimeLast).total_milliseconds();

	if (dwElapsedTime >= 500)
	{
		float fFPS = (((float) dwFrames) * 1000.0f) / ((float) dwElapsedTime);
		_frameRate = (int) fFPS;
		dwFrames = 0;
		TimeLast = TimeNow;
	}

	D3DVIEWPORT9 viewPort;
	pDevice->GetViewport(&viewPort);

	_width = viewPort.Width;
	_height = viewPort.Height;


	if(_renderObjects.empty())
		return;

	erase_if(_renderObjects, [&](int id, SharedRenderObject obj) -> bool 
	{
		if(obj->_isMarkedForDeletion)
		{
			obj->releaseResourcesForDeletion(pDevice);
			return obj->canBeDeleted();
		}

		return false;
	});


	for(auto it = _renderObjects.begin(); it != _renderObjects.end(); it ++)
	{
		if(it->second->_hasToBeInitialised)
		{
			if(!it->second->loadResource(pDevice))
				continue;

			it->second->_hasToBeInitialised = false;
		}

		if(it->second->_firstDrawAfterReset)
		{
			it->second->firstDrawAfterReset(pDevice);
			it->second->_firstDrawAfterReset = false;
		}

		if(it->second->_resourceChanged)
		{
			it->second->releaseResourcesForDeletion(pDevice);
			if(!it->second->loadResource(pDevice))
				continue;

			it->second->_resourceChanged = false;
		}

		it->second->draw(pDevice);
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

	for(auto it = _renderObjects.begin(); it != _renderObjects.end();it ++)
	{
		if(it->second->_isMarkedForDeletion)
			continue;

		it->second->destroy();
	}
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
