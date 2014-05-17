#include "renderbase.h"

renderbase::renderbase(renderer *renderer)
	: _renderer(renderer), _isMarkedForDeletion(false), _resourceChanged(false), _hasToBeInitialised(true), _firstDrawAfterReset(false)
{
}

renderbase::~renderbase()
{
}

void renderbase::Destroy()
{
	if(!_isMarkedForDeletion)
	{
		_isMarkedForDeletion = true;
	}	
}

void renderbase::ChangeResource()
{
	_resourceChanged = true;
}

renderer *renderbase::Renderer()
{
	return _renderer;
}
