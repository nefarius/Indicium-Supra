#pragma once
#include <d3dx9.h>

#include "renderbase.h"

class image : public renderbase
{
public:
	static void DrawSprite(LPD3DXSPRITE SpriteInterface, LPDIRECT3DTEXTURE9 TextureInterface, int PosX, int PosY, int Rotation, int Align);

	image(renderer *renderer, const std::string& file_path, int x, int y, int rotation, int align, bool bShow);

	void SetFilePath(const std::string & path);
	void SetPos(int x, int y);
	void SetRotation(int rotation);
	void SetAlign(int align);
	void SetShown(bool show);
	bool UpdateImage(const std::string& file_path, int x, int y, int rotation, int align, bool bShow);

protected:
	virtual void Draw(IDirect3DDevice9 *pDevice) sealed;
	virtual void Reset(IDirect3DDevice9 *pDevice) sealed;

	virtual void Show() sealed;
	virtual void Hide() sealed;

	virtual void ReleaseResourcesForDeletion(IDirect3DDevice9 *pDevice) sealed;
	virtual bool CanBeDeleted() sealed;

	virtual bool LoadResource(IDirect3DDevice9 *pDevice) override sealed;
	virtual void FirstDrawAfterReset(IDirect3DDevice9 *pDevice) override sealed;

private:
	std::string			m_filePath;

	int					m_x, m_y,
						m_rotation,
						m_align;

	bool				m_bShow;

	LPDIRECT3DTEXTURE9	m_pTexture;
	LPD3DXSPRITE		m_pSprite;
};