#include "Component.h"
#include "..\Headers\Component_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Setup_Component_Manager(_int _iSceneCount)
{
	if (1 > _iSceneCount)
	{
		PRINT_LOG(L"Under_Range To SceneCount For Component_Manager", LOG::ENGINE);
		return E_FAIL;
	}

	m_pComponents = new COMPONENTS[_iSceneCount];
	m_iSceneCount = _iSceneCount;
	return S_OK;
}

HRESULT CComponent_Manager::Add_Component_Prototype(_int _iSceneID, const wstring& _strPrototypeTag, CComponent* _pPrototype)
{
	if (0 > _iSceneID || m_iSceneCount <= _iSceneID)
		return E_FAIL;

	if (nullptr == m_pComponents || nullptr == _pPrototype)
		return E_FAIL;

	auto iter_find = m_pComponents[_iSceneID].find(_strPrototypeTag);
	if (m_pComponents[_iSceneID].end() != iter_find)
	{
		Safe_Release(_pPrototype);
		return E_FAIL;
	}

	m_pComponents[_iSceneID].emplace(_strPrototypeTag, _pPrototype);
	return S_OK;
}

CComponent * CComponent_Manager::Clone_Component(_int _iSceneID, const wstring& _strPrototypeTag, void* _pArg)
{
	if (0 > _iSceneID || m_iSceneCount <= _iSceneID)
		return nullptr;

	if (nullptr == m_pComponents)
		return nullptr;

	auto iter_find = m_pComponents[_iSceneID].find(_strPrototypeTag);
	if (m_pComponents[_iSceneID].end() == iter_find)
	{
		PRINT_LOG(_strPrototypeTag.c_str(), LOG::ENGINE);
		PRINT_LOG(L"Prototype Not Found!", LOG::ENGINE);
		return nullptr;
	}

	return iter_find->second->Clone_Component(_pArg);
}

HRESULT CComponent_Manager::Clear_ForScene(_int _iSceneID)
{
	if (0 > _iSceneID || m_iSceneCount <= _iSceneID)
		return E_FAIL;

	if (nullptr == m_pComponents)
		return E_FAIL;

	for (auto& Pair : m_pComponents[_iSceneID])
		Safe_Release(Pair.second);

	m_pComponents[_iSceneID].clear();
	
	return S_OK;
}

void CComponent_Manager::Free()
{
	for (_int iCnt = 0; iCnt < m_iSceneCount; ++iCnt)
	{
		for (auto& rPair : m_pComponents[iCnt])
			Safe_Release(rPair.second);

		m_pComponents[iCnt].clear();
	}

	Safe_Delete_Array(m_pComponents);
}
