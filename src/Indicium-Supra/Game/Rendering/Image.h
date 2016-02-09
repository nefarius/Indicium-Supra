#pragma once
#include <d3dx9.h>

#include "RenderBase.h"

class Image : public RenderBase
{
public:
	Image(Renderer *renderer, const std::string& file_path, int x, int y, float scaleX, float scaleY, int rotation, int align, bool bShow);

	void setFilePath(const std::string & path);
	void setPos(int x, int y);
	void setRotation(int rotation);
	void setAlign(int align);
	void setShown(bool show);
	void setScale(float x, float y);
	bool updateImage(const std::string& file_path, int x, int y, float scaleX, float scaleY, int rotation, int align, bool bShow);

protected:
	virtual void draw(IDirect3DDevice9 *pDevice) override sealed;
	virtual void reset(IDirect3DDevice9 *pDevice) override sealed;

	virtual void show() override sealed;
	virtual void hide() override sealed;

	virtual void releaseResourcesForDeletion(IDirect3DDevice9 *pDevice) override sealed;
	virtual bool canBeDeleted() override sealed;

	virtual bool loadResource(IDirect3DDevice9 *pDevice) override sealed;
	virtual void firstDrawAfterReset(IDirect3DDevice9 *pDevice) override sealed;

private:
	std::string			m_filePath;

	int	m_x, m_y, m_rotation, m_align;

	bool m_bShow;

	float m_scale_x, m_scale_y;

	LPDIRECT3DTEXTURE9 m_pTexture;
	LPD3DXSPRITE m_pSprite;
	D3DSURFACE_DESC m_TextureDesc;
};