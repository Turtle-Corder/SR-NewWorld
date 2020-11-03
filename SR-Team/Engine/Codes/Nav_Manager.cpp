#include "NavAgent.h"
#include "..\Headers\Nav_Manager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CNav_Manager)

CNav_Manager::CNav_Manager()
{
}

HRESULT CNav_Manager::Setup_NavigationManager()
{
	return S_OK;
}

HRESULT CNav_Manager::Set_TileInfo(TILEINFO * _pTileInfo, _int _iWidth, _int _iHeight)
{
	if (nullptr == _pTileInfo)
	{
		PRINT_LOG(L"Failed To Set_TileInfo Info is Null", LOG::ENGINE);
		return E_FAIL;
	}

	if (0 >= _iWidth || 0 >= _iHeight)
	{
		PRINT_LOG(L"Failed To Set_TileInfo Out of Range", LOG::ENGINE);
		return E_FAIL;
	}

	Safe_Delete(m_pTileInfo);
	m_pTileInfo = _pTileInfo;
	m_iWidth = _iWidth;
	m_iHeight = _iHeight;

	return S_OK;
}

HRESULT CNav_Manager::PathFind(CNavAgent* _pAgent, _int _iStartX, _int _iStartZ, _int _iGoalX, _int _iGoalZ)
{
	if (nullptr == _pAgent)
		return E_FAIL;

	if (0 > _iStartX || 0 > _iStartZ ||
		m_iWidth <= _iGoalX || m_iHeight <= _iGoalZ)
		return E_FAIL;

	Clear_FindInfo();

	m_pAgent = _pAgent;
	m_iStartX = _iStartX;
	m_iStartZ = _iStartZ;
	m_iGoalX = _iGoalX;
	m_iGoalZ = _iGoalZ;

	//--------------------------------------------------
	// 처음 하나 만들어서 넣어준다.
	//--------------------------------------------------
	NAV_NODE* pNewNode = &m_arryNodes[m_iCurFindCount++];
	pNewNode->pParent = nullptr;
	pNewNode->iX = _iStartX;
	pNewNode->iZ= _iStartZ;
	pNewNode->iG = 0;
	pNewNode->iH = abs(_iGoalX - _iStartX) + abs(_iGoalZ - _iStartZ);
	pNewNode->iF = pNewNode->iG + pNewNode->iH;

	m_OpenList.push_back(pNewNode);


	//--------------------------------------------------
	// 탐색 수행
	//--------------------------------------------------
	while (true)
	{
		if (FindProcess())
			break;
	}

	LerpProcess();

	_pAgent->Set_NavDone();
	return S_OK;
}

// S_OK		: 수행 끝남
// E_FAIL	: 수행 계속
HRESULT CNav_Manager::FindProcess()
{
	//--------------------------------------------------
	// 최대 탐색 가능한 기회를 넘어서면 실패로 처리
	//  -> BestList는 비어있으므로 어차피 던져줄 것 없음
	//--------------------------------------------------
	if (m_iMaxFindCount <= m_iCurFindCount)
		return S_OK;


	//--------------------------------------------------
	// open list 앞에서 하나 뽑는다.
	//--------------------------------------------------
	if (0 >= (int)m_OpenList.size())
		return S_OK;

	NAV_NODE* pOldNode = m_OpenList.front();
	m_OpenList.pop_front();
	m_CloseList.push_back(pOldNode);

	//--------------------------------------------------
	// 길이 없거나 || 목적지를 찾았거나
	//--------------------------------------------------
	if (!pOldNode || !pOldNode->iH)
	{
		while (nullptr != pOldNode)
		{
			m_BestList.push_front(pOldNode);
			pOldNode = pOldNode->pParent;
		}

		return S_OK;
	}

	CreateExpandNode(pOldNode);
	return E_FAIL;
}

void CNav_Manager::CreateExpandNode(NAV_NODE * _pParent)
{
	int iX = _pParent->iX;
	int iY = _pParent->iZ;

	// ↖
	if (ValidateExpand(iX - 1, iY - 1))
	{
		CreateNode(_pParent, iX - 1, iY - 1, true);
	}

	// ↗
	if (ValidateExpand(iX + 1, iY - 1))
	{
		CreateNode(_pParent, iX + 1, iY - 1, true);
	}

	// ↙
	if (ValidateExpand(iX - 1, iY + 1))
	{
		CreateNode(_pParent, iX - 1, iY + 1, true);
	}

	// ↘
	if (ValidateExpand(iX + 1, iY + 1))
	{
		CreateNode(_pParent, iX + 1, iY + 1, true);
	}

	// ↑
	if (ValidateExpand(iX, iY - 1))
	{
		CreateNode(_pParent, iX, iY - 1);
	}

	// ↓
	if (ValidateExpand(iX, iY + 1))
	{
		CreateNode(_pParent, iX, iY + 1);
	}

	// ←
	if (ValidateExpand(iX - 1, iY))
	{
		CreateNode(_pParent, iX - 1, iY);
	}

	// →
	if (ValidateExpand(iX + 1, iY))
	{
		CreateNode(_pParent, iX + 1, iY);
	}
}

