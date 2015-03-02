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

int RenderBase::calculatedXPos(int x)
{
	return (int)(((float)x / (float)xCalculator) * (float)_renderer->screenWidth());
}

int RenderBase::calculatedYPos(int y)
{
	return (int)(((float)y / (float)yCalculator) * (float)_renderer->screenHeight());
}

Renderer *RenderBase::renderer()
{
	return _renderer;
}


