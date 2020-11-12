#include "stdafx.h"
#include "..\Headers\Title.h"

USING(Client)

CTitle::CTitle(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	:CUIObject(_pDevice, _pSprite, _pFont)
{
}

CTitle::CTitle(const CTitle & other)
	: CUIObject(other)
{
}

HRESULT CTitle::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CTitle::Setup_GameObject(void * _pArg)
{
	m_bEnding = true;
	if (_pArg)
		m_bEnding = false;

	if (FAILED(Add_Component()))
		return E_FAIL;


	return S_OK;
}

_int CTitle::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CTitle::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CTitle::Render_UI()
{
	const D3DXIMAGE_INFO* pTexInfo = m_pTexture->Get_TexInfo(0);
	_vec3 vCenter = { (_float)(WINCX >> 1), (_float)(WINCY >> 1), 0.f };

	_matrix matWorld;
	D3DXMatrixTranslation(&matWorld, (_float)(WINCX >> 1), (_float)(WINCY >> 1), 0.f);

	m_pSprite->SetTransform(&matWorld);
	m_pSprite->Draw((LPDIRECT3DTEXTURE9)m_pTexture->GetTexture(0), nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	return S_OK;
}

HRESULT CTitle::Add_Component()
{

	if (m_bEnding)
	{
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Title", L"Com_Texture", (CComponent**)&m_pTexture)))
			return E_FAIL;
	}
	else if (!m_bEnding)
	{
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Ending", L"Com_Texture", (CComponent**)&m_pTexture)))
			return E_FAIL;
	}
	return S_OK;
}

CTitle * CTitle::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CTitle* pInstance = new CTitle(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTitle::Clone_GameObject(void * _pArg)
{
	CTitle* pInstance = new CTitle(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTitle::Free()
{
	Safe_Release(m_pTexture);

	CUIObject::Free();
}
