#include "stdafx.h"
#include "..\Headers\RandomBox_Npc.h"


USING(Client)

CRandomBox_Npc::CRandomBox_Npc(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		m_pTextureCom[iCnt] = nullptr;
		m_pTransformCom[iCnt] = nullptr;
		m_pVIBufferCom[iCnt] = nullptr;
	}
}

CRandomBox_Npc::CRandomBox_Npc(const CRandomBox_Npc & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CRandomBox_Npc::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CRandomBox_Npc::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CRandomBox_Npc::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CRandomBox_Npc::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

HRESULT CRandomBox_Npc::Render_NoneAlpha()
{
	return S_OK;
}

HRESULT CRandomBox_Npc::Add_Component()
{
	return S_OK;
}

HRESULT CRandomBox_Npc::Add_Component_Transform()
{
	return S_OK;
}

HRESULT CRandomBox_Npc::Add_Component_Texture()
{
	return S_OK;
}

CRandomBox_Npc * CRandomBox_Npc::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CRandomBox_Npc* pInstance = new CRandomBox_Npc(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CRandomBox_Npc", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRandomBox_Npc::Clone_GameObject(void * _pArg)
{
	CRandomBox_Npc* pInstance = new CRandomBox_Npc(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CRandomBox_Npc", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRandomBox_Npc::Free()
{
	for (_uint i = 0; i < PART_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pVIBufferCom[i]);
	}

	CGameObject::Free();
}
