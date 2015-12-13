#include "Image.h"
#include "dx_utils.h"

Image::Image(Renderer *renderer, const std::string& file_path, int x, int y, int rotation, int align, bool bShow)
	: RenderBase(renderer), m_pSprite(NULL), m_pTexture(NULL)
{
	setFilePath(file_path);
	setPos(x, y);
	setRotation(rotation);
	setAlign(align);
	setShown(bShow);
}

void Image::setFilePath(const std::string & path)
{
	m_filePath = path;
}

void Image::setPos(int x, int y)
{
	m_x = x, m_y = y;
}

void Image::setRotation(int rotation)
{
	m_rotation = rotation;
}

void Image::setAlign(int align)
{
	m_align = align;
}

void Image::setShown(bool show)
{
	m_bShow = show;
}

bool Image::updateImage(const std::string& file_path, int x, int y, int rotation, int align, bool bShow)
{
	setFilePath(file_path);
	setPos(x, y);
	setRotation(rotation);
	setAlign(align);
	setShown(bShow);

	changeResource();

	return true;
}

void Image::draw(IDirect3DDevice9 *pDevice)
{
	if(!m_bShow)
		return;

	int x = calculatedXPos(m_x);
	int y = calculatedYPos(m_y);

	m_Imagepos.x = (float)x;
	m_Imagepos.y = (float)y;
	m_Imagepos.z = 0.0f;

	if(m_pTexture && m_pSprite)
	{
		D3DXMATRIX mat;
		D3DXVECTOR2 scaling(1.0f, 1.0f);
		D3DSURFACE_DESC desc;

		m_pTexture->GetLevelDesc(0, &desc);

		D3DXVECTOR2 spriteCentre;
		if (m_align == 1)
			spriteCentre = D3DXVECTOR2((FLOAT)desc.Width / 2, (FLOAT)desc.Height / 2);
		else
			spriteCentre = D3DXVECTOR2(0, 0);

		D3DXVECTOR2 trans = D3DXVECTOR2(0, 0);
		D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCentre, (FLOAT)m_rotation, &trans);
		
		//m_pSprite->SetTransform(&mat);
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pSprite->Draw(m_pTexture, nullptr, nullptr, &m_Imagepos, 0xFFFFFFFF);
		m_pSprite->End();
	}
		
	//Drawing::DrawSprite(m_pSprite, m_pTexture, x, y, m_rotation, m_align);
}

void Image::reset(IDirect3DDevice9 *pDevice)
{
	if(m_pSprite)
	{
		m_pSprite->OnLostDevice();
		m_pSprite->OnResetDevice();
	}
}


void Image::show()
{
	setShown(true);
}

void Image::hide()
{
	setShown(false);
}


void Image::releaseResourcesForDeletion(IDirect3DDevice9 *pDevice)
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

bool Image::canBeDeleted()
{
	return (m_pTexture == NULL && m_pSprite == NULL);
}

bool Image::loadResource(IDirect3DDevice9 *pDevice)
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

void Image::firstDrawAfterReset(IDirect3DDevice9 *pDevice)
{

}