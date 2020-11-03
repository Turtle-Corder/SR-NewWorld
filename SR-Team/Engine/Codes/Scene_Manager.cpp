#include "Scene.h"
#include "..\Headers\Scene_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CScene_Manager)

CScene_Manager::CScene_Manager()
{
}

_int CScene_Manager::Get_CurrentSceneID()
{
	return m_iCurrentSceneID;
}

HRESULT CScene_Manager::Change_CurrentScene(_int _iSceneID, CScene * pCurrentScene)
{
	if (nullptr == pCurrentScene)
		return E_FAIL;

	if (m_iCurrentSceneID != _iSceneID)
	{
		Safe_Release(m_pCurrentScene);

		m_pCurrentScene = pCurrentScene;
		m_iCurrentSceneID = _iSceneID;
	}

	return S_OK;
}

_int CScene_Manager::Update_Scene_Manager(_float _fDeltaTime)
{
	if (nullptr == m_pCurrentScene)
		return -1;

	return m_pCurrentScene->Update_Scene(_fDeltaTime);
}

_int CScene_Manager::LateUpdate_Scene_Manager(_float _fDeltaTime)
{
	if (nullptr == m_pCurrentScene)
		return -1;

	return m_pCurrentScene->LateUpdate_Scene(_fDeltaTime);
}

void CScene_Manager::Free()
{
	Safe_Release(m_pCurrentScene);
}
