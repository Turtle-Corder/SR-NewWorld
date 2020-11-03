#include "Management.h"
#include "Component.h"
#include "..\Headers\GameObject.h"

USING(Engine)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 _pDevice)
	: m_pDevice(_pDevice)
{
	Safe_AddRef(m_pDevice);
}

CGameObject::CGameObject(const CGameObject & _rOther)
	: m_pDevice(_rOther.m_pDevice)
{
	Safe_AddRef(m_pDevice);
}

void CGameObject::Free()
{
	for (auto& rPair : m_Components)
		Safe_Release(rPair.second);

	m_Components.clear();

	Safe_Release(m_pDevice);
}

_int CGameObject::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CGameObject::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

HRESULT CGameObject::Render_Priority()
{
	return E_NOTIMPL;
}

HRESULT CGameObject::Render_NoneAlpha()
{
	return E_NOTIMPL;
}

HRESULT CGameObject::Render_OnlyAlpha()
{
	return E_NOTIMPL;
}

HRESULT CGameObject::Render_BlendAlpha()
{
	return E_NOTIMPL;
}

HRESULT CGameObject::Render_UI()
{
	return E_NOTIMPL;
}

HRESULT CGameObject::Take_Damage(const CComponent* _pDamageComp)
{
	return E_NOTIMPL;
}

void CGameObject::Set_Dead()
{
	m_bDead = true;
}

_bool CGameObject::IsDead()
{
	return m_bDead;
}

CComponent * CGameObject::Get_Component(const wstring & _strComponentTag)
{
	auto iter_find = m_Components.find(_strComponentTag);
	if (m_Components.end() == iter_find)
		return nullptr;

	return iter_find->second;
}

HRESULT CGameObject::Add_Component(_int _iSceneID, const wstring & _strPrototypeTag, const wstring & _strComponentTag, CComponent ** _ppComponent, void * _pArg)
{
	auto iter_find = m_Components.find(_strComponentTag);
	if (m_Components.end() != iter_find)
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// UNDONE: need component
	CComponent* pClone = pManagement->Clone_Component(_iSceneID, _strPrototypeTag, _pArg);
	if (nullptr == pClone)
		return E_FAIL;

	m_Components.emplace(_strComponentTag, pClone);

	if (nullptr != _ppComponent)
	{
		*_ppComponent = pClone;
		Safe_AddRef(pClone);
	}

	return S_OK;
}
