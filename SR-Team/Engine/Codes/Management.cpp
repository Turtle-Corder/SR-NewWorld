#include "..\Headers\Management.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pFrame_Manager(CFrame_Manager::Get_Instance())
	, m_pDevice_Manager(CDevice_Manager::Get_Instance())
	, m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pScene_Manager(CScene_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pNav_Manager(CNav_Manager::Get_Instance())
	, m_pKey_Manager(CKey_Manager::Get_Instance())
{
	Safe_AddRef(m_pFrame_Manager);
	Safe_AddRef(m_pDevice_Manager);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pScene_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pNav_Manager);
	Safe_AddRef(m_pKey_Manager);
}

void CManagement::Free()
{
	Safe_Release(m_pNav_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pScene_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pDevice_Manager);
	Safe_Release(m_pFrame_Manager);
	Safe_Release(m_pKey_Manager);
}

HRESULT CManagement::Setup_Engine(HWND _hWnd, _uint _iFramePerSec, _uint _iWinCX, _uint _iWinCY, CDevice_Manager::DISPLAY_MODE _eDisplayMode, _uint _iSceneCnt, const wstring & _strAppTimerTag)
{
	if (nullptr == m_pKey_Manager		||
		nullptr == m_pFrame_Manager		||
		nullptr == m_pDevice_Manager	||
		nullptr == m_pScene_Manager		||
		nullptr == m_pTimer_Manager		||
		nullptr == m_pComponent_Manager ||
		nullptr == m_pObject_Manager	||
		nullptr == m_pNav_Manager)
		return E_FAIL;

	if (FAILED(m_pKey_Manager->Setup_KeyManager()))
		return E_FAIL;

	if (FAILED(m_pFrame_Manager->Setup_FrameManager(_hWnd, _iFramePerSec)))
		return E_FAIL;

	if (FAILED(m_pDevice_Manager->Setup_GraphicDevice(_hWnd, _iWinCX, _iWinCY, _eDisplayMode)))
		return E_FAIL;

	if (FAILED(m_pTimer_Manager->Setup_TimerManager()))
		return E_FAIL;

	if (FAILED(m_pTimer_Manager->Add_Timer(_strAppTimerTag, true)))
		return E_FAIL;
	m_strAppTimerTag = _strAppTimerTag;

	m_pRenderer = CRenderer::Create(m_pDevice_Manager->Get_Device(), m_pDevice_Manager->Get_Sprite(), m_pDevice_Manager->Get_Font());
	if (nullptr == m_pRenderer)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Setup_Component_Manager(_iSceneCnt)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Setup_Object_Manager(_iSceneCnt)))
		return E_FAIL;

	if (FAILED(m_pNav_Manager->Setup_NavigationManager()))
		return E_FAIL;

	return S_OK;
}

_int CManagement::Update_Engine(void)
{
	if (nullptr == m_pTimer_Manager ||
		nullptr == m_pScene_Manager ||
		nullptr == m_pObject_Manager)
		return -1;


	//--------------------------------------------------
	// timer
	//--------------------------------------------------
	_float fDeltaTime = 0.f;
	if (m_iUpdate_Result = m_pTimer_Manager->Update_TimeManager())
		return 0;

	if (FAILED(m_pTimer_Manager->Get_DeltaTime(m_strAppTimerTag, fDeltaTime)))
		return 0;


	//--------------------------------------------------
	// Update
	//--------------------------------------------------
	if (m_iUpdate_Result = m_pScene_Manager->Update_Scene_Manager(fDeltaTime))
		return 0;

	if (m_iUpdate_Result = m_pObject_Manager->Update_Object_Manger(fDeltaTime))
		return 0;


	//--------------------------------------------------
	// LateUpdate
	//--------------------------------------------------
	if (m_iUpdate_Result = m_pScene_Manager->LateUpdate_Scene_Manager(fDeltaTime))
		return 0;

	if (m_iUpdate_Result = m_pObject_Manager->LateUpdate_Object_Manager(fDeltaTime))
		return 0;

	m_pKey_Manager->Key_Update();

	return 0;
}

HRESULT CManagement::Render_Engine(void)
{
	//--------------------------------------------------
	// 씬 변경으로인한 Frame Skip / UNDONE : enum?
	//--------------------------------------------------
	if (1 == m_iUpdate_Result)
	{
		m_iUpdate_Result = 0;
		return S_OK;
	}

	if (nullptr == m_pRenderer)
		return E_FAIL;

	if (FAILED(m_pRenderer->Render_Renderer()))
		return E_FAIL;

	return S_OK;
}

