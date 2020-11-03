#include "stdafx.h"
#include "..\Headers\LoadingScreen.h"

USING(Client)

CLoadingScreen::CLoadingScreen(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	:CUIObject(_pDevice, _pSprite, _pFont)
{
}

CLoadingScreen::CLoadingScreen(const CLoadingScreen & _rOther)
	: CUIObject(_rOther)
{
}

HRESULT CLoadingScreen::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CLoadingScreen::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_uint iCnt = m_pTextureComp_Progress->Get_TextureCount();
	m_pTextureComp_Progress->SetFrameRange(0, iCnt - 1);

	return S_OK;
}

_int CLoadingScreen::Update_GameObject(_float _fDeltaTime)
{
	if (FAILED(m_pTransformComp->Update_Transform()))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTextureComp_Progress->Update_Frame(_fDeltaTime, &m_iCurFrame)))
		return GAMEOBJECT::ERR;

	return GAMEOBJECT::NOEVENT;
}

_int CLoadingScreen::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CLoadingScreen::Render_UI()
{
	const D3DXIMAGE_INFO* pTexInfo = nullptr; 
	_vec3 vCenter = {};


	m_pSprite->SetTransform(&m_pTransformComp->Get_Desc().matWorld);

	//--------------------------------------------------
	// Background
	//--------------------------------------------------
	pTexInfo = m_pTextureComp_Background->Get_TexInfo(0);
	if (nullptr == pTexInfo) return E_FAIL;

	vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };
	m_pSprite->Draw((LPDIRECT3DTEXTURE9)m_pTextureComp_Background->GetTexture(0), nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//--------------------------------------------------
	// Progress
	//--------------------------------------------------
	pTexInfo = m_pTextureComp_Progress->Get_TexInfo(m_iCurFrame);
	if (nullptr == pTexInfo) return E_FAIL;

	vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };
	m_pSprite->Draw((LPDIRECT3DTEXTURE9)m_pTextureComp_Progress->GetTexture(m_iCurFrame), nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	return S_OK;
}

HRESULT CLoadingScreen::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(CTransform::TRANSFORM_DESC));

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformComp, &tTransformDesc)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Textrue_LoadingBackground", L"Com_Texture_Background", (CComponent**)&m_pTextureComp_Background)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_LoadingProgress", L"Com_Texture_Progress", (CComponent**)&m_pTextureComp_Progress)))
		return E_FAIL;

	return S_OK;
}

CLoadingScreen * CLoadingScreen::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice || nullptr == _pSprite || nullptr == _pFont)
		return nullptr;

	CLoadingScreen* pInstance = new CLoadingScreen(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CLoadingScreen", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;		
}

CGameObject * CLoadingScreen::Clone_GameObject(void * _pArg)
{
	CLoadingScreen* pInstance = new CLoadingScreen(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CLoadingScreen", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadingScreen::Free()
{
	Safe_Release(m_pTransformComp);
	Safe_Release(m_pTextureComp_Background);
	Safe_Release(m_pTextureComp_Progress);

	CUIObject::Free();
}
