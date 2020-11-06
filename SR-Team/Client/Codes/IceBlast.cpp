#include "stdafx.h"
#include "..\Headers\IceBlast.h"

USING(Client)

CIceBlast::CIceBlast(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CIceBlast::CIceBlast(const CIceBlast & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CIceBlast::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CIceBlast::Setup_GameObject(void * _pArg)
{
	return S_OK;
}

_int CIceBlast::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (!m_bActive)
		return GAMEOBJECT::NOEVENT;

	return GAMEOBJECT::NOEVENT;
}

_int CIceBlast::LateUpdate_GameObject(_float _fDeltaTime)
{

	// renderer list에 등록 안함
	if (!m_bActive)
		GAMEOBJECT::NOEVENT;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CIceBlast::Render_NoneAlpha()
{
	return S_OK;
}

HRESULT CIceBlast::Add_Component()
{
	return S_OK;
}

CGameObject * CIceBlast::Clone_GameObject(void * _pArg)
{
	CIceBlast* pInstance = new CIceBlast(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create EnergyBolt Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBlast::Free()
{
	Safe_Release(m_pVIBufferComp);
	Safe_Release(m_pTransformComp);
	Safe_Release(m_pTextureComp);

	CGameObject::Free();
}

CIceBlast * CIceBlast::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CIceBlast* pInstance = new CIceBlast(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create EnergyBolt", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}
