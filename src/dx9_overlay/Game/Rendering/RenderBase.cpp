#include "RenderBase.h"

int RenderBase::xCalculator = 800;
int RenderBase::yCalculator = 600;

RenderBase::RenderBase(Renderer *renderer)
	: _renderer(renderer), _isMarkedForDeletion(false), _resourceChanged(false), _hasToBeInitialised(true), _firstDrawAfterReset(false)
{
}

RenderBase::~RenderBase()
{
}

void RenderBase::destroy()
{
	if(!_isMarkedForDeletion)
		_isMarkedForDeletion = true;
}

void RenderBase::setPriority(int p)
{
	_priority = p;
}

int RenderBase::priority()
{
	return _priority;
}

void RenderBase::changeResource()
{
	_resourceChanged = true;
}

int RenderBase::calculatedXPos(IDirect3DDevice9 *pDevice, int x)
{
	D3DVIEWPORT9 view;
	pDevice->GetViewport(&view);

	return (int)(((float)x / (float)xCalculator) * (float)view.Width);
}

int RenderBase::calculatedYPos(IDirect3DDevice9 *pDevice, int y)
{
	D3DVIEWPORT9 view;
	pDevice->GetViewport(&view);

	return (int)(((float)y / (float)yCalculator) * (float)view.Height);
}

Renderer *RenderBase::renderer()
{
	return _renderer;
}


