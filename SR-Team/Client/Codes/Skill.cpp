#include "stdafx.h"
#include "Mouse.h"
#include "..\Headers\Skill.h"

#include "MainUI.h"
USING(Client)


CSkill::CSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < SKILL_END; i++)
	{
		m_pTransformWnd[i] = nullptr;
		m_pTextureWnd[i] = nullptr;
	}

	for (_uint i = 0; i < ACTIVE_SKILL_END; i++)
	{
		m_pTextureSkillInfo[i] = nullptr;
	}
}

CSkill::CSkill(const CSkill & other)
	: CUIObject(other)
{
}

HRESULT CSkill::Setup_GameObject_Prototype()
{

	return S_OK;
}

HRESULT CSkill::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_vCenterPos = { 500.f, 370.f, 0.f };

	m_pTransformWnd[SKILL_ACTIVE_WND]->Set_Position(m_vCenterPos);
	m_pTransformWnd[SKILL_PASSIVE_WND]->Set_Position(
		_vec3(m_vCenterPos.x, m_vCenterPos.y - 100.f, 0.f));

	_vec3 vActiveCenterPos = { m_vCenterPos.x - 65.f, m_vCenterPos.y - 220.f, 0.f };
	_vec3 vPassiveCenterPos = { m_vCenterPos.x + 60.f, m_vCenterPos.y - 220.f, 0.f };

	m_tSkillSortCollRt[ACTIVE].left = (LONG)(vActiveCenterPos.x - 60.f);
	m_tSkillSortCollRt[ACTIVE].right = (LONG)(vActiveCenterPos.x + 60.f);
	m_tSkillSortCollRt[ACTIVE].top = (LONG)(vActiveCenterPos.y - 30.f);
	m_tSkillSortCollRt[ACTIVE].bottom = (LONG)(vActiveCenterPos.y + 30.f);

	m_tSkillSortCollRt[PASSIVE].left = (LONG)(vPassiveCenterPos.x - 60.f);
	m_tSkillSortCollRt[PASSIVE].right = (LONG)(vPassiveCenterPos.x + 60.f);
	m_tSkillSortCollRt[PASSIVE].top = (LONG)(vPassiveCenterPos.y - 30.f);
	m_tSkillSortCollRt[PASSIVE].bottom = (LONG)(vPassiveCenterPos.y + 30.f);

	for (_uint i = 0; i < ACTIVE_SKILL_END; i++)
	{
		_vec3 vPos = { 0.f, 0.f, 0.f };
		_vec3 vSize = { 8.f, 8.f, 0.f };
		vPos.x = m_vCenterPos.x - 95.f;
		//vPos.y = (i * 73.f) + (m_vCenterPos.y - 140.f);
		vPos.y = (i * 70.f) + (m_vCenterPos.y - 210.f);

		m_pTransformSkillInfo[i]->Set_Position(vPos);

		m_tActiveCollRt[i].left = (LONG)(vPos.x - vSize.x);
		m_tActiveCollRt[i].right = (LONG)(vPos.x + vSize.x);
		m_tActiveCollRt[i].top = (LONG)(vPos.y - vSize.y);
		m_tActiveCollRt[i].bottom = (LONG)(vPos.y + vSize.y);
	}

	return S_OK;
}

_int CSkill::Update_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (pManagement->Key_Down('Y'))
		m_bRender = !m_bRender;

	if (m_bRender)
	{
		if (FAILED(Set_SkillSort()))
			return GAMEOBJECT::WARN;
		if (FAILED(Show_ActiveSkill_Info()))
			return GAMEOBJECT::WARN;
		if (FAILED(Move_To_QuickSlot()))
			return GAMEOBJECT::WARN;
			
		for (_uint i = 0; i < SKILL_END; ++i)
			m_pTransformWnd[i]->Update_Transform();
		for (_uint i = 0; i < ACTIVE_SKILL_END; i++)
			m_pTransformSkillInfo[i]->Update_Transform();
	}

	return GAMEOBJECT::NOEVENT;
}

