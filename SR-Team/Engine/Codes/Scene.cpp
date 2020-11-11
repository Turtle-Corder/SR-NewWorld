#include "..\Headers\Scene.h"

USING(Engine)

CScene::CScene(LPDIRECT3DDEVICE9 _pDevice)
	:m_pDevice(_pDevice)
{
	Safe_AddRef(m_pDevice);
}

void CScene::Free(void)
{
	Safe_Release(m_pDevice);
}

HRESULT CScene::Setup_Scene()
{
	return S_OK;
}

_int CScene::Update_Scene(_float _fDeltaTime)
{
	return 0;
}

_int CScene::LateUpdate_Scene(_float _fDeltaTime)
{
	return 0;
}

HRESULT CScene::Set_SceneEvent(_int _iEventNo)
{
	return S_OK;
}