_uint CManagement::Release_Engine()
{
	_uint iRefCnt = 0;
	if (iRefCnt = CManagement::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CManagement", LOG::ENGINE);
		return iRefCnt;
	}

	// obj
	if (iRefCnt = CObject_Manager::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CObject_Manager", LOG::ENGINE);
		return iRefCnt;
	}

	// component
	if (iRefCnt = CComponent_Manager::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CComponent_Manager", LOG::ENGINE);
		return iRefCnt;
	}

	// scene
	if (iRefCnt = CScene_Manager::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CScene_Manager", LOG::ENGINE);
		return iRefCnt;
	}

	// timer
	if (iRefCnt = CTimer_Manager::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CTimer_Manager", LOG::ENGINE);
		return iRefCnt;
	}

	// nav
	if (iRefCnt = CNav_Manager::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CNav_Manager", LOG::ENGINE);
		return iRefCnt;
	}

	// frame
	if (iRefCnt = CFrame_Manager::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CFrame_Manager", LOG::ENGINE);
		return iRefCnt;
	}

	if (iRefCnt = CKey_Manager::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CKey_Manager", LOG::ENGINE);
		return iRefCnt;
	}

	// device
	if (iRefCnt = CDevice_Manager::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CDevice_Manager", LOG::ENGINE);
		return iRefCnt;
	}

	return iRefCnt;
}