_int CSkill::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CSkill::Render_UI()
{
	if (m_bRender)
	{
		_matrix matTrans, matWorld;
		_vec3 vPos = { 0.f, 0.f, 0.f };
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		// 스킬 윈도우 그리기
		for (_uint i = (_uint)m_eSkillSort; i < SKILL_END;)
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureWnd[i]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			vPos = m_pTransformWnd[i]->Get_Desc().vPosition;

			m_tWndCollRt[i].left = (LONG)(vPos.x - vCenter.x);
			m_tWndCollRt[i].right = (LONG)(vPos.x + vCenter.x);
			m_tWndCollRt[i].top = (LONG)(vPos.y - vCenter.y);
			m_tWndCollRt[i].bottom = (LONG)(vPos.y + vCenter.y);

			m_pSprite->SetTransform(&m_pTransformWnd[i]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureWnd[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			if (i == ACTIVE)
				i += 2;
			else
				++i;
		}

		if (m_bRenderSkillInfo)
		{
			D3DXMATRIX matTrans, matWorld;
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureSkillInfo[m_iSkillInfoIndex]->Get_TexInfo(0);
			if (nullptr == pTexInfo)
				return E_FAIL;

			_vec3 vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };
			m_vSkillInfoPos.x += 220.f;

			D3DXMatrixTranslation(&matTrans, m_vSkillInfoPos.x, m_vSkillInfoPos.y, 0.f);
			matWorld = matTrans;

			m_pSprite->SetTransform(&matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureSkillInfo[m_iSkillInfoIndex]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		// 스킬 아이콘 출력
		// -> 스킬 아이콘 충돌 렉트 알아보려고
		for (_uint i = 0; i < ACTIVE_SKILL_END; i++)
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureSkillIcon[i]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			vPos = m_pTransformSkillInfo[i]->Get_Desc().vPosition;

			m_tActiveCollRt[i].left = (LONG)(vPos.x - vCenter.x);
			m_tActiveCollRt[i].right = (LONG)(vPos.x + vCenter.x);
			m_tActiveCollRt[i].top = (LONG)(vPos.y - vCenter.y);
			m_tActiveCollRt[i].bottom = (LONG)(vPos.y + vCenter.y);

			m_pSprite->SetTransform(&m_pTransformSkillInfo[i]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureSkillIcon[i]->GetTexture(0),
				&m_tActiveCollRt[i], &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	return S_OK;
}

HRESULT CSkill::Set_SkillSort()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (pManagement->Key_Pressing(VK_LBUTTON))
	{
		CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");

		_int k = 0;
		if (PtInRect(&m_tSkillSortCollRt[ACTIVE], pMouse->Get_Point()))
			m_eSkillSort = ACTIVE;
		else if (PtInRect(&m_tSkillSortCollRt[PASSIVE], pMouse->Get_Point()))
			m_eSkillSort = PASSIVE;
	}

	return S_OK;
}

HRESULT CSkill::Show_ActiveSkill_Info()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");

	for (_uint i = 0; i < ACTIVE_SKILL_END; i++)
	{
		if (PtInRect(&m_tActiveCollRt[i], pMouse->Get_Point()))
		{
			m_bRenderSkillInfo = true;
			m_iSkillInfoIndex = i;
			m_vSkillInfoPos.x = (_float)pMouse->Get_Point().x;
			m_vSkillInfoPos.y = (_float)pMouse->Get_Point().y;
			return S_OK;
		}
		else
			m_bRenderSkillInfo = false;
	}

	return S_OK;
}

HRESULT CSkill::Move_To_QuickSlot()
{
	_int iIndex = 0;
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CMainUI* pMainUI = (CMainUI*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 0);

	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (nullptr == pMouse)
		return E_FAIL;

	for (_uint i = 0; i < ACTIVE_SKILL_END; i++)
	{
		if (pManagement->Key_Pressing(VK_LBUTTON))
		{
			if (PtInRect(&m_tActiveCollRt[i], pMouse->Get_Point()))
			{
				_int k = 0;
				if (FAILED(pMainUI->Get_QuickSlotSkill(i)))
					return E_FAIL;
				return S_OK;

			}
		}
		
	}

	return S_OK;
}

HRESULT CSkill::Add_Component()
{
	for (_uint i = 0; i < SKILL_END; ++i)
	{
		// 2. Transform
		TCHAR szTransform[MAX_PATH] = L"";
		StringCchPrintf(szTransform, _countof(szTransform),
			L"Com_Transform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformWnd[i])))
			return E_FAIL;

		// 3. Texture
		TCHAR szTexture[MAX_PATH] = L"";
		TCHAR szTextureName[MAX_PATH] = L"";
		wstring strTexture = L"";
		wstring strTextureName = L"";

		StringCchPrintf(szTexture, _countof(szTexture),
			L"Com_Texture%d", i);
		strTexture = szTexture;
		if (i == SKILL_ACTIVE_WND)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Skill_ActiveWnd");
		if (i == SKILL_PASSIVE_WND)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_Skill_PassiveWnd");
		strTextureName = szTextureName;

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, strTextureName
			, strTexture, (CComponent**)&m_pTextureWnd[i])))
			return E_FAIL;
	}

	// 변경(추가)
	TCHAR szTextureName[][MIN_STR] =
	{
		L"Component_Texture_SkillInfo_FlameWave",
		L"Component_Texture_SkillInfo_IceSpear",
		L"Component_Texture_SkillInfo_ThunderStorm",
		L"Component_Texture_SkillInfo_ManaDrift",
		L"Component_Texture_SkillInfo_Blind",
		L"Component_Texture_SkillInfo_MagicArmor",
		L"Component_Texture_SkillInfo_IceStrike",
		L"Component_Texture_SkillInfo_EnergyExploitiation"
	};
	TCHAR szTexture[MIN_STR] = L"Com_SkillInfoTexture%d";
	TCHAR szCombine[MIN_STR] = L"";

	TCHAR szTransform[MIN_STR] = L"";

	TCHAR szTextureIconName[][MIN_STR] =
	{
		L"Component_Texture_SkillIcon_FlameWave",
		L"Component_Texture_SkillIcon_IceSpear",
		L"Component_Texture_SkillIcon_ThunderStorm",
		L"Component_Texture_SkillIcon_ManaDrift",
		L"Component_Texture_SkillIcon_Blind",
		L"Component_Texture_SkillIcon_MagicArmor",
		L"Component_Texture_SkillIcon_IceStrike",
		L"Component_Texture_SkillIcon_EnergyExplotiation"
	};
	TCHAR szTextureIcon[MIN_STR] = L"Com_SkillIcon%d";
	TCHAR szTextureIconCombine[MIN_STR] = L"";

	// 변경(추가)
	for (_uint i = 0; i < ACTIVE_SKILL_END; i++)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureName[i], szCombine, (CComponent**)&m_pTextureSkillInfo[i])))
			return E_FAIL;

		StringCchPrintf(szTextureIconCombine, _countof(szTextureIconCombine), szTextureIcon, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureIconName[i], szTextureIconCombine, (CComponent**)&m_pTextureSkillIcon[i])))
			return E_FAIL;

		StringCchPrintf(szTransform, _countof(szTransform),
			L"Com_SkillInfoTransform%d", i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC,
			L"Component_Transform", szTransform, (CComponent**)&m_pTransformSkillInfo[i])))
			return E_FAIL;
	}

	return S_OK;
}

CGameObject * CSkill::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSkill* pInstance = new CSkill(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkill::Clone_GameObject(void * _pArg)
{
	CSkill* pInstance = new CSkill(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill::Free()
{
	for (_uint i = 0; i < SKILL_END; i++)
	{
		Safe_Release(m_pTransformWnd[i]);
		Safe_Release(m_pTextureWnd[i]);
	}

	for (_uint i = 0; i < ACTIVE_SKILL_END; i++)
	{
		Safe_Release(m_pTextureSkillInfo[i]);
		Safe_Release(m_pTransformSkillInfo[i]);
		Safe_Release(m_pTextureSkillIcon[i]);
	}

	CUIObject::Free();
}
