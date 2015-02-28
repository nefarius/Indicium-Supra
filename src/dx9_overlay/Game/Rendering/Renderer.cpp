#include <Utils/algorithm.h>

#include "Renderer.h"
#include "RenderBase.h"

#include <boost/range/algorithm.hpp>
#include <boost/date_time.hpp>

CRenderer::RenderObjects	CRenderer::_renderObjects;
boost::mutex			CRenderer::_mtx;

int CRenderer::add(SharedRenderObject Object)
{
	boost::lock_guard<boost::mutex> l(_mtx);

	int id = 0;

	if(!_renderObjects.empty())
		for(;;id++)
			if(_renderObjects.find(id) == _renderObjects.end())
				break;
	
	_renderObjects[id] = Object;

	return id;
}

bool CRenderer::remove(int id)
{
	boost::lock_guard<boost::mutex> l(_mtx);

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

void CRenderer::draw(IDirect3DDevice9 *pDevice)
{
	boost::lock_guard<boost::mutex> l(_mtx);

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

void CRenderer::reset(IDirect3DDevice9 *pDevice)
{
	boost::lock_guard<boost::mutex> l(_mtx);

	if(_renderObjects.empty())
		return;
	
	for(auto it = _renderObjects.begin(); it != _renderObjects.end(); it ++)
	{
		it->second->reset(pDevice);
		it->second->_firstDrawAfterReset = true;
	}
}

void CRenderer::showAll()
{
	boost::lock_guard<boost::mutex> l(_mtx);

	if(_renderObjects.empty())
		return;

	for(auto it = _renderObjects.begin(); it != _renderObjects.end();it ++)
	{
		if(it->second->_isMarkedForDeletion)
			continue;

		it->second->show();
	}
}

void CRenderer::hideAll()
{
	boost::lock_guard<boost::mutex> l(_mtx);

	if(_renderObjects.empty())
		return;

	for(auto it = _renderObjects.begin(); it != _renderObjects.end();it ++)
	{
		if(it->second->_isMarkedForDeletion)
			continue;

		it->second->hide();
	}
}

void CRenderer::destroyAll()
{
	boost::lock_guard<boost::mutex> l(_mtx);

	if(_renderObjects.empty())
		return;

	for(auto it = _renderObjects.begin(); it != _renderObjects.end();it ++)
	{
		if(it->second->_isMarkedForDeletion)
			continue;

		it->second->destroy();
	}
}

int CRenderer::frameRate() const
{
	return _frameRate;
}

int CRenderer::screenWidth() const
{
	return _width;
}

int CRenderer::screenHeight() const
{
	return _height;
}

boost::mutex& CRenderer::renderMutex()
{
	return _mtx;
}
