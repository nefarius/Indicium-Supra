#pragma once
#include <d3dx9.h>

#include <boost/container/map.hpp>

#include <boost/thread.hpp>
#include <memory>
#include <map>

class CRenderBase;

class CRenderer
{
	typedef std::shared_ptr<CRenderBase> SharedRenderObject;
	typedef std::map<int, SharedRenderObject> RenderObjects;

public:
	int add(SharedRenderObject Object);

	bool remove(int id);

	template<typename T> std::shared_ptr<T> get(int id)
	{
		if(_renderObjects.empty())
			return nullptr;

		if(_renderObjects.find(id) == _renderObjects.end())
			return nullptr;

		if(_renderObjects[id]->_isMarkedForDeletion)
			return nullptr;

		return std::dynamic_pointer_cast<T, CRenderBase>(_renderObjects[id]);
	}

	void draw(IDirect3DDevice9 *pDevice);
	void reset(IDirect3DDevice9 *pDevice);

	void showAll();
	void hideAll();
	void destroyAll();

	int frameRate() const;

	int screenWidth() const;
	int screenHeight() const;

	boost::mutex& renderMutex();

private:
	int _frameRate;
	int _width;
	int _height;

	static RenderObjects _renderObjects;
	static boost::mutex	 _mtx;
};

