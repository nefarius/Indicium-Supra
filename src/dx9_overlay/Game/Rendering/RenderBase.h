#pragma once
#include "Renderer.h"

class RenderBase
{
	friend class Renderer;
public:
	RenderBase(Renderer *render);
	virtual ~RenderBase(void);

	void destroy();

	static int xCalculator;
	static int yCalculator;

protected:
	virtual void draw(IDirect3DDevice9 *pDevice)  = 0;
	virtual void reset(IDirect3DDevice9 *pDevice) = 0;

	virtual void show() = 0;
	virtual void hide() = 0;

	virtual void releaseResourcesForDeletion(IDirect3DDevice9 *pDevice) = 0;

	virtual bool canBeDeleted() = 0;

	virtual bool loadResource(IDirect3DDevice9 *pDevice) = 0;

	virtual void firstDrawAfterReset(IDirect3DDevice9 *pDevice) = 0;

	void changeResource();

	int calculatedXPos(IDirect3DDevice9 *pDevice, int x);
	int calculatedYPos(IDirect3DDevice9 *pDevice, int y);

	Renderer *renderer();

private:
	bool _hasToBeInitialised, _isMarkedForDeletion, _resourceChanged, _firstDrawAfterReset;

	Renderer *	_renderer;
};

