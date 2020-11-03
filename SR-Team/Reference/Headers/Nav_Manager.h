#pragma once
#ifndef __NAV_MANAGER_H__
#define __NAV_MANAGER_H__

#include "Base.h"

#define NAVNODE_MAX 3000

BEGIN(Engine)
class CNavAgent;
class CVIBuffer;
class CNav_Manager final : public CBase
{
public:
	typedef struct tagNavNode
	{
		tagNavNode* pParent = nullptr;
		_int iX, iZ;
		_int iG, iH, iF;
	} NAV_NODE;

	DECLARE_SINGLETON(CNav_Manager)

private:
	CNav_Manager();
	virtual ~CNav_Manager() = default;

public:
	HRESULT Setup_NavigationManager();
	
	HRESULT Set_TileInfo(TILEINFO* _pTileInfo, _int _iWidth, _int _iHeight);

	HRESULT PathFind(CNavAgent* _pAgent, _int _iStartX, _int _iStartZ, _int _iGoalX, _int _iGoalZ);

private:
	//----------------------------------------------------------------------------------------------------
	// 러프한 길찾기
	//----------------------------------------------------------------------------------------------------
	HRESULT FindProcess();

	void CreateExpandNode(NAV_NODE* _pParent);

	bool ValidateExpand(_int _iNextX, _int _iNextZ);

	void CreateNode(NAV_NODE* _pParent, _int _iNextX, _int _iNextZ, _bool _bDiagonal = false);

	void Clear_FindInfo();


	//----------------------------------------------------------------------------------------------------
	// 보간 처리
	//----------------------------------------------------------------------------------------------------
	HRESULT LerpProcess();

	void Lerp_Position(_int _iStartX, _int _iStartZ, _int _iGoalX, _int _iGoalZ);

	bool Lerp_Next(_int* _pPosX, _int* _pPosZ);


public:
	virtual void Free() override;



private:
	//----------------------------------------------------------------------------------------------------
	// (터레인)타일
	//----------------------------------------------------------------------------------------------------
	TILEINFO* m_pTileInfo = nullptr;
	_int m_iWidth = 0;
	_int m_iHeight = 0;

	_int m_iWeight_Staraight = 10;
	_int m_iWeight_Diagonal = 14;


	//----------------------------------------------------------------------------------------------------
	// 길찾기
	//----------------------------------------------------------------------------------------------------
	CNavAgent* m_pAgent = nullptr;
	list<NAV_NODE*> m_OpenList;
	list<NAV_NODE*> m_CloseList;
	list<NAV_NODE*> m_BestList;

	_int m_iStartX, m_iStartZ;
	_int m_iGoalX, m_iGoalZ;

	NAV_NODE m_arryNodes[NAVNODE_MAX] = {};
	_int m_iMaxFindCount = NAVNODE_MAX;
	_int m_iCurFindCount;


	//----------------------------------------------------------------------------------------------------
	// 보간
	//----------------------------------------------------------------------------------------------------
	_int iLerp_Error;
	_int m_iLerp_AddX, m_iLerp_AddZ;
	_int m_iLerp_CurX, m_iLerp_CurZ;

	_int m_iLerp_DeltaX, m_iLerp_DeltaZ;
	_int m_iLerp_StartX, m_iLerp_StartZ;
	_int m_iLerp_GoalX, m_iLerp_GoalZ;
};

END

#endif // !__NAV_MANAGER_H__
