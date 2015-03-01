#pragma once
#include <d3dx9.h>

#include <memory>
#include <map>
#include <functional>
#include <mutex>

class RenderBase;

class Renderer
{
	typedef std::shared_ptr<RenderBase> SharedRenderObject;
	typedef std::map<int, SharedRenderObject> RenderObjects;

public:
	int add(SharedRenderObject Object);

	bool remove(int id);

	template<typename T> 
	std::shared_ptr<T> getAs(int id)
	{
		if(_renderObjects.empty())
			return nullptr;

		if(_renderObjects.find(id) == _renderObjects.end())
			return nullptr;

		if(_renderObjects[id]->_isMarkedForDeletion)
			return nullptr;

		return std::dynamic_pointer_cast<T, RenderBase>(_renderObjects[id]);
	}

	std::shared_ptr<RenderBase> get(int id);

	template<typename T> 
	void execute(int id, std::function<void(std::shared_ptr<T>)> successor = nullptr, std::function<void()> error = nullptr)
	{
		auto obj = getAs<T>(id);
		if (obj && successor)
			return successor(obj);
		
		if (error)
			return error();
	}

	void draw(IDirect3DDevice9 *pDevice);
	void reset(IDirect3DDevice9 *pDevice);

	void showAll();
	void hideAll();
	void destroyAll();

	int frameRate() const;

	int screenWidth() const;
	int screenHeight() const;

	std::recursive_mutex& renderMutex();

private:
	int _frameRate, _width, _height;

	static RenderObjects _renderObjects;
	static std::recursive_mutex _mtx;
};