bool CNav_Manager::ValidateExpand(_int _iNextX, _int _iNextZ)
{
	// 범위 밖
	if (0 > _iNextX || 0 > _iNextZ ||
		_iNextX > m_iWidth - 1 || _iNextZ > m_iHeight - 1)
		return false;

	// 장애물
	_int iIndex = m_iWidth * _iNextZ + _iNextX;
	if (0 == m_pTileInfo[iIndex].iOpt)
		return false;

	return true;
}

void CNav_Manager::CreateNode(NAV_NODE * _pParent, _int _iNextX, _int _iNextZ, _bool _bDiagonal)
{
	if (m_iCurFindCount >= m_iMaxFindCount)
		return;

	_int iWeight = 0;
	if (!_pParent)
		return;

	if (_bDiagonal)
		iWeight = m_iWeight_Diagonal;
	else
		iWeight = m_iWeight_Staraight;


	//--------------------------------------------------
	// case 1 :
	// 앞으로 가야할 경로 중에 있는지 찾아본다.
	//--------------------------------------------------
	NAV_NODE* pOldNode = nullptr;
	for (auto& pNode : m_OpenList)
	{
		if (pNode->iX == _iNextX && pNode->iZ == _iNextZ)
		{
			pOldNode = pNode;
			break;
		}
	}

	// 있으면..
	if (pOldNode)
	{
		//--------------------------------------------------
		// 새로운 경로가 비용이 더 작으면 갱신하고 재정렬 한다.
		//--------------------------------------------------
		int G = _pParent->iG + iWeight;
		if (pOldNode->iG > G)
		{
			pOldNode->pParent = _pParent;
			pOldNode->iG = G;
			// H는 그대로이므로 갱신 안함
			pOldNode->iF = G + pOldNode->iH;
			m_OpenList.sort([](const NAV_NODE* Src, const NAV_NODE* Dst)
			{
				if (Src->iG < Dst->iG)
					return true;

				if (Src->iG == Dst->iG)
				{
					if (Src->iH < Dst->iH)
						return true;

					if (Src->iH == Dst->iH)
					{
						if (Src->iF < Dst->iF)
							return true;
					}
				}
				return false;
			});
		}
		return;
	}


	//--------------------------------------------------
	// case 2 :
	// 이미 지나친 경로
	//--------------------------------------------------
	for (auto& pNode : m_CloseList)
	{
		if (pNode->iX == _iNextX && pNode->iZ == _iNextZ)
		{
			pOldNode = pNode;
			break;
		}
	}

	if (pOldNode)
	{
		//--------------------------------------------------
		//  -> 비용을 비교해서 갱신해도 된다. (드문 경우)
		//--------------------------------------------------
		return;
	}


	//--------------------------------------------------
	// case 3 :
	// 처음 발견한 경로, 없으므로 새로 추가 후 재정렬
	//--------------------------------------------------
	NAV_NODE* pNewNode = new NAV_NODE;
	pNewNode->pParent = _pParent;
	pNewNode->iX = _iNextX;
	pNewNode->iZ = _iNextZ;
	pNewNode->iG = _pParent->iG + iWeight;
	pNewNode->iH = (abs(_iNextX - m_iGoalX) + abs(_iNextZ - m_iGoalZ)) * 10;
	pNewNode->iF = pNewNode->iG + pNewNode->iH;

	m_OpenList.push_back(pNewNode);
	m_OpenList.sort([](const NAV_NODE* Src, const NAV_NODE* Dst)
	{
		if (Src->iG < Dst->iG)
			return true;

		if (Src->iG == Dst->iG)
		{
			if (Src->iH < Dst->iH)
				return true;

			if (Src->iH == Dst->iH)
			{
				if (Src->iF < Dst->iF)
					return true;
			}
		}
		return false;
	});

	++m_iCurFindCount;
}

void CNav_Manager::Clear_FindInfo()
{
	for (auto& pNode : m_OpenList)
		Safe_Delete(pNode);

	for (auto& pNode : m_CloseList)
		Safe_Delete(pNode);

	m_OpenList.clear();
	m_CloseList.clear();
	m_BestList.clear();

	m_iStartX = -1;
	m_iStartZ = -1;
	m_iGoalX = -1;
	m_iGoalZ = -1;

	m_iCurFindCount = 0;
}

