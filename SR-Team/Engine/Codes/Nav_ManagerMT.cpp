#include "GameObject.h"
#include "NavAgent.h"
#include "..\Headers\Nav_ManagerMT.h"

USING(Engine)

IMPLEMENT_SINGLETON(CNavigation_ManagerMT)

CNavigation_ManagerMT::CNavigation_ManagerMT()
{
}

HRESULT CNavigation_ManagerMT::Setup_NavigationManager()
{
	m_hNavEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (NULL == m_hNavEvent)
	{
		PRINT_LOG(L"Failed To Create NavEvent", LOG::ENGINE);
		return E_FAIL;
	}

	m_hNavThread = (HANDLE)_beginthreadex(NULL, 0, Call_NavUpdateThread, this, 0, NULL);
	if (NULL == m_hNavThread)
	{
		PRINT_LOG(L"Failed To NavThread Begin", LOG::ENGINE);
		return E_FAIL;
	}

	return S_OK;
}

_int CNavigation_ManagerMT::Update_NavgationManager()
{
	_int iRet = 0;
	m_iCntPerCycle = 0;

	while (true)
	{
		//--------------------------------------------------
		// 처리된 결과가 없을 경우
		//--------------------------------------------------
		if (m_QueueDone.empty())
			break;

		//--------------------------------------------------
		// 무분별한 대기 방지
		//--------------------------------------------------
		if (m_iCntPerCycle >= m_iMaxPerCycle)
			break;

		CNavAgent* pNavAgent = m_QueueDone.front();
		m_QueueDone.pop();
		
		if (!pNavAgent)
		{
			iRet = -1;
			break;
		}

		CGameObject* pGameObject = pNavAgent->Get_Owner();
		if (!pGameObject)
		{
			iRet = -1;
			break;
		}


		//--------------------------------------------------
		// 완료 상태로 변경 후 RefCnt 낮춰준다.
		// 여기서 dead 처리를 할 경우 layer list에서 제거를 해줄 수 없게 된다.
		// object update에서 refcnt를 확인해서 dead 처리를 지연 해주어야 한다.
		//--------------------------------------------------
		pNavAgent->Set_NavDone();
		Safe_Release(pGameObject);
		++m_iCntPerCycle;
	}

	return iRet;
}

HRESULT CNavigation_ManagerMT::NavUpdateThread()
{
	DWORD dwResult = 0;

	while (!m_bThreadDown)
	{
		//--------------------------------------------------
		// 신호 받을때까지 스레드 대기
		//--------------------------------------------------
		dwResult = WaitForSingleObject(m_hNavEvent, INFINITE);

		if (WAIT_OBJECT_0 != dwResult)
		{
			PRINT_LOG(L"Wait NavEvent Error!", LOG::ENGINE);
			return E_FAIL;
		}

		// TODO : 길 찾기..


	}

	return S_OK;
}

_uint CNavigation_ManagerMT::Call_NavUpdateThread(LPVOID _lpParam)
{
	CNavigation_ManagerMT* pNavMgr = (CNavigation_ManagerMT*)_lpParam;
	if (nullptr == pNavMgr)
		return -1;

	if (FAILED(pNavMgr->NavUpdateThread()))
		return -1;

	return 0;
}

HRESULT CNavigation_ManagerMT::Add_NavAgent(CNavAgent* _pNavAgent)
{
	if (nullptr == _pNavAgent)
	{
		PRINT_LOG(L"NavAgent is nullptr in Add_NavAgent", LOG::ENGINE);
		return E_FAIL;
	}

	CGameObject* pGameObject = _pNavAgent->Get_Owner();
	if (nullptr == pGameObject)
	{
		PRINT_LOG(L"GameObject Not Found!", LOG::ENGINE);
		return E_FAIL;
	}

	//--------------------------------------------------
	// RefCnt부터 올린 뒤 Enqueue
	//--------------------------------------------------
	pGameObject->AddRef();
	m_QueueWait.push(_pNavAgent);

	//--------------------------------------------------
	// 스레드가 깨어나도록 신호를 보낸다
	//--------------------------------------------------
	SetEvent(m_hNavEvent);
	
	return S_OK;
}

void CNavigation_ManagerMT::Free()
{
	//--------------------------------------------------
	// 스레드부터 종료시킨 뒤 큐를 비워야 함
	//--------------------------------------------------
	m_bThreadDown = true;
	SetEvent(m_hNavEvent);

	WaitForSingleObject(m_hNavThread, INFINITE);

	while (!m_QueueWait.empty())
	{
		CNavAgent* pNavAgent = m_QueueWait.front();
		m_QueueWait.pop();
		CGameObject* pGameObject = pNavAgent->Get_Owner();
		Safe_Release(pGameObject);
	}

	while (!m_QueueDone.empty())
	{
		CNavAgent* pNavAgent = m_QueueDone.front();
		m_QueueDone.pop();
		CGameObject* pGameObject = pNavAgent->Get_Owner();
		Safe_Release(pGameObject);
	}

	//--------------------------------------------------
	// 이벤트, 스레드 핸들 닫음
	//--------------------------------------------------
	CloseHandle(m_hNavEvent);
	CloseHandle(m_hNavThread);
}
