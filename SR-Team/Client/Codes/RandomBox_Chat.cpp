#include "stdafx.h"
#include "..\Headers\RandomBox_Chat.h"


USING(Client)

CRandomBox_Chat::CRandomBox_Chat(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
}

CRandomBox_Chat::CRandomBox_Chat(const CRandomBox_Chat & other)
	: CUIObject(other)
{
}

HRESULT CRandomBox_Chat::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CRandomBox_Chat::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return S_OK;

	return S_OK;
}

_int CRandomBox_Chat::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CRandomBox_Chat::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CRandomBox_Chat::Render_UI()
{
	return S_OK;
}

HRESULT CRandomBox_Chat::Add_Component()
{
	return S_OK;
}

CRandomBox_Chat * CRandomBox_Chat::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CRandomBox_Chat* pInstance = new CRandomBox_Chat(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CRandomBox_Chat", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRandomBox_Chat::Clone_GameObject(void * _pArg)
{
	CRandomBox_Chat* pInstance = new CRandomBox_Chat(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CRandomBox_Chat", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRandomBox_Chat::Free()
{
	CUIObject::Free();
}
