#include "stdafx.h"
#include "..\Headers\Mouse.h"

USING(Client)

CMouse::CMouse(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint iCnt = 0; iCnt < KIND_END; ++iCnt)
		m_pTransformComp[iCnt] = nullptr;

	for (_uint iCnt = 0; iCnt < STATE_END; ++iCnt)
		m_pTextrueComp[iCnt] = nullptr;
}

CMouse::CMouse(const CMouse & _rOther)
	: CUIObject(_rOther)
{
}

HRESULT CMouse::Add_Component()
{
	TCHAR szComponentTag[][MIN_STR] =
	{
		L"Component_Texture_Mouse_Idle",
		L"Component_Texture_Mouse_Grab",
		L"Component_Texture_Mouse_Help",
		L"Component_Texture_Mouse_Click",
		L"Component_Texture_Mouse_Work"
	};

	TCHAR szTexture[MIN_STR] = L"Com_Texture%d";
	TCHAR szCombine[MIN_STR] = L"";

	for (_uint iCnt = 0; iCnt < STATE_END; ++iCnt)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, iCnt);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szComponentTag[iCnt], szCombine, (CComponent**)&m_pTextrueComp[iCnt])))
			return E_FAIL;
	}

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform0", (CComponent**)&m_pTransformComp[KIND::ACTUAL])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform2", (CComponent**)&m_pTransformComp[KIND::RENDER])))
		return E_FAIL;

	return S_OK;
}

HRESULT CMouse::Change_State(CMouse::STATE _eState)
{
	if (_eState == m_eState)
		return E_FAIL;

	_uint iCnt = m_pTextrueComp[_eState]->Get_TextureCount();
	m_pTextrueComp[_eState]->SetFrameRange(0, iCnt - 1);

	m_iCurrFrame = 0;
	m_eState = _eState;
	
	return S_OK;
}

const POINT & CMouse::Get_Point()
{
	return m_tPoint;
}

HRESULT CMouse::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMouse::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_eState = IDLE;

	return S_OK;
}

int CMouse::Update_GameObject(float _fDeltaTime)
{
	GetCursorPos(&m_tPoint);
	ScreenToClient(g_hWnd, &m_tPoint);

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		_int iRand = rand() % (_int)STATE::STATE_END;
		Change_State((STATE)iRand);
	}

	m_pTransformComp[KIND::ACTUAL]->Set_Position(_vec3( (_float)m_tPoint.x, (_float)m_tPoint.y, 0.f ));
	m_pTransformComp[KIND::ACTUAL]->Update_Transform();

	m_pTransformComp[KIND::RENDER]->Set_Position(_vec3((_float)(m_tPoint.x + 16.f), (_float)(m_tPoint.y + 16.f), 0.f));
	m_pTransformComp[KIND::RENDER]->Update_Transform();

	if (m_eState >= ANIMSTATE_START)
		m_pTextrueComp[m_eState]->Update_Frame(_fDeltaTime, &m_iCurrFrame);

	return GAMEOBJECT::NOEVENT;
}

int CMouse::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_MOUSE, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CMouse::Render_UI()
{
	const D3DXIMAGE_INFO* pTexInfo = m_pTextrueComp[m_eState]->Get_TexInfo(m_iCurrFrame);
	if (nullptr == pTexInfo)
		return E_FAIL;

	_vec3 vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };

	m_pSprite->SetTransform(&m_pTransformComp[KIND::RENDER]->Get_Desc().matWorld);
	m_pSprite->Draw( (LPDIRECT3DTEXTURE9)m_pTextrueComp[m_eState]->GetTexture(m_iCurrFrame),
		nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255) );

	return S_OK;
}

CMouse * CMouse::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	CMouse* pInstance = new CMouse(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create Mouse", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMouse::Clone_GameObject(void * _pArg)
{
	CMouse* pInstance = new CMouse(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone Mouse", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMouse::Free()
{
	for(_uint iCnt = 0; iCnt < KIND_END; ++iCnt)
		Safe_Release(m_pTransformComp[iCnt]);

	for (_uint iCnt = 0; iCnt < STATE_END; ++iCnt)
		Safe_Release(m_pTextrueComp[iCnt]);

	CUIObject::Free();
}