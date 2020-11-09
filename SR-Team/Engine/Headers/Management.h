#pragma once
#ifndef __MANAGEMENT_H__
#define __MANAGEMENT_H__

#include "Device_Manager.h"
#include "Scene_Manager.h"
#include "Timer_Manager.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Nav_Manager.h"
#include "FrameManager.h"
#include "Key_Manager.h"
#include "Renderer.h"

BEGIN(Engine)

class ENGINE_DLL CManagement final : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	CManagement(void);
	virtual ~CManagement(void) = default;



	//----------------------------------------------------------------------------------------------------
	// 일반 함수
	//----------------------------------------------------------------------------------------------------
public:
	HRESULT Setup_Engine(HWND _hWnd, _uint _iFramePerSec, _uint _iWinCX, _uint _iWinCY, CDevice_Manager::DISPLAY_MODE _eDisplayMode, _uint _iSceneCnt, const wstring& _strAppTimerTag);
	_int	Update_Engine(void);
	HRESULT Render_Engine(void);
	static _uint Release_Engine();



	//----------------------------------------------------------------------------------------------------
	// 인터페이스
	//----------------------------------------------------------------------------------------------------
public:
	//--------------------------------------------------
	// 공통
	//--------------------------------------------------
	// 전부
	HRESULT ClearScene_All(_uint _iSceneID);

	// 컴포넌트
	HRESULT ClearScene_Component_All(_uint _iSceneID);

	// 오브젝트
	HRESULT ClearScene_Object_All(_uint _iSceneID);

	// 레이어 태그를 등록해두면 해당 레이어는 다음 씬으로 살려서 이동시킨다.
	HRESULT ClearScene_Except_RegisterTag(_uint _iSceneID, const wstring& _strLayerTag);

	HRESULT Clear_Except(_uint _iFromSceneID, _uint _iToSceneID);


	//--------------------------------------------------
	// 장치
	//--------------------------------------------------
	LPDIRECT3DDEVICE9	Get_Device(void)	const;
	LPD3DXSPRITE		Get_Sprite(void)	const;
	LPD3DXFONT			Get_Font(void)		const;


	//--------------------------------------------------
	// 프레임
	//--------------------------------------------------
	_bool Lock_FrameManager();
	void Render_FrameManager();


	//--------------------------------------------------
	// 타이머
	//--------------------------------------------------
	HRESULT Add_Timer(const wstring& _strTimerTag, bool _bStart = true);
	HRESULT Get_DeltaTime(const wstring& _strTimerTag, float& _fDeltaTime);

	HRESULT Pause(const wstring& _strTimerTag);
	HRESULT Resume(const wstring& _strTimerTag);


	//--------------------------------------------------
	// 렌더러
	//--------------------------------------------------
	HRESULT Add_RendererList(CRenderer::eRENDER_TYPE _eType, CGameObject* _pObject);


	//--------------------------------------------------
	// 콜라이더
	//--------------------------------------------------
	HRESULT CollisionSphere_Detection_Layers(_int _iSceneID, const wstring& _strSrcLayerTag, const wstring& _strDstLayerTag, const wstring& _strColliderTag, const wstring& _strDmgInfoTag);
	HRESULT CollisionSphere_Detection_Layers_Both(_int _iSceneID, const wstring& _strSrcLayerTag, const wstring& _strDstLayerTag, const wstring& _strColliderTag, const wstring& _strDmgInfoTag);

	HRESULT CollisionBox_Detection_Layers(_int _iSceneID, const wstring& _strSrcLayerTag, const wstring& _strDstLayerTag, const wstring& _strColliderTag, const wstring& _strDmgInfoTag);
	HRESULT CollisionBox_Detection_Layers_Both(_int _iSceneID, const wstring& _strSrcLayerTag, const wstring& _strDstLayerTag, const wstring& _strColliderTag, const wstring& _strDmgInfoTag);

	HRESULT CollisionSphere_Impulse_Layers(_int _iSceneID, const wstring& _strSrcLayerTag, const wstring& _strDstLayerTag, const wstring& _strColliderTag, const wstring& _strTransformTag);
	HRESULT CollisionBox_Impulse_Layers(_int _iSceneID, const wstring& _strSrcLayerTag, const wstring& _strDstLayerTag, const wstring& _strColliderTag, const wstring& _strTransformTag);


	//--------------------------------------------------
	// 씬
	//--------------------------------------------------
	HRESULT Change_CurrentScene(_uint _iSceneID, CScene* pCurrentScene);
	_int Get_CurrentSceneID();


	//--------------------------------------------------
	// 오브젝트
	//--------------------------------------------------
	CGameObject* Get_GameObject(_int _iSceneID, const wstring& _strLayerTag, _uint _iIndex = 0);
	CComponent* Get_Component(_int _iSceneID, const wstring& _strLayerTag, const wstring& _strComponentTag, _uint _iIndex = 0);
	HRESULT Add_GameObject_Prototype(_int _iSceneID, const wstring& _strPrototypeTag, CGameObject* _pPrototype);
	HRESULT Add_GameObject_InLayer(_int _iFromSceneID, const wstring& _strPrototypeTag, _int _iToSceneID, const wstring& _strLayerTag, void* _pArg = nullptr);
	HRESULT Add_GameObject_InLayer(CGameObject** _pOut, _int _iFromSceneID, const wstring& _strPrototypeTag, _int _iToSceneID, const wstring& _strLayerTag, void* _pArg = nullptr);


	//--------------------------------------------------
	// 컴포넌트
	//--------------------------------------------------
	HRESULT Add_Component_Prototype(_int _iSceneID, const wstring& _strPrototypeTag, CComponent* _pPrototype);
	CComponent* Clone_Component(_int _iSceneID, const wstring& _strPrototypeTag, void* _pArg = nullptr);


	//--------------------------------------------------
	// 길찾기
	//--------------------------------------------------
	HRESULT Set_TileInfo(TILEINFO* _pTileInfo, _int _iWidth, _int _iHeight);
	HRESULT PathFind(CNavAgent* _pAgent, _int _iStartX, _int _iStartZ, _int _iGoalX, _int _iGoalZ);


	//--------------------------------------------------
	// 키 입력
	//--------------------------------------------------
	_bool Key_Pressing(_int _key);
	_bool Key_Down(_int _key);
	_bool Key_Up(_int _key);


	//--------------------------------------------------
	// 사운드
	//--------------------------------------------------
	//void PlayBGM(TCHAR* pSoundKey);

	//void PlayUnit(TCHAR* pSoundKey);
	//void PlayMonster(TCHAR* pSoundKey);
	//void PlayEffect(TCHAR* pSoundKey);
	//void PlayUI(TCHAR* pSoundKey);

	//void StopSound(_int iChannel);
	//void StopAll();


public:
	virtual void Free() override;



	//----------------------------------------------------------------------------------------------------
	// 변수
	//----------------------------------------------------------------------------------------------------
private:
	CDevice_Manager*	m_pDevice_Manager		= nullptr;
	CTimer_Manager*		m_pTimer_Manager		= nullptr;
	CScene_Manager*		m_pScene_Manager		= nullptr;
	CComponent_Manager*	m_pComponent_Manager	= nullptr;
	CObject_Manager*	m_pObject_Manager		= nullptr;
	CNav_Manager*		m_pNav_Manager			= nullptr;
	CFrame_Manager*		m_pFrame_Manager		= nullptr;
	CKey_Manager*		m_pKey_Manager			= nullptr;

	CRenderer*			m_pRenderer				= nullptr;

	_int				m_iUpdate_Result = 0;
	wstring				m_strAppTimerTag;

	_bool				m_bAppDown = false;
};

END

#endif
