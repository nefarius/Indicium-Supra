#include "Image.h"
#include "dx_utils.h"

CImage::CImage(CRenderer *renderer, const std::string& file_path, int x, int y, int rotation, int align, bool bShow)
	: CRenderBase(renderer), m_pSprite(NULL), m_pTexture(NULL)
{
	SetFilePath(file_path);
	SetPos(x, y);
	SetRotation(rotation);
	SetAlign(align);
	SetShown(bShow);
}

void CImage::SetFilePath(const std::string & path)
{
	m_filePath = path;
}

void CImage::SetPos(int x, int y)
{
	m_x = x, m_y = y;
}

void CImage::SetRotation(int rotation)
{
	m_rotation = rotation;
}

void CImage::SetAlign(int align)
{
	m_align = align;
}

void CImage::SetShown(bool show)
{
	m_bShow = show;
}

bool CImage::UpdateImage(const std::string& file_path, int x, int y, int rotation, int align, bool bShow)
{
	SetFilePath(file_path);
	SetPos(x, y);
	SetRotation(rotation);
	SetAlign(align);
	SetShown(bShow);

	ChangeResource();

	return true;
}

void CImage::Draw(IDirect3DDevice9 *pDevice)
{
	if(!m_bShow)
		return;

	D3DVIEWPORT9 view;
	pDevice->GetViewport(&view);

	float fFactor[2] = { (float)m_x/(float)800, (float)m_y/(float)600 };

	if(m_pTexture && m_pSprite)
	{
		Drawing::DrawSprite(m_pSprite, m_pTexture, view.Width * fFactor[0], view.Height * fFactor[1], m_rotation, m_align);
	}
}

void CImage::Reset(IDirect3DDevice9 *pDevice)
{
	if(m_pSprite)
	{
		m_pSprite->OnLostDevice();
		m_pSprite->OnResetDevice();
	}
}


void CImage::Show()
{
	SetShown(true);
}

void CImage::Hide()
{
	SetShown(false);
}


void CImage::ReleaseResourcesForDeletion(IDirect3DDevice9 *pDevice)
{
	if(m_pSprite)
	{
		m_pSprite->Release();
		m_pSprite = NULL;
	}

	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

bool CImage::CanBeDeleted()
{
	return (m_pTexture == NULL && m_pSprite == NULL);
}

bool CImage::LoadResource(IDirect3DDevice9 *pDevice)
{
	if(m_pSprite)
	{
		m_pSprite->Release();
		m_pSprite = NULL;
	}

	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	D3DXCreateTextureFromFileA(pDevice, m_filePath.c_str(), &m_pTexture);
	D3DXCreateSprite(pDevice, &m_pSprite);

	return (m_pTexture != NULL && m_pSprite != NULL);
}

void CImage::FirstDrawAfterReset(IDirect3DDevice9 *pDevice)
{

}