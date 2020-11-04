#include "stdafx.h"
#include "..\Headers\MainCamera.h"

USING(Client)

CMainCamera::CMainCamera(LPDIRECT3DDEVICE9 _pDevice)
	: CCamera(_pDevice)
{
}

CMainCamera::CMainCamera(const CMainCamera & _rOther)
	: CCamera(_rOther)
	, m_tShakeInfo(_rOther.m_tShakeInfo)
	//, m_fDistanceToTarget(_rOther.m_fDistanceToTarget)
	//, m_fZoomInOutSpeedPerSecond(_rOther.m_fZoomInOutSpeedPerSecond)
	//, m_fCameraAngle(_rOther.m_fCameraAngle)
	//, m_fUpDownSpeedPerSecond(_rOther.m_fUpDownSpeedPerSecond)
{
}

HRESULT CMainCamera::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMainCamera::Setup_GameObject(void * _pArg)
{
	if (nullptr == _pArg)
		return E_FAIL;

	memcpy_s(&m_tCameraDesc, sizeof(CCamera::CAMERA_DESC), _pArg, sizeof(CCamera::CAMERA_DESC));
	return S_OK;
}

_int CMainCamera::Update_GameObject(_float _fDeltaTime)
{
	if (FAILED(Movement(_fDeltaTime)))
			return GAMEOBJECT::WARN;

	if (FAILED(ZoomInOut(_fDeltaTime)))
			return GAMEOBJECT::WARN;


	switch (m_eEventMode)
	{
	case Client::CMainCamera::CAMERA_NORMAL:
		break;
	case Client::CMainCamera::CARERA_WIGGING:
		if (FAILED(Wigging(_fDeltaTime)))
			return GAMEOBJECT::WARN;
		break;
	default:
		break;
	}


	return GAMEOBJECT::NOEVENT;
}

_int CMainCamera::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (CCamera::LateUpdate_GameObject(_fDeltaTime))
		return GAMEOBJECT::ERR;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CMainCamera::Movement(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Transform1");
	if (nullptr == pPlayerTransform)
	{
		PRINT_LOG(L"pPlayerTransform is nullptr in CMainCamera::Movement", LOG::CLIENT);
		return E_FAIL;
	}



	//마름모꼴 고정
	_vec3 vXZDistanceNormal = { 0.7f, 0.f, 1.f };
	_vec3 vCameraHeight = { 0.f, 1.f, 0.f };
	_vec3 vSemiTopView = m_fHeight * vCameraHeight - m_fDistance * vXZDistanceNormal;


	switch (m_eViewMode)
	{
	case Client::CMainCamera::CAMERA_3D:


		//시점은 플레이어로
		m_tCameraDesc.vAt = pPlayerTransform->Get_Desc().vPosition;


		m_tCameraDesc.vEye = m_tCameraDesc.vAt + vSemiTopView;

		break;
	case Client::CMainCamera::CAMERA_2D_X:

		m_tCameraDesc.vAt = pPlayerTransform->Get_Desc().vPosition;

		m_tCameraDesc.vEye = m_tCameraDesc.vAt + _vec3(1.f, 0.f, 0.f) * m_fDistanceOn2DView;

		break;
	case Client::CMainCamera::CAMERA_2D_Z:

		m_tCameraDesc.vAt = pPlayerTransform->Get_Desc().vPosition;

		m_tCameraDesc.vEye = m_tCameraDesc.vAt + _vec3(0.f, 0.f, 1.f) * m_fDistanceOn2DView;

		break;
	default:
		break;
	}


	return S_OK;
}

HRESULT CMainCamera::ZoomInOut(_float _fDeltaTime)
{
	if (GetAsyncKeyState(VK_ADD) & 0x8000)
	{
		m_fHeight -= m_fZoomInOutSpeedPerSecond * _fDeltaTime * m_fStartHeight;
		m_fDistance -= m_fZoomInOutSpeedPerSecond * _fDeltaTime * m_fStartDistance;
	}
	if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
	{
		m_fHeight += m_fZoomInOutSpeedPerSecond * _fDeltaTime* m_fStartHeight;
		m_fDistance += m_fZoomInOutSpeedPerSecond * _fDeltaTime * m_fStartDistance;
	}

	if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
	{
		m_fHeight -= m_fUpDownSpeedPerSecond * _fDeltaTime * m_fStartHeight;
	}
	if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
	{
		m_fHeight += m_fUpDownSpeedPerSecond * _fDeltaTime* m_fStartHeight;
	}

	return S_OK;
}

