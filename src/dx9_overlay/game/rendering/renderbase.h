#pragma once
#include "renderer.h"

class renderbase
{
	friend class renderer;
public:
	renderbase(renderer *render);
	virtual ~renderbase(void);

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

	renderer *Renderer();

private:
	bool		_hasToBeInitialised;
	bool		_isMarkedForDeletion;
	bool		_resourceChanged;
	bool		_firstDrawAfterReset;

	renderer *	_renderer;
};