HRESULT CNav_Manager::LerpProcess()
{
	_int iX = 0, iZ = 0;
	bool bRefine = false;

	//--------------------------------------------------
	// node 2개 이상 있는지 검사
	//--------------------------------------------------
	auto iter_start = m_BestList.begin();
	if (2 < m_BestList.size())
	{
		auto iter_terminal = iter_start;
		++iter_terminal;

		m_pAgent->Add_Branch((*iter_start)->iX, (*iter_start)->iZ);

		while (true)
		{
			//--------------------------------------------------
			// 다음 노드 없음, 마지막
			//--------------------------------------------------
			++iter_terminal;
			if (iter_terminal == m_BestList.end())
			{
				--iter_terminal;
				m_pAgent->Add_Branch((*iter_terminal)->iX, (*iter_terminal)->iZ);
				break;
			}

			//--------------------------------------------------
			// 보간처리 할 두 위치 셋팅
			//--------------------------------------------------
			Lerp_Position((*iter_start)->iX, (*iter_start)->iZ, (*iter_terminal)->iX, (*iter_terminal)->iZ);

			while (true)
			{
				//--------------------------------------------------
				// 보간 작업 수행..
				//--------------------------------------------------
				if (!Lerp_Next(&iX, &iZ))
				{
					bRefine = true;
					break;
				}

				//--------------------------------------------------
				// 장애물이 있는지 체크
				//--------------------------------------------------
				if (ValidateExpand(iX, iZ))
				{
					bRefine = false;
					break;
				}
			}

			//--------------------------------------------------
			// 장애물 있음
			//--------------------------------------------------
			if (!bRefine)
			{
				--iter_terminal;
				m_pAgent->Add_Branch((*iter_terminal)->iX, (*iter_terminal)->iZ);

				if (m_pAgent->Get_BranchSize() >= (_int)m_BestList.size())
					break;

				iter_start = iter_terminal;
				++iter_terminal;
			}
		}
	}

	// node 2개
	else
	{
		m_pAgent->Add_Branch((*iter_start)->iX, (*iter_start)->iZ);
		++iter_start;

		m_pAgent->Add_Branch((*iter_start)->iX, (*iter_start)->iZ);
	}

	return S_OK;
}

void CNav_Manager::Lerp_Position(_int _iStartX, _int _iStartZ, _int _iGoalX, _int _iGoalZ)
{
	iLerp_Error = 0;

	m_iLerp_CurX = m_iLerp_StartX = _iStartX;
	m_iLerp_CurZ = m_iLerp_StartZ = _iStartZ;
	m_iLerp_GoalX = _iGoalX;
	m_iLerp_GoalZ = _iGoalZ;

	m_iLerp_DeltaX = m_iLerp_GoalX - _iStartX;
	m_iLerp_DeltaZ = m_iLerp_GoalZ - _iStartZ;

	// X 왼쪽으로 이동
	if (m_iLerp_DeltaX < 0)
	{
		m_iLerp_AddX = -1;
		m_iLerp_DeltaX = -m_iLerp_DeltaX; // abs(iEndX-iStartX)
	}
	// X 오른쪽으로 이동
	else
	{
		m_iLerp_AddX = 1;
	}

	// Y 왼쪽으로 이동
	if (m_iLerp_DeltaZ < 0)
	{
		m_iLerp_AddZ = -1;
		m_iLerp_DeltaZ = -m_iLerp_DeltaZ; // abs(iEndY-iStartY)
	}
	// Y 오른쪽으로 이동
	else
	{
		m_iLerp_AddZ = 1;
	}
}

bool CNav_Manager::Lerp_Next(_int * _pPosX, _int * _pPosZ)
{
	if (m_iLerp_CurX == m_iLerp_GoalX && m_iLerp_CurZ == m_iLerp_GoalZ)
		return false;

	// 기울기 45도 일때는 X,Y 아무거나 축으로..
	// 좌우로 긴 경우
	if (m_iLerp_DeltaX >= m_iLerp_DeltaZ)
	{
		iLerp_Error += m_iLerp_DeltaZ * 2;
		m_iLerp_CurX += m_iLerp_AddX;

		if (iLerp_Error >= m_iLerp_DeltaX)
		{
			iLerp_Error -= m_iLerp_DeltaX * 2;
			m_iLerp_CurZ += m_iLerp_AddZ;
		}
	}
	// 위아래로 긴 경우
	else
	{
		iLerp_Error += m_iLerp_DeltaX * 2;
		m_iLerp_CurZ += m_iLerp_AddZ;

		if (iLerp_Error >= m_iLerp_DeltaZ)
		{
			iLerp_Error -= m_iLerp_DeltaZ * 2;
			m_iLerp_CurX += m_iLerp_AddX;
		}
	}

	*_pPosX = m_iLerp_CurX;
	*_pPosZ = m_iLerp_CurZ;

	return true;
}

void CNav_Manager::Free()
{
	for (auto& pNode : m_OpenList)
		delete pNode;

	for (auto& pNode : m_CloseList)
		delete pNode;

	m_OpenList.clear();
	m_CloseList.clear();
	m_BestList.clear();

	Safe_Delete(m_pTileInfo);
}