HRESULT CMainCamera::Wigging(_float _fDeltaTime)
{
	//시간 지났을시(혹은 진동강도가 0이 되었을 시) 초기화, 유한기계상태 중립
	if (m_tShakeInfo.m_fSettingTime < m_tShakeInfo.m_fTimeFlow || m_tShakeInfo.m_fMagnitude < 0)
	{
		m_tShakeInfo.m_fSettingTime = 0.f;
		m_tShakeInfo.m_fTimeFlow = 0.f;
		m_eEventMode = CAMERA_EVNETMODE::CAMERA_NORMAL;
	}
	//시간 누적
	m_tShakeInfo.m_fTimeFlow += _fDeltaTime;
	
	//-----------------------------------------------------------------------
	//실제로 흔드는 함수
	//DAMPED : 감쇠진동(일정한 힘을 밖으로 방출하는 진동이 서서히 약해지는 진동)
	//HARMONIC : 조화진동(시간동안 일정한 힘으로 진동)
	//-----------------------------------------------------------------------
	switch (m_tShakeInfo.m_eWigType)
	{
	case Client::CMainCamera::DAMPED:
		m_tShakeInfo.m_fMagnitude -= _fDeltaTime;
		m_tCameraDesc.vEye += m_tCameraDesc.vUp * sinf(m_tShakeInfo.m_fTimeFlow* m_tShakeInfo.m_fFrequency) * m_tShakeInfo.m_fMagnitude;
		break;
	case Client::CMainCamera::HARMONIC:
		m_tCameraDesc.vEye += m_tCameraDesc.vUp * sinf(m_tShakeInfo.m_fTimeFlow* m_tShakeInfo.m_fFrequency) * m_tShakeInfo.m_fMagnitude;
		break;
	case Client::CMainCamera::MIXED:
		break;
	default:
		break;
	}
	


	return S_OK;
}

HRESULT CMainCamera::Set_Camera_Wigging(_float _Magnitude, _float _Frequency, _float _Time, WIG_TYPE _Option)
{
	if (_Magnitude < 0 || _Frequency < 0 || _Time < 0)
	{
		PRINT_LOG(_T("Camera Wigging Fail, Check The Value Which is Less Then 0"), LOG::CLIENT);
		return E_FAIL;
	}

	m_eEventMode = CAMERA_EVNETMODE::CARERA_WIGGING;
	m_tShakeInfo.m_eWigType = _Option;
	m_tShakeInfo.m_fMagnitude = _Magnitude;
	m_tShakeInfo.m_fFrequency = _Frequency;
	m_tShakeInfo.m_fSettingTime = _Time;

	return S_OK;
}

HRESULT CMainCamera::Set_Camera_Mode(CAMERA_VIEWMODE _ModeOption,  void* _Option)
{
	if (_ModeOption >= VIEWMODE_END)
	{
		PRINT_LOG(L"Failed To Change ViewState, Check The Value", LOG::CLIENT);
		return E_FAIL;
	}

	m_eViewMode = _ModeOption;

	if (m_eViewMode != CAMERA_3D && _Option != nullptr)
	{
		m_fDistanceOn2DView = *((_float*)_Option);
	}

	return S_OK;
}

CMainCamera * CMainCamera::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CMainCamera* pInstance = new CMainCamera(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMainCamera", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMainCamera::Clone_GameObject(void * _pArg)
{
	CMainCamera* pInstance = new CMainCamera(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMainCamera", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainCamera::Free()
{
	CCamera::Free();
}




//폐기코드 Movement에 플레이어.
/*_vec3 vPlayerLook;
memcpy_s(&vPlayerLook, sizeof(_vec3), &pPlayerTransform->Get_Desc().matWorld.m[2][0], sizeof(_vec3));
D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

vPlayerLook *= -m_fDistanceToTarget;

_vec3 vPlayerRight;
memcpy_s(&vPlayerRight, sizeof(_vec3), &pPlayerTransform->Get_Desc().matWorld.m[0][0], sizeof(_vec3));
D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

_matrix matRotAxis;
D3DXMatrixRotationAxis(&matRotAxis, &vPlayerRight, m_fCameraAngle);
D3DXVec3TransformNormal(&vPlayerLook, &vPlayerLook, &matRotAxis);

m_tCameraDesc.vEye = m_tCameraDesc.vAt + vPlayerLook;*/