HRESULT CManagement::ClearScene_All(_uint _iSceneID)
{
	if (nullptr == m_pComponent_Manager || nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Clear_ForScene(_iSceneID)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Clear_ForScene(_iSceneID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CManagement::ClearScene_Component_All(_uint _iSceneID)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Clear_ForScene(_iSceneID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CManagement::ClearScene_Object_All(_uint _iSceneID)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Clear_ForScene(_iSceneID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CManagement::ClearScene_Except_RegisterTag(_uint _iSceneID, const wstring & _strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Register_ExceptTag(_iSceneID, _strLayerTag);
}

HRESULT CManagement::Clear_Except(_uint _iFromSceneID, _uint _iToSceneID)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Clear_Except(_iFromSceneID, _iToSceneID);
}

LPDIRECT3DDEVICE9 CManagement::Get_Device(void) const
{
	if (nullptr == m_pDevice_Manager)
		return nullptr;

	return m_pDevice_Manager->Get_Device();
}

LPD3DXSPRITE CManagement::Get_Sprite(void) const
{
	if (nullptr == m_pDevice_Manager)
		return nullptr;

	return m_pDevice_Manager->Get_Sprite();
}

LPD3DXFONT CManagement::Get_Font(void) const
{
	if (nullptr == m_pDevice_Manager)
		return nullptr;

	return m_pDevice_Manager->Get_Font();
}

_bool CManagement::Lock_FrameManager()
{
	if (nullptr == m_pFrame_Manager)
		return false;

	return m_pFrame_Manager->Lock_FrameManager();
}

void CManagement::Render_FrameManager()
{
	if (nullptr == m_pFrame_Manager)
		return;

	m_pFrame_Manager->Render_FrameManager();
}

HRESULT CManagement::Add_Timer(const wstring & _strTimerTag, bool _bStart)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(_strTimerTag, _bStart);
}

HRESULT CManagement::Get_DeltaTime(const wstring & _strTimerTag, float & _fDeltaTime)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Get_DeltaTime(_strTimerTag, _fDeltaTime);
}

HRESULT CManagement::Pause(const wstring & _strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Pause(_strTimerTag);
}

HRESULT CManagement::Resume(const wstring & _strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Resume(_strTimerTag);
}

HRESULT CManagement::Add_RendererList(CRenderer::eRENDER_TYPE _eType, CGameObject * _pObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RendererList(_eType, _pObject);
}

HRESULT CManagement::CollisionSphere_Detection_Layers(_int _iSceneID, const wstring & _strSrcLayerTag, const wstring & _strDstLayerTag, const wstring & _strColliderTag, const wstring & _strDmgInfoTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->CollisionSphere_Detection_Layers(_iSceneID, _strSrcLayerTag, _strDstLayerTag, _strColliderTag, _strDmgInfoTag);
}

HRESULT CManagement::CollisionSphere_Detection_Layers_Both(_int _iSceneID, const wstring & _strSrcLayerTag, const wstring & _strDstLayerTag, const wstring & _strColliderTag, const wstring & _strDmgInfoTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->CollisionSphere_Detection_Layers_Both(_iSceneID, _strSrcLayerTag, _strDstLayerTag, _strColliderTag, _strDmgInfoTag);
}

HRESULT CManagement::CollisionBox_Detection_Layers(_int _iSceneID, const wstring & _strSrcLayerTag, const wstring & _strDstLayerTag, const wstring & _strColliderTag, const wstring & _strDmgInfoTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->CollisionBox_Detection_Layers(_iSceneID, _strSrcLayerTag, _strDstLayerTag, _strColliderTag, _strDmgInfoTag);
}

HRESULT CManagement::CollisionBox_Detection_Layers_Both(_int _iSceneID, const wstring & _strSrcLayerTag, const wstring & _strDstLayerTag, const wstring & _strColliderTag, const wstring & _strDmgInfoTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->CollisionBox_Detection_Layers_Both(_iSceneID, _strSrcLayerTag, _strDstLayerTag, _strColliderTag, _strDmgInfoTag);
}

HRESULT CManagement::CollisionSphere_Impulse_Layers(_int _iSceneID, const wstring & _strSrcLayerTag, const wstring & _strDstLayerTag, const wstring & _strColliderTag, const wstring & _strTransformTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->CollisionSphere_Impulse_Layers(_iSceneID, _strSrcLayerTag, _strDstLayerTag, _strColliderTag, _strTransformTag);
}

HRESULT CManagement::CollisionBox_Impulse_Layers(_int _iSceneID, const wstring & _strSrcLayerTag, const wstring & _strDstLayerTag, const wstring & _strColliderTag, const wstring & _strTransformTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->CollisionBox_Impulse_Layers(_iSceneID, _strSrcLayerTag, _strDstLayerTag, _strColliderTag, _strTransformTag);
}

HRESULT CManagement::Change_CurrentScene(_uint _iSceneID, CScene * pCurrentScene)
{
	if (nullptr == m_pScene_Manager)
		return E_FAIL;

	return m_pScene_Manager->Change_CurrentScene(_iSceneID, pCurrentScene);
}

_int CManagement::Get_CurrentSceneID()
{
	if (nullptr == m_pScene_Manager)
		return -1;

	return m_pScene_Manager->Get_CurrentSceneID();
}

CGameObject * CManagement::Get_GameObject(_int _iSceneID, const wstring & _strLayerTag, _uint _iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_GameObject(_iSceneID, _strLayerTag, _iIndex);
}

CComponent * CManagement::Get_Component(_int _iSceneID, const wstring & _strLayerTag, const wstring & _strComponentTag, _uint _iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(_iSceneID, _strLayerTag, _strComponentTag, _iIndex);
}

HRESULT CManagement::Add_GameObject_Prototype(_int _iSceneID, const wstring & _strPrototypeTag, CGameObject * _pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject_Prototype(_iSceneID, _strPrototypeTag, _pPrototype);
}

HRESULT CManagement::Add_GameObject_InLayer(_int _iFromSceneID, const wstring & _strPrototypeTag, _int _iToSceneID, const wstring & _strLayerTag, void* _pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject_InLayer(_iFromSceneID, _strPrototypeTag, _iToSceneID, _strLayerTag, _pArg);
}

HRESULT CManagement::Add_Component_Prototype(_int _iSceneID, const wstring & _strPrototypeTag, CComponent * _pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Component_Prototype(_iSceneID, _strPrototypeTag, _pPrototype);
}

CComponent * CManagement::Clone_Component(_int _iSceneID, const wstring& _strPrototypeTag, void* _pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(_iSceneID, _strPrototypeTag, _pArg);
}

HRESULT CManagement::Set_TileInfo(TILEINFO * _pTileInfo, _int _iWidth, _int _iHeight)
{
	if (nullptr == m_pNav_Manager)
		return E_FAIL;

	return m_pNav_Manager->Set_TileInfo(_pTileInfo, _iWidth, _iHeight);
}

HRESULT CManagement::PathFind(CNavAgent * _pAgent, _int _iStartX, _int _iStartZ, _int _iGoalX, _int _iGoalZ)
{
	if (nullptr == m_pNav_Manager)
		return E_FAIL;

	return m_pNav_Manager->PathFind(_pAgent, _iStartX, _iStartZ, _iGoalX, _iGoalZ);
}

_bool CManagement::Key_Pressing(_int _key)
{
	if (nullptr == m_pKey_Manager)
		return false;

	return m_pKey_Manager->Key_Pressing(_key);
}

_bool CManagement::Key_Down(_int _key)
{
	if (nullptr == m_pKey_Manager)
		return false;

	return m_pKey_Manager->Key_Down(_key);
}

_bool CManagement::Key_Up(_int _key)
{
	if (nullptr == m_pKey_Manager)
		return false;

	return m_pKey_Manager->Key_Up(_key);
}
