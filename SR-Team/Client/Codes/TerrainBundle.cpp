#include "stdafx.h"
#include "..\Headers\TerrainBundle.h"

USING(Client)

CTerrainBundle::CTerrainBundle(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CTerrainBundle::CTerrainBundle(const CTerrainBundle & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CTerrainBundle::Setup_GameObject_Prototype()
{
	return S_OK;
}



HRESULT CTerrainBundle::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	{
		int VecSize = (*(int*)(_pArg));

		for (size_t i = 0; i < 16; i++)
		{
			m_TerrianList[i].resize(VecSize);
		}

	}

	if (FAILED(Add_Component()))
	{
		PRINT_LOG(_T("CubeTerrain Setup Failed"), SCENE_STAGE0);
		return E_FAIL;
	}


	return S_OK;
}

_int CTerrainBundle::Update_GameObject(_float _fDeltaTime)
{


	return GAMEOBJECT::NOEVENT;
}

_int CTerrainBundle::LateUpdate_GameObject(_float _fDeltaTime)
{
	

	return GAMEOBJECT::NOEVENT;
}

HRESULT CTerrainBundle::Render_NoneAlpha()
{
	
	return S_OK;
}

TERRAININFO CTerrainBundle::Get_TerrainInfo(_uint iIndex, _uint iFloor)
{
	return m_TerrianList[iFloor][iIndex];
}

TERRAININFO CTerrainBundle::Get_TerrainInfo(_vec3 iObjectPos, _uint iFloor)
{
	int iIndex = (int)(iObjectPos.x / (float)(m_TerrianList[0][0].iInterval) + (iObjectPos.z / (float)(m_TerrianList[0][0].iInterval)) * m_TerrianList[0][0].iMaxX);

	return m_TerrianList[iFloor][iIndex];
}

HRESULT CTerrainBundle::Set_TerrainInfo(_uint iIndex, _uint iFloor, const TERRAININFO & Input)
{

	//-----------------------------------------------------------
	//실패조건
	//-----------------------------------------------------------
	if (iFloor > 15)
		return E_FAIL;

	if (iIndex > m_TerrianList[iFloor].size())
		return E_FAIL;

	//-----------------------------------------------------------
	//성공시 Input
	//-----------------------------------------------------------
	m_TerrianList[iFloor][iIndex] = Input;


	return S_OK;
}


HRESULT CTerrainBundle::Add_Component()
{

	return S_OK;
}

CTerrainBundle * CTerrainBundle::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;


	CTerrainBundle* pInstance = new CTerrainBundle(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMonster", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrainBundle::Clone_GameObject(void * _pArg)
{
	CTerrainBundle* pInstance = new CTerrainBundle(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMonster", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrainBundle::Free()
{
	CGameObject::Free();
}
