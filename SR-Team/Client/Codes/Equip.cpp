#include "stdafx.h"
#include "Mouse.h"
#include "..\Headers\Equip.h"

#include "DataManager.h"
#include "Player.h"
#include "Sound_Manager.h"
USING(Client)


CEquip::CEquip(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < EQUIP_END; ++i)
	{
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}

	for (_uint i = 0; i < ITEMSORT_END; ++i)
	{
		m_pStatItem[i] = nullptr;
		m_pTextureItem[i] = nullptr;
	}
}

CEquip::CEquip(const CEquip & other)
	: CUIObject(other)
{
}

void CEquip::Equip_Item(eITEM_SORT eSort, const wstring& strItemTag/*INVEN_ITEM & _tItem*/)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return;
	CDataManager* pItem = (CDataManager*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Item");
	if (pItem == nullptr)
		return;

	// 장비 장착
	if (m_pStatItem[eSort])
		m_pStatItem[eSort] = nullptr;
	if (m_pTextureItem[eSort])
		m_pTextureItem[eSort] = nullptr;

	m_pStatItem[eSort] = pItem->Get_ItemStat(strItemTag);
	m_pTextureItem[eSort] = pItem->Get_ItemInfo_Texture(strItemTag);

	m_bEquip[eSort] = true;
	StringCchPrintf(szEquipItemTag[eSort], _countof(szEquipItemTag[eSort]), strItemTag.c_str());
}

_bool CEquip::Check_IsItemEquip(const wstring & strItemTag)
{
	for (_int i = 0; i < ITEMSORT_END; i++)
	{
		if (m_bEquip[i])
		{
			if (!wcscmp(szEquipItemTag[i], strItemTag.c_str()))
				return true;
		}
	}

	return false;
}

void CEquip::Set_PlayerAtt(_int iAtt)
{
	m_pStatCom->Set_Att(iAtt);
}

void CEquip::Set_PlayerHp(_int iHP)
{
	m_pStatCom->Set_HP(-iHP);
	// iHp > iMaxHp 이면 iHp = iMaxHp 고정
	if (m_pStatCom->Get_Status().iHp >= m_pStatCom->Get_Status().iMaxHp)
		m_pStatCom->Set_HP(m_pStatCom->Get_Status().iHp - m_pStatCom->Get_Status().iMaxHp);
	if (m_pStatCom->Get_Status().iHp <= 0)
		m_pStatCom->Change_Hp(0);
}

void CEquip::Set_PlayerMP(_int iMP)
{
	m_pStatCom->Set_MP(-iMP);
	if (m_pStatCom->Get_Status().iMp >= m_pStatCom->Get_Status().iMaxMp)
		m_pStatCom->Set_MP(m_pStatCom->Get_Status().iMp - m_pStatCom->Get_Status().iMaxMp);
	if (m_pStatCom->Get_Status().iMp <= 0)
		m_pStatCom->Change_Mp(0);
}

HRESULT CEquip::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CEquip::Setup_GameObject(void * _pArg)
{
	// 12
	//m_pStatItem.resize(12);
	//m_pTextureItem.resize(12);
	m_pTransformItem.resize(12);
	if (FAILED(Add_Component()))
		return E_FAIL;

	// 변경
	m_vPos = { 500.f, 400.f, 0.f };

	// 변경
	m_pTransformCom[EQUIP_WND]->Set_Position(m_vPos);
	m_pTransformCom[EQUIP_EQUIPMENT]->Set_Position(
		_vec3(m_vPos.x - 285.f, m_vPos.y, 0.f));
	m_pTransformCom[EQUIP_INFO]->Set_Position(
		_vec3(m_vPos.x - 180.f, m_vPos.y - 265.f, 0.f));

	// 변경
	for (_uint i = 0; i < 6; ++i)
	{
		_vec3 vPos = {};
		vPos.x = m_vPos.x - 430.f;
		vPos.y = (i * 60.f) + (m_vPos.y - 130.f);
		vPos.z = 0.f;
		m_pTransformItem[i]->Set_Position(vPos);
	}

	// 변경
	for (_uint i = 6, j = 0; i < ITEMSORT_END; ++i, ++j)
	{
		_vec3 vPos = {};
		vPos.x = m_vPos.x - 138.f;
		vPos.y = (j * 60.f) + (m_vPos.y - 130.f);
		vPos.z = 0.f;
		m_pTransformItem[i]->Set_Position(vPos);
	}


	return S_OK;
}

