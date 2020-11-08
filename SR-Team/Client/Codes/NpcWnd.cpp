#include "stdafx.h"
#include "..\Headers\NpcWnd.h"


USING(Client)

CNpcWnd::CNpcWnd(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < STAGE_NPC_END; ++i)
		m_pTextureCom[i] = nullptr;
}

CNpcWnd::CNpcWnd(const CNpcWnd & other)
	: CUIObject(other)
{
}

CNpcWnd::~CNpcWnd()
{
}

HRESULT CNpcWnd::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CNpcWnd::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CNpcWnd::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CNpcWnd::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CNpcWnd::Render_UI()
{
	if (m_eNpcID != STAGE_NPC_END)
	{
		D3DXMATRIX matTrans, matWorld;
		const D3DXIMAGE_INFO* pTexInfo = m_pTextureCom[m_eNpcID]->Get_TexInfo(0);
		if (nullptr == pTexInfo)
			return E_FAIL;

		_vec3 vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };

		D3DXMatrixTranslation(&matTrans, 950.f, 800.f, 0.f);
		matWorld = matTrans;

		m_pSprite->SetTransform(&matWorld);
		m_pSprite->Draw(
			(LPDIRECT3DTEXTURE9)m_pTextureCom[m_eNpcID]->GetTexture(0),
			nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	return S_OK;
}

HRESULT CNpcWnd::Add_Component()
{
	TCHAR szTextureNameTag[][MAX_STR] =
	{
		L"Component_Texture_NormalNpcWnd_Stage1",
		L"Component_Texture_NormalNpcWnd_Stage3"
	};
	TCHAR szTexture[MIN_STR] = L"Com_TextureWnd%d";
	TCHAR szCombine[MIN_STR] = L"";

	for (_uint i = 0; i < STAGE_NPC_END; i++)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureNameTag[i],
			szCombine, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	return S_OK;
}

CNpcWnd * CNpcWnd::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CNpcWnd* pInstance = new CNpcWnd(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CNpcWnd", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CNpcWnd::Clone_GameObject(void * _pArg)
{
	CNpcWnd* pInstance = new CNpcWnd(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CNpcWnd", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcWnd::Free()
{
	for (_uint i = 0; i < STAGE_NPC_END; ++i)
		Safe_Release(m_pTextureCom[i]);

	CUIObject::Free();
}
