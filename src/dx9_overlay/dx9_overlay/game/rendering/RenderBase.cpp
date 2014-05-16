#include "RenderBase.h"

CRenderBase::CRenderBase(CRenderer *renderer)
	: _renderer(renderer), _isMarkedForDeletion(false), _resourceChanged(false), _hasToBeInitialised(true), _firstDrawAfterReset(false)
{
}

CRenderBase::~CRenderBase()
{
}

void CRenderBase::Destroy()
{
	if(!_isMarkedForDeletion)
	{
		_isMarkedForDeletion = true;
	}	
}

void CRenderBase::ChangeResource()
{
	_resourceChanged = true;
}

CRenderer *CRenderBase::Renderer()
{
	return _renderer;
}