_int CEquip::Update_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;
	CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Player");
	if (pPlayer == nullptr)
		return E_FAIL;
	CStatus* pPlayerStat = (CStatus*)pManagement->Get_Component(
		pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Stat");
	if (pPlayerStat == nullptr)
		return GAMEOBJECT::WARN;

	if (pManagement->Key_Down('U'))
	{
		CSoundManager::Get_Instance()->PlayUI(L"OpenWnd.wav");
		m_bRender = !m_bRender;
	}

	if (FAILED(Count_Stat()))
		return GAMEOBJECT::WARN;
	if (FAILED(UnEquip_Item()))
		return GAMEOBJECT::WARN;

	// 치트
	if (pManagement->Key_Down(VK_RETURN))
	{
		pPlayerStat->Change_Hp(100);
		pPlayerStat->Change_Mp(100);
	}

	for (_uint i = 0; i < EQUIP_END; ++i)
	{
		m_pTransformCom[i]->Update_Transform();
	}

	for (_int i = 0; i < ITEMSORT_END; i++)
	{
		if(nullptr != m_pTransformItem[i])
			m_pTransformItem[i]->Update_Transform();
	}

	return GAMEOBJECT::NOEVENT;
}

_int CEquip::LateUpdate_GameObject(_float _fDeltaTime)
{
	_int i = 0;

	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CEquip::Render_UI()
{
	_int k = 0;
	if (m_bRender)
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		for (_uint i = 0; i < EQUIP_END; ++i)
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureCom[i]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			_vec3 vPos = m_pTransformCom[i]->Get_Desc().vPosition;
			D3DXMATRIX matWorld, matTrans, matScale;

			m_tEquipWndCollRt[i].left = (LONG)(vPos.x - vCenter.x);
			m_tEquipWndCollRt[i].right = (LONG)(vPos.x + vCenter.x);
			m_tEquipWndCollRt[i].top = (LONG)(vPos.y - vCenter.y);
			m_tEquipWndCollRt[i].bottom = (LONG)(vPos.y + vCenter.y);

			if (i == EQUIP_INFO)
				D3DXMatrixScaling(&matScale, 0.6f, 0.6f, 0.f);
			else
				D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);
			matWorld = matScale * matTrans;

			m_pSprite->SetTransform(&matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureCom[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (FAILED(Render_EquipItem()))
			return E_FAIL;
		if (FAILED(Render_Stat()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEquip::Render_EquipItem()
{
	for (_uint i = 0; i < ITEMSORT_END; ++i)
	{
		if (m_bEquip[i] && (m_pTextureItem[i] != nullptr))
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureItem[i]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			D3DXMATRIX matTrans, matScale, matWorld;
			_vec3 vPos = m_pTransformItem[i]->Get_Desc().vPosition;

			// 변경
			D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
			matWorld = matScale * matTrans;

			m_tEquipItemCollRt[i].left = (LONG)(vPos.x - vCenter.x);
			m_tEquipItemCollRt[i].right = (LONG)(vPos.x + vCenter.x);
			m_tEquipItemCollRt[i].top = (LONG)(vPos.y - vCenter.y);
			m_tEquipItemCollRt[i].bottom = (LONG)(vPos.y + vCenter.y);

			m_pSprite->SetTransform(&matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureItem[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	return S_OK;
}

HRESULT CEquip::Render_Stat()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Player");
	if (pPlayer == nullptr)
		return E_FAIL;
	CStatus* pPlayerStat = (CStatus*)pManagement->Get_Component(
		pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Stat");
	if (pPlayerStat == nullptr)
		return GAMEOBJECT::WARN;

	// 공격력
	TCHAR szBuff2[MAX_PATH] = L"";
	_matrix matScale, matTrans, matWorld;
	_vec3 vPos = m_pTransformCom[EQUIP_EQUIPMENT]->Get_Desc().vPosition;
	D3DXMatrixIdentity(&matWorld);

	StringCchPrintf(szBuff2, _countof(szBuff2), L"%d ~ %d", 
		pPlayerStat->Get_Status().iMinAtt, pPlayerStat->Get_Status().iMaxAtt);

	D3DXMatrixScaling(&matScale, 1.2f, 1.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, vPos.x + 300.f, vPos.y - 160.f, 0.f);
	matWorld = matScale * matTrans;

	m_pSprite->SetTransform(&matWorld);
	m_pFont->DrawTextW(m_pSprite, szBuff2, lstrlen(szBuff2),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 방어력
	StringCchPrintf(szBuff2, _countof(szBuff2), L"%d",
		pPlayerStat->Get_Status().iDef);

	D3DXMatrixScaling(&matScale, 1.2f, 1.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, vPos.x + 300.f, vPos.y - 90.f, 0.f);
	matWorld = matScale * matTrans;

	m_pSprite->SetTransform(&matWorld);
	m_pFont->DrawTextW(m_pSprite, szBuff2, lstrlen(szBuff2),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 치명 확률
	StringCchPrintf(szBuff2, _countof(szBuff2), L"%d",
		pPlayerStat->Get_Status().iCriticalRate);

	D3DXMatrixScaling(&matScale, 1.2f, 1.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, vPos.x + 300.f, vPos.y - 20.f, 0.f);
	matWorld = matScale * matTrans;

	m_pSprite->SetTransform(&matWorld);
	m_pFont->DrawTextW(m_pSprite, szBuff2, lstrlen(szBuff2),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 치명 피해
	StringCchPrintf(szBuff2, _countof(szBuff2), L"%d",
		pPlayerStat->Get_Status().iCriticalRate);

	D3DXMatrixScaling(&matScale, 1.2f, 1.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, vPos.x + 300.f, vPos.y + 50.f, 0.f);
	matWorld = matScale * matTrans;

	m_pSprite->SetTransform(&matWorld);
	m_pFont->DrawTextW(m_pSprite, szBuff2, lstrlen(szBuff2),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	// HP
	StringCchPrintf(szBuff2, _countof(szBuff2), L"%d / %d",
		pPlayerStat->Get_Status().iHp, pPlayerStat->Get_Status().iMaxHp);

	D3DXMatrixScaling(&matScale, 1.2f, 1.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, vPos.x + 250.f, vPos.y + 110.f, 0.f);
	matWorld = matScale * matTrans;

	m_pSprite->SetTransform(&matWorld);
	m_pFont->DrawTextW(m_pSprite, szBuff2, lstrlen(szBuff2),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	// MP
	StringCchPrintf(szBuff2, _countof(szBuff2), L"%d / %d",
		pPlayerStat->Get_Status().iMp, pPlayerStat->Get_Status().iMaxMp);

	D3DXMatrixScaling(&matScale, 1.2f, 1.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, vPos.x + 250.f, vPos.y + 175.f, 0.f);
	matWorld = matScale * matTrans;

	m_pSprite->SetTransform(&matWorld);
	m_pFont->DrawTextW(m_pSprite, szBuff2, lstrlen(szBuff2),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	//// 레벨
	//StringCchPrintf(szBuff2, sizeof(TCHAR) * MAX_PATH, L"%d",
	//	m_pStatCom->Get_Status().iLevel);

	//D3DXMatrixScaling(&matScale, 1.2f, 1.7f, 0.f);
	//D3DXMatrixTranslation(&matTrans, vPos.x + 25.f, vPos.y - 220.f, 0.f);
	//matWorld = matScale * matTrans;

	//m_pSprite->SetTransform(&matWorld);
	//m_pFont->DrawTextW(m_pSprite, szBuff2, lstrlen(szBuff2),
	//	nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

	return S_OK;
}

HRESULT CEquip::Count_Stat()
{
	_int iMinAtt = 0, iMaxAtt = 0, iDef = 0, iCirRate = 0, iCriHit = 0;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Player");
	if (pPlayer == nullptr)
		return E_FAIL;
	CStatus* pPlayerStat = (CStatus*)pManagement->Get_Component(
		pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Stat");
	if (pPlayerStat == nullptr)
		return GAMEOBJECT::WARN;

	for (_uint i = 0; i < ITEMSORT_END; ++i)
	{
		if (m_bEquip[i])
		{
			iMinAtt += m_pStatItem[i]->Get_Status().iMinAtt;
			iMaxAtt += m_pStatItem[i]->Get_Status().iMaxAtt;
			iDef += m_pStatItem[i]->Get_Status().iDef;
			iCirRate += m_pStatItem[i]->Get_Status().iCriticalChance;
			iCriHit += m_pStatItem[i]->Get_Status().iCriticalRate;
		}
	}

	CStatus::STAT	tStat;
	tStat.iMaxAtt = iMinAtt;
	tStat.iMaxAtt = iMaxAtt;
	tStat.iDef = iDef;
	tStat.iCriticalChance = iCirRate;
	tStat.iCriticalRate = iCriHit;

	tStat.iHp = m_pStatCom->Get_Status().iHp;
	tStat.iMp = m_pStatCom->Get_Status().iMp;
	tStat.iMaxHp = m_pStatCom->Get_Status().iMaxHp;
	tStat.iMaxMp = m_pStatCom->Get_Status().iMaxMp;

	// 플레이어 스탯 변경
	pPlayerStat->Set_EquipStat(tStat);
	m_pStatCom->Set_Status(tStat);

	return S_OK;
}

HRESULT CEquip::UnEquip_Item()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (nullptr == pMouse)
		return E_FAIL;

	for (_uint i = 0; i < ITEMSORT_END; ++i)
	{
		if (m_bEquip[i])
		{
			if (pManagement->Key_Pressing(VK_LBUTTON))
			{
				if (PtInRect(&m_tEquipItemCollRt[i], pMouse->Get_Point()))
				{
					m_bEquip[i] = false;
				}
			}
		}
	}

	return S_OK;
}

HRESULT CEquip::Add_Component()
{
	for (_uint i = 0; i < EQUIP_END; ++i)
	{
		// 2. Transform
		TCHAR szTransform[MAX_STR] = L"";
		wsprintf(szTransform, L"Com_Transform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformCom[i])))
			return E_FAIL;

		// 3. Texture
		TCHAR szTexture[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		if (i == EQUIP_START)
			wsprintf(szTextureName, L"Component_Texture_Equip_Stat");
		else if (i == EQUIP_EQUIPMENT)
			wsprintf(szTextureName, L"Component_Texture_Equip_Equipment");
		else if (i == EQUIP_INFO)
			wsprintf(szTextureName, L"Component_Texture_Equip_Info");
		wsprintf(szTexture, L"Com_Texture%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	for (_uint i = 0; i < ITEMSORT_END; i++)
	{
		// 2. Transform
		TCHAR szStat2[MAX_STR] = L"";
		CTransform::TRANSFORM_DESC tTransform;
		wsprintf(szStat2, L"Com_ItemTransform%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szStat2, (CComponent**)&m_pTransformItem[i])))
			return E_FAIL;
	}

	CStatus::STAT	tStat;
	tStat.iMaxHp = 200;
	tStat.iMaxMp = 200;
	tStat.iHp = 10;
	tStat.iMp = 10;
	tStat.iLevel = 1;

	if (FAILED(CGameObject::Add_Component(
		SCENE_STATIC, L"Component_Status",
		L"Com_PlayerStat", (CComponent**)&m_pStatCom, &tStat)))
		return E_FAIL;

	return S_OK;
}

CGameObject * CEquip::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CEquip* pInstance = new CEquip(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CEquip", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEquip::Clone_GameObject(void * _pArg)
{
	CEquip* pInstance = new CEquip(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CEquip", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEquip::Free()
{
	Safe_Release(m_pStatCom);

	for (_uint i = 0; i < EQUIP_END; ++i)
	{
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pTextureCom[i]);
	}

	//for (auto& pItem : m_pStatItem)
	//	Safe_Release(pItem);
	//m_pStatItem.clear();

	//for (auto& pItem : m_pTextureItem)
	//	Safe_Release(pItem);
	//m_pTextureItem.clear();

	for (auto& pItem : m_pTransformItem)
		Safe_Release(pItem);
	m_pTransformItem.clear();

	for (_uint i = 0; i < ITEMSORT_END; i++)
	{
		//Safe_Release(m_pStatItem[i]);
		//Safe_Release(m_pTextureItem[i]);
		//Safe_Release(m_pTransformItem[i]);
	}

	CUIObject::Free();
}
