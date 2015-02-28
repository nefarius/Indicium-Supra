#pragma once
#include "Renderer.h"

class CRenderBase
{
	friend class CRenderer;
public:
	CRenderBase(CRenderer *render);
	virtual ~CRenderBase(void);

	void destroy();

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

	CRenderer *renderer();

private:
	bool		_hasToBeInitialised;
	bool		_isMarkedForDeletion;
	bool		_resourceChanged;
	bool		_firstDrawAfterReset;

	CRenderer *	_renderer;
};

