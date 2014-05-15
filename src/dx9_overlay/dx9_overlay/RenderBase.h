#pragma once
#include "Renderer.h"

class CRenderBase
{
	friend class CRenderer;
public:
	CRenderBase(CRenderer *render);
	virtual ~CRenderBase(void);

	void Destroy();

protected:
	virtual void Draw(IDirect3DDevice9 *pDevice)  = 0;
	virtual void Reset(IDirect3DDevice9 *pDevice) = 0;

	virtual void Show() = 0;
	virtual void Hide() = 0;

	virtual void ReleaseResourcesForDeletion(IDirect3DDevice9 *pDevice) = 0;

	virtual bool CanBeDeleted() = 0;

	virtual bool LoadResource(IDirect3DDevice9 *pDevice) = 0;

	virtual void FirstDrawAfterReset(IDirect3DDevice9 *pDevice) = 0;

	void ChangeResource();

	CRenderer *Renderer();

private:
	bool		_hasToBeInitialised;
	bool		_isMarkedForDeletion;
	bool		_resourceChanged;
	bool		_firstDrawAfterReset;

	CRenderer *	_renderer;
};

