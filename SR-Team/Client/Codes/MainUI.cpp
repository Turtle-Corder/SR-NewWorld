#include "stdafx.h"
#include "UICamera.h"
#include "DataManager.h"
#include "Inventory.h"
#include "SkillInven.h"
#include "ItemInventory.h"
#include "Equip.h"
#include "Player.h"
#include "Status.h"
#include "..\Headers\MainUI.h"
#include "Mouse.h"

USING(Client)

CMainUI::CMainUI(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < MAINUI_END; ++i)
	{
		m_pVIBufferCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}

	for (_uint i = 0; i < 8; i++)
	{
		m_pTransformLeftSlot[i] = nullptr;
		m_pTransformRightSlot[i] = nullptr;

		m_pTextureLeftQuickSlot[i] = nullptr;
		m_pTextureRightQuickSlot[i] = nullptr;

		m_pLeftSlotItem[i] = nullptr;
		m_pRightSlotItem[i] = nullptr;
	}
}

CMainUI::CMainUI(const CMainUI & _rOther)
	: CUIObject(_rOther)
{
}

HRESULT CMainUI::Get_QuickSlotItem(INVEN_ITEM * pItem)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;

	CDataManager* pItems = (CDataManager*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Item");

	m_pMovingItem = pItem;

	m_bRender_GoingItem = true;

	_int p = 0;

	if (m_pTexture_GoingItem)
		m_pTexture_GoingItem = nullptr;
	m_pTexture_GoingItem = pItems->Get_ItemInfo_Texture(m_pMovingItem->szItemTag);

	return S_OK;
}

HRESULT CMainUI::Delete_Potion(INVEN_ITEM * pItem)
{
	for (_uint i = 0; i < 8; i++)
	{
		if (m_pRightSlotItem[i] != nullptr)
		{
			if (!wcscmp(pItem->szItemTag, m_pRightSlotItem[i]->szItemTag))
			{
				m_pRightSlotItem[i]->iCnt = 0;
				return S_OK;
			}
		}
	}

	return S_OK;
}

HRESULT CMainUI::Get_QuickSlotSkill(_int iSkillID)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CDataManager* pItems = (CDataManager*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Item");

	// 받아온 스킬 아이디로 아이템 클래스에서 스킬 아이콘 객체를 받아온다
	m_pMovingItem = pItems->Get_ActiveSkillIcon(iSkillID);
	if (m_pMovingItem == nullptr)
		return E_FAIL;

	m_bRender_GoingItem = true;

	if (m_pTexture_GoingItem)
		m_pTexture_GoingItem = nullptr;
	m_pTexture_GoingItem = pItems->Get_ItemInfo_Texture(m_pMovingItem->szItemTag);
	if (m_pTexture_GoingItem == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CMainUI::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMainUI::Setup_GameObject(void * pArg)
{
	_vec3 vPos = {};
	if (FAILED(Add_Component()))
		return E_FAIL;

	for (_uint i = 0; i < 8; i++)
	{
		m_pTextureLeftQuickSlot[i] = nullptr;
		m_pTextureRightQuickSlot[i] = nullptr;

		m_pLeftSlotItem[i] = nullptr;
		m_pRightSlotItem[i] = nullptr;
	}

	m_pTransformCom[MAINUI_MAIN]->Set_Position(m_vMainPos);

	// 변경
	vPos = { m_vMainPos.x - 41.f, m_vMainPos.y - 15.f, 0.f };
	m_pTransformCom[MAINUI_HP]->Set_Position(vPos);

	vPos = { m_vMainPos.x + 42.f, m_vMainPos.y - 15.f, 0.f };
	m_pTransformCom[MAINUI_MP]->Set_Position(vPos);

	vPos = { m_vMainPos.x - 500.f, m_vMainPos.y, 0.f };
	m_vLeftSlotPos = vPos;
	m_pTransformCom[MAINUI_QUICKSLOT_LFFT]->Set_Position(vPos);

	vPos = { m_vMainPos.x + 500.f, m_vMainPos.y, 0.f };
	m_vRightSlotPos = vPos;
	m_pTransformCom[MAINUI_QUICKSLOT_RIGHT]->Set_Position(vPos);


	// 왼쪽 퀵슬롯 충돌 렉트
	// 변경
	for (_uint i = 0; i < 4; ++i)
	{
		_vec3 vPos = {};
		vPos.x = (i * 78.f) + (m_vLeftSlotPos.x - 155.f);
		vPos.y = 710.f;
		vPos.z = 0.f;

		m_pTransformLeftSlot[i]->Set_Position(vPos);

		m_tLeftSlotCollRt[i].left = (LONG)(vPos.x - 35.f);
		m_tLeftSlotCollRt[i].right = (LONG)(vPos.x + 35.f);
		m_tLeftSlotCollRt[i].top = (LONG)(vPos.y - 35.f);
		m_tLeftSlotCollRt[i].bottom = (LONG)(vPos.y + 35.f);
	}
	// 변경
	for (_uint i = 4, j = 0; i < 8; ++i, ++j)
	{
		_vec3 vPos = {};
		vPos.x = (j * 78.f) + (m_vLeftSlotPos.x - 130.f);
		vPos.y = 790.f;
		vPos.z = 0.f;

		m_pTransformLeftSlot[i]->Set_Position(vPos);

		m_tLeftSlotCollRt[i].left = (LONG)(vPos.x - 35.f);
		m_tLeftSlotCollRt[i].right = (LONG)(vPos.x + 35.f);
		m_tLeftSlotCollRt[i].top = (LONG)(vPos.y - 35.f);
		m_tLeftSlotCollRt[i].bottom = (LONG)(vPos.y + 35.f);
	}

	// 오른쪽 퀵슬롯 위치
	// 변경
	for (_uint i = 0; i < 4; ++i)
	{
		_vec3 vPos = {};
		vPos.x = (i * 78.f) + (m_vRightSlotPos.x - 155.f);
		vPos.y = 710.f;
		vPos.z = 0.f;

		m_pTransformRightSlot[i]->Set_Position(vPos);

		m_tRightSlotCollRt[i].left = (LONG)(vPos.x - 32.f);
		m_tRightSlotCollRt[i].right = (LONG)(vPos.x + 32.f);
		m_tRightSlotCollRt[i].top = (LONG)(vPos.y - 32.f);
		m_tRightSlotCollRt[i].bottom = (LONG)(vPos.y + 32.f);
	}
	// 변경
	for (_uint i = 4, j = 0; i < 8; ++i, ++j)
	{
		_vec3 vPos = {};
		vPos.x = (j * 78.f) + (m_vRightSlotPos.x - 130.f);
		vPos.y = 790.f;
		vPos.z = 0.f;

		m_pTransformRightSlot[i]->Set_Position(vPos);

		m_tRightSlotCollRt[i].left = (LONG)(vPos.x - 32.f);
		m_tRightSlotCollRt[i].right = (LONG)(vPos.x + 32.f);
		m_tRightSlotCollRt[i].top = (LONG)(vPos.y - 32.f);
		m_tRightSlotCollRt[i].bottom = (LONG)(vPos.y + 32.f);
	}

	return S_OK;
}

int CMainUI::Update_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (nullptr == pMouse)
		return GAMEOBJECT::NOEVENT;

	if (FAILED(Change_SkillIconPos()))
		return GAMEOBJECT::WARN;
	if (FAILED(Change_ItemPos()))
		return GAMEOBJECT::WARN;

	if (m_bRender_GoingItem)
	{
		if (FAILED(Move_To_QuickSlot()))
			return GAMEOBJECT::WARN;
		if (FAILED(Check_LeftQuickSlot_Item()))
			return GAMEOBJECT::WARN;
		if (FAILED(Check_RightQuickSlot_Item()))
			return GAMEOBJECT::WARN;
	}

	if (FAILED(Set_SlotItem_Count()))
		return GAMEOBJECT::WARN;

#pragma region Move_MainUI
	if (pManagement->Key_Down('P'))
	{
		for (_uint i = 0; i < MAINUI_END; ++i)
			m_pTransformCom[i]->Set_Position(m_pTransformCom[i]->Get_Desc().vPosition - _vec3(0.f, 10.f, 0.f));
		for (_uint i = 0; i < 8; ++i)
		{
			m_pTransformLeftSlot[i]->Set_Position(m_pTransformLeftSlot[i]->Get_Desc().vPosition - _vec3(0.f, 10.f, 0.f));
			m_pTransformRightSlot[i]->Set_Position(m_pTransformRightSlot[i]->Get_Desc().vPosition - _vec3(0.f, 10.f, 0.f));
		}
	}

	if (pManagement->Key_Down('L'))
	{
		for (_uint i = 0; i < MAINUI_END; ++i)
			m_pTransformCom[i]->Set_Position(m_pTransformCom[i]->Get_Desc().vPosition + _vec3(0.f, 10.f, 0.f));
		for (_uint i = 0; i < 8; ++i)
		{
			m_pTransformLeftSlot[i]->Set_Position(m_pTransformLeftSlot[i]->Get_Desc().vPosition + _vec3(0.f, 10.f, 0.f));
			m_pTransformRightSlot[i]->Set_Position(m_pTransformRightSlot[i]->Get_Desc().vPosition + _vec3(0.f, 10.f, 0.f));
		}
	}

	for (_uint i = 0; i < MAINUI_END; ++i)
		m_pTransformCom[i]->Update_Transform();
	for (_uint i = 0; i < 8; i++)
	{
		m_pTransformLeftSlot[i]->Update_Transform();
		m_pTransformRightSlot[i]->Update_Transform();

		_vec3 vLeftPos = m_pTransformLeftSlot[i]->Get_Desc().vPosition;
		_vec3 vRightPos = m_pTransformRightSlot[i]->Get_Desc().vPosition;

		m_tLeftSlotCollRt[i].left = (LONG)(vLeftPos.x - 35.f);
		m_tLeftSlotCollRt[i].right = (LONG)(vLeftPos.x + 35.f);
		m_tLeftSlotCollRt[i].top = (LONG)(vLeftPos.y - 35.f);
		m_tLeftSlotCollRt[i].bottom = (LONG)(vLeftPos.y + 35.f);

		m_tRightSlotCollRt[i].left = (LONG)(vRightPos.x - 35.f);
		m_tRightSlotCollRt[i].right = (LONG)(vRightPos.x + 35.f);
		m_tRightSlotCollRt[i].top = (LONG)(vRightPos.y - 35.f);
		m_tRightSlotCollRt[i].bottom = (LONG)(vRightPos.y + 35.f);
	}
#pragma endregion

	return GAMEOBJECT::NOEVENT;
}

int CMainUI::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CMainUI::Render_UI()
{
	_matrix matScale, matTrans, matWorld;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	CPlayer* pEquip = (CPlayer*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Player");
	if (pEquip == nullptr)
		return E_FAIL;
	CStatus* pPlayerStat = (CStatus*)pManagement->Get_Component(
		pManagement->Get_CurrentSceneID(), L"Layer_Player", L"Com_Stat");

	for (_uint i = 0; i < MAINUI_END; ++i)
	{
		const D3DXIMAGE_INFO* pTexInfo = m_pTextureCom[i]->Get_TexInfo(0);
		_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
		_vec3 vUIPose = m_pTransformCom[i]->Get_Desc().vPosition;

		// - HP/MP 지연 감소
		// HP/MP 감소에 따른 RECT 변경 나중에 하기
		if (i == MAINUI_HP)
		{
			if (pPlayerStat->Get_Status().iHp <= 0)
				pPlayerStat->Change_Hp(0);

			m_tCollRt[i].left = 0;
			m_tCollRt[i].right = (LONG)(pTexInfo->Width);
			m_tCollRt[i].top = (LONG)((pTexInfo->Height - pPlayerStat->Get_Status().iHp * 2.2f));
			m_tCollRt[i].bottom = (LONG)(pTexInfo->Height);

			_vec3 vPos = { 0.f, 0.f, 0.f };
			vPos.y = (_float)(pTexInfo->Height - pPlayerStat->Get_Status().iHp * 2.2f);

			D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
			D3DXMatrixTranslation(&matTrans, vUIPose.x, vUIPose.y, 0.f);
			matWorld = matScale * matTrans;

			m_pSprite->SetTransform(&matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureCom[i]->GetTexture(0),
				&m_tCollRt[i], &vCenter, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else if (i == MAINUI_MP)
		{
			if (pPlayerStat->Get_Status().iMp <= 0)
				pPlayerStat->Change_Mp(0);

			m_tCollRt[i].left = 0;
			m_tCollRt[i].right = (LONG)(pTexInfo->Width);
			m_tCollRt[i].top = (LONG)((pTexInfo->Height - pPlayerStat->Get_Status().iMp * 2.2f));
			m_tCollRt[i].bottom = (LONG)(pTexInfo->Height);
			
			_vec3 vPos = { 0.f, 0.f, 0.f };
			vPos.y = (_float)(pTexInfo->Height - pPlayerStat->Get_Status().iMp * 2.2f);

			D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
			D3DXMatrixTranslation(&matTrans, vUIPose.x, vUIPose.y, 0.f);
			matWorld = matScale * matTrans;

			m_pSprite->SetTransform(&matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureCom[i]->GetTexture(0),
				&m_tCollRt[i], &vCenter, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else
		{
			m_tCollRt[i].left = 0;
			m_tCollRt[i].right = (LONG)(pTexInfo->Width);
			m_tCollRt[i].top = 0;
			m_tCollRt[i].bottom = (LONG)(pTexInfo->Height);

			m_pSprite->SetTransform(&m_pTransformCom[i]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureCom[i]->GetTexture(0),
				&m_tCollRt[i], &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

	}

	if (m_bRender_GoingItem)
		if (FAILED(Render_Item_GoingToQuickSlot()))
			return E_FAIL;
	if (FAILED(Render_QuickSlot_Item()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainUI::Move_To_QuickSlot()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;;

	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (nullptr == pMouse)
		return E_FAIL;

	m_vGoingItem_Pos.x = (_float)pMouse->Get_Point().x;
	m_vGoingItem_Pos.y = (_float)pMouse->Get_Point().y;
	m_vGoingItem_Pos.z = 0.f;

	m_tGoingItem_CollRt.left = (LONG)(m_vGoingItem_Pos.x - 20.f);
	m_tGoingItem_CollRt.right = (LONG)(m_vGoingItem_Pos.x + 20.f);
	m_tGoingItem_CollRt.top = (LONG)(m_vGoingItem_Pos.y - 20.f);
	m_tGoingItem_CollRt.bottom = (LONG)(m_vGoingItem_Pos.y + 20.f);


	return S_OK;
}

HRESULT CMainUI::Check_LeftQuickSlot_Item()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CSkillInven* pSkillInven = (CSkillInven*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 3);
	if (pSkillInven == nullptr)
		return E_FAIL;

	RECT rc = {};

	for (_uint i = 0; i < 8; i++)
	{
		// 왼쪽
		if (IntersectRect(&rc, &m_tLeftSlotCollRt[i], &m_tGoingItem_CollRt))
		{
			if (pManagement->Key_Up(VK_LBUTTON))
			{
				// 스킬 전용 슬롯
				if (m_pMovingItem->eSort == SKILL_ICON)
				{
					if (!m_bChange_SkillIconPos)
					{
						if (Check_Item_In_Slot())
						{
							// 초기화
							m_bRender_GoingItem = false;

							if (m_pTextureLeftQuickSlot[i])
								m_pTextureLeftQuickSlot[i] = nullptr;
							m_pTextureLeftQuickSlot[i] = m_pTexture_GoingItem;
							m_pTexture_GoingItem = nullptr;

							if (m_pLeftSlotItem[i])
								m_pLeftSlotItem[i] = nullptr;
							m_pLeftSlotItem[i] = m_pMovingItem;

							if (FAILED(pSkillInven->Set_SkillIndex(i, m_pMovingItem->eActiveID)))
								return E_FAIL;

							m_pMovingItem = nullptr;
						}
					}
					// 스킬 퀵슬롯 내 위치 이동
					else if (m_bChange_SkillIconPos)
					{
						// 초기화
						m_bRender_GoingItem = false;
						m_bChange_SkillIconPos = false;

						// 바꾸려는 자리가 nullptr이 아니면
						if (m_pTextureLeftQuickSlot[i] != nullptr)
							m_pTextureLeftQuickSlot[m_iBefore_SkillIconIndex] = m_pTextureLeftQuickSlot[i];
						else
							m_pTextureLeftQuickSlot[m_iBefore_SkillIconIndex] = nullptr;

						m_pTextureLeftQuickSlot[i] = m_pTexture_GoingItem;
						m_pTexture_GoingItem = nullptr;

						if (m_pLeftSlotItem[i] != nullptr)
							m_pLeftSlotItem[m_iBefore_SkillIconIndex] = m_pLeftSlotItem[i];
						else
							m_pLeftSlotItem[m_iBefore_SkillIconIndex] = nullptr;

						m_pLeftSlotItem[i] = m_pMovingItem;

						if (FAILED(pSkillInven->Set_SkillIndex(i, m_pMovingItem->eActiveID)))
							return E_FAIL;

						m_pMovingItem = nullptr;

						// 이전에 있던 위치에는 모두 비워주기
						//if (m_pTextureLeftQuickSlot[m_iBefore_SkillIconIndex])
						//	m_pTextureLeftQuickSlot[m_iBefore_SkillIconIndex] = nullptr;
						//if (m_pLeftSlotItem[m_iBefore_SkillIconIndex])
						//	m_pLeftSlotItem[m_iBefore_SkillIconIndex] = nullptr;
						m_iBefore_SkillIconIndex = -1;
					}
				}
				else
				{
					m_bRender_GoingItem = false;
					m_pMovingItem = nullptr;
					m_pTexture_GoingItem = nullptr;
				}
			}
		}
	}

	return S_OK;
}

HRESULT CMainUI::Check_RightQuickSlot_Item()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CItemInventory* pItemInven = (CItemInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 4);
	if (pItemInven == nullptr)
		return E_FAIL;

	RECT rc = {};

	for (_uint i = 0; i < 8; i++)
	{
		// 왼쪽
		if (IntersectRect(&rc, &m_tRightSlotCollRt[i], &m_tGoingItem_CollRt))
		{
			if (pManagement->Key_Up(VK_LBUTTON))
			{
				// 스킬 전용 슬롯
				if (m_pMovingItem->eSort == POTION)
				{
					if (!m_bChange_ItemPos)
					{
						if (Check_Item_In_Slot())
						{
							// 초기화
							m_bRender_GoingItem = false;

							if (m_pTextureRightQuickSlot[i])
								m_pTextureRightQuickSlot[i] = nullptr;
							m_pTextureRightQuickSlot[i] = m_pTexture_GoingItem;
							m_pTexture_GoingItem = nullptr;

							if (m_pRightSlotItem[i])
								m_pRightSlotItem[i] = nullptr;
							m_pRightSlotItem[i] = m_pMovingItem;

							if (m_pMovingItem->ePotionID != POTION_END && i <= 3)
								if (FAILED(pItemInven->Set_ItemIndex(i, m_pMovingItem->ePotionID)))
									return E_FAIL;
							m_pMovingItem = nullptr;
						}
					}
					// 스킬 퀵슬롯 내 위치 이동
					else if (m_bChange_ItemPos)
					{
						// 초기화
						m_bRender_GoingItem = false;
						m_bChange_ItemPos = false;

						// 바꾸려는 자리가 nullptr이 아니면
						if (m_pTextureRightQuickSlot[i] != nullptr)
							m_pTextureRightQuickSlot[m_iBefore_ItemIndex] = m_pTextureRightQuickSlot[i];
						else
							m_pTextureRightQuickSlot[m_iBefore_ItemIndex] = nullptr;

						m_pTextureRightQuickSlot[i] = m_pTexture_GoingItem;
						m_pTexture_GoingItem = nullptr;

						if (m_pRightSlotItem[i] != nullptr)
							m_pRightSlotItem[m_iBefore_ItemIndex] = m_pRightSlotItem[i];
						else
							m_pRightSlotItem[m_iBefore_ItemIndex] = nullptr;

						m_pRightSlotItem[i] = m_pMovingItem;

						if (m_pMovingItem->ePotionID != POTION_END && i <= 3)
							if (FAILED(pItemInven->Set_ItemIndex(i, m_pMovingItem->ePotionID)))
								return E_FAIL;

						m_pMovingItem = nullptr;

						// 이전에 있던 위치에는 모두 비워주기
						//if (m_pTextureLeftQuickSlot[m_iBefore_SkillIconIndex])
						//	m_pTextureLeftQuickSlot[m_iBefore_SkillIconIndex] = nullptr;
						//if (m_pLeftSlotItem[m_iBefore_SkillIconIndex])
						//	m_pLeftSlotItem[m_iBefore_SkillIconIndex] = nullptr;
						m_iBefore_ItemIndex = -1;
					}
				}
				else
				{
					m_bRender_GoingItem = false;
					m_pMovingItem = nullptr;
					m_pTexture_GoingItem = nullptr;
				}
			}
		}
	}

	return S_OK;
}

_bool CMainUI::Check_Item_In_Slot()
{
	// 현재 퀵슬롯에 해당 아이템이 있는지 없는지 검사
	for (_uint i = 0; i < 8; ++i)
	{
		if (m_pLeftSlotItem[i] != nullptr)
		{
			// 현재 퀵슬롯에 같은 이름의 아이템이 있으면
			if (!wcscmp(m_pMovingItem->szItemTag, m_pLeftSlotItem[i]->szItemTag))
			{
				PRINT_LOG(L"이미 장착하고 있는 아이템", LOG::CLIENT);
				return false;
			}
		}
		if (m_pRightSlotItem[i] != nullptr)
		{
			// 현재 퀵슬롯에 같은 이름의 아이템이 있으면
			if (!wcscmp(m_pMovingItem->szItemTag, m_pRightSlotItem[i]->szItemTag))
			{
				PRINT_LOG(L"이미 장착하고 있는 아이템", LOG::CLIENT);
				return false;
			}
		}
	}

	return true;
}

HRESULT CMainUI::Set_SlotItem_Count()
{
	CManagement* pManagement = CManagement::Get_Instance();
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");

	for (_uint i = 0; i < 8; ++i)
	{
		if (m_pRightSlotItem[i] != nullptr)
		{
			m_pRightSlotItem[i]->iCnt = pInven->Get_ItemCount(m_pRightSlotItem[i]->szItemTag);
			if (m_pRightSlotItem[i]->iCnt == 0 || m_pRightSlotItem[i]->iCnt == -1)
			{
				m_pTextureRightQuickSlot[i] = nullptr;
				m_pRightSlotItem[i] = nullptr;
			}
		}
	}

	return S_OK;
}

HRESULT CMainUI::Render_Item_GoingToQuickSlot()
{
	const D3DXIMAGE_INFO* pTexInfo = m_pTexture_GoingItem->Get_TexInfo(0);
	_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
	_vec3 vPos = m_vGoingItem_Pos;
	D3DXMATRIX matTrans, matWorld;

	m_tGoingItem_CollRt.left = (LONG)(vPos.x - vCenter.x);
	m_tGoingItem_CollRt.right = (LONG)(vPos.x + vCenter.x);
	m_tGoingItem_CollRt.top = (LONG)(vPos.y - vCenter.y);
	m_tGoingItem_CollRt.bottom = (LONG)(vPos.y + vCenter.y);

	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);
	matWorld = matTrans;

	m_pSprite->SetTransform(&matWorld);
	m_pSprite->Draw(
		(LPDIRECT3DTEXTURE9)m_pTexture_GoingItem->GetTexture(0),
		nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	return S_OK;
}

HRESULT CMainUI::Render_QuickSlot_Item()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Player");
	if (pPlayer == nullptr)
		return E_FAIL;
	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (pMouse == nullptr)
		return E_FAIL;

	CSkillInven* pSkillInven = (CSkillInven*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 3);
	if (pSkillInven == nullptr)
		return E_FAIL;

	CItemInventory* pItemInven = (CItemInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 4);
	if (pItemInven == nullptr)
		return E_FAIL;

	_int k = 0;
	_vec3 vCenter = { 0.f, 0.f, 0.f };
	_vec3 vPos = { 0.f, 0.f, 0.f };
	D3DXMATRIX matTrans, matWorld, matScale;


	for (_uint i = 0; i < 8; ++i)
	{
		if (m_pTextureLeftQuickSlot[i] != nullptr)	// 스킬
		{
			_bool bCheck = false;
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureLeftQuickSlot[i]->Get_TexInfo(0);
			vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			vPos = m_pTransformLeftSlot[i]->Get_Desc().vPosition;

			m_tGoingItem_CollRt.left = (LONG)(vPos.x - vCenter.x);
			m_tGoingItem_CollRt.right = (LONG)(vPos.x + vCenter.x);
			m_tGoingItem_CollRt.top = (LONG)(vPos.y - vCenter.y);
			m_tGoingItem_CollRt.bottom = (LONG)(vPos.y + vCenter.y);

			D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
			D3DXMatrixTranslation(&matTrans, vPos.x + 25.f, vPos.y + 25.f, 0.f);
			matWorld = matScale * matTrans;

			if (m_pLeftSlotItem[i]->eActiveID == ACTIVE_ENERGY_EXPLOTIATION)
				bCheck = pPlayer->IsOnBuff(CPlayer::ACTIVE_BUFF::BUFF_ATTACK);
			else if (m_pLeftSlotItem[i]->eActiveID == ACTIVE_MAGIC_ARMOR)
				bCheck = pPlayer->IsOnBuff(CPlayer::ACTIVE_BUFF::BUFF_SHIELD);
			else if (m_pLeftSlotItem[i]->eActiveID == ACTIVE_MANA_DRIFT)
				bCheck = pPlayer->IsOnBuff(CPlayer::ACTIVE_BUFF::BUFF_MANA);


			m_pSprite->SetTransform(&matWorld);
			// 현재 버프를 사용중이면 약간 투명하게 그린다
			if (bCheck)
			{
				m_pSprite->Draw(
					(LPDIRECT3DTEXTURE9)m_pTextureLeftQuickSlot[i]->GetTexture(0),
					nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(100, 255, 255, 255));
			}
			else
			{
				m_pSprite->Draw(
					(LPDIRECT3DTEXTURE9)m_pTextureLeftQuickSlot[i]->GetTexture(0),
					nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}


			//----------------------------------------------------------------------------------------------------
			// 쿨타임

			_float fProgress = 1.f - pSkillInven->Get_Progress(i);
			if (1.f == fProgress) continue;
			pTexInfo = m_pEmptyTexture->Get_TexInfo(0);
			vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

			RECT rc = { 0, 0, (LONG)pTexInfo->Width, (LONG)(pTexInfo->Height * fProgress) };
			m_pSprite->Draw((LPDIRECT3DTEXTURE9)m_pEmptyTexture->GetTexture(0), &rc, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (m_pTextureRightQuickSlot[i] != nullptr)		// 아이템
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureRightQuickSlot[i]->Get_TexInfo(0);
			vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			vPos = m_pTransformRightSlot[i]->Get_Desc().vPosition;

			m_tGoingItem_CollRt.left = (LONG)(vPos.x - vCenter.x);
			m_tGoingItem_CollRt.right = (LONG)(vPos.x + vCenter.x);
			m_tGoingItem_CollRt.top = (LONG)(vPos.y - vCenter.y);
			m_tGoingItem_CollRt.bottom = (LONG)(vPos.y + vCenter.y);

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
			D3DXMatrixTranslation(&matTrans, vPos.x + 25.f, vPos.y + 25.f, 0.f);
			matWorld = matScale * matTrans;

			m_pSprite->SetTransform(&matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureRightQuickSlot[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


			// 쿨타임
			_float fProgress = 1.f - pItemInven->Get_Progress(i);
			if (1.f == fProgress) continue;
			pTexInfo = m_pEmptyTexture->Get_TexInfo(0);
			vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

			RECT rc = { 0, 0, (LONG)pTexInfo->Width, (LONG)(pTexInfo->Height * fProgress) };
			m_pSprite->Draw((LPDIRECT3DTEXTURE9)m_pEmptyTexture->GetTexture(0), &rc, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


			// 개수
			TCHAR		szBuff[MAX_PATH] = L"";
			D3DXMATRIX	matScale2, matTrans2, matWorld2;
			D3DXMatrixIdentity(&matWorld);
			StringCchPrintf(szBuff, _countof(szBuff), L"%d", m_pRightSlotItem[i]->iCnt);

			D3DXMatrixTranslation(&matTrans2, vPos.x + 40.f, vPos.y + 40.f, 0.f);
			D3DXMatrixScaling(&matScale2, 1.5f, 1.5f, 0.f);
			matWorld2 = matScale2 * matTrans2;

			m_pSprite->SetTransform(&matWorld2);
			m_pFont->DrawTextW(m_pSprite, szBuff, lstrlen(szBuff),
				nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
		}
	}

	// 마우스 위치
	TCHAR		szBuff[MAX_PATH] = L"";
	D3DXMATRIX	matScale2, matTrans2, matWorld2;
	StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"%d, %d",
		pMouse->Get_Point().x, pMouse->Get_Point().y);

	D3DXMatrixScaling(&matScale2, 3.f, 3.f, 0.f);
	D3DXMatrixTranslation(&matTrans2, 100.f, 1000.f, 0.f);
	matWorld2 = matScale2 * matTrans2;

	m_pSprite->SetTransform(&matWorld2);
	m_pFont->DrawTextW(m_pSprite, szBuff, lstrlen(szBuff),
		nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

	return S_OK;
}

HRESULT CMainUI::Change_SkillIconPos()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (pMouse == nullptr)
		return E_FAIL;

	RECT rc = {};

	for (_uint i = 0; i < 8; i++)
	{
		// 왼쪽
		if (PtInRect(&m_tLeftSlotCollRt[i], pMouse->Get_Point()) && !m_bChange_SkillIconPos)
		{
			if (pManagement->Key_Pressing(VK_LBUTTON))
			{
				if (m_pLeftSlotItem[i] != nullptr && m_pTextureLeftQuickSlot[i] != nullptr)
				{
					m_bChange_SkillIconPos = true;
					m_bRender_GoingItem = true;
					m_pMovingItem = m_pLeftSlotItem[i];
					m_pTexture_GoingItem = m_pTextureLeftQuickSlot[i];
					m_iBefore_SkillIconIndex = i;
				}
			}
		}
	}

	return S_OK;
}

HRESULT CMainUI::Change_ItemPos()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (pMouse == nullptr)
		return E_FAIL;

	RECT rc = {};

	for (_uint i = 0; i < 8; i++)
	{
		// 오른쪽
		if (PtInRect(&m_tRightSlotCollRt[i], pMouse->Get_Point()) && !m_bChange_ItemPos)
		{
			if (pManagement->Key_Pressing(VK_LBUTTON))
			{
				if (m_pRightSlotItem[i] != nullptr && m_pTextureRightQuickSlot[i] != nullptr)
				{
					m_bChange_ItemPos = true;
					m_bRender_GoingItem = true;
					m_pMovingItem = m_pRightSlotItem[i];
					m_pTexture_GoingItem = m_pTextureRightQuickSlot[i];
					m_iBefore_ItemIndex = i;
				}
			}
		}
	}

	return S_OK;
}



HRESULT CMainUI::Add_Component()
{
	for (_uint i = 0; i < MAINUI_END; ++i)
	{
		// 1. VIBuffer
		TCHAR szVIBuffer[MAX_PATH] = L"";
		StringCchPrintf(szVIBuffer, _countof(szVIBuffer),
			L"Com_VIBuffer%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_VIBuffer_RectTexture"
			, szVIBuffer, (CComponent**)&m_pVIBufferCom[i])))
			return E_FAIL;

		// 2. Transform
		TCHAR szTransform[MAX_PATH] = L"";
		StringCchPrintf(szTransform, _countof(szTransform),
			L"Com_Transform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformCom[i])))
			return E_FAIL;

		// 3. Texture
		TCHAR szTexture[MAX_PATH] = L"";
		TCHAR szTextureName[MAX_PATH] = L"";
		wstring strTexture = L"";
		wstring strTextureName = L"";

		StringCchPrintf(szTexture, _countof(szTexture),
			L"Com_Texture%d", i);
		strTexture = szTexture;
		if (i == MAINUI_MAIN)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_MainUI_Main");
		else if (i == MAINUI_HP)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_MainUI_Hp");
		else if (i == MAINUI_MP)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_MainUI_Mp");
		else if (i == MAINUI_QUICKSLOT_LFFT)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_MainUI_QuickSlot_Left");
		else if (i == MAINUI_QUICKSLOT_RIGHT)
			StringCchPrintf(szTextureName, _countof(szTextureName),
				L"Component_Texture_MainUI_QuickSlot_Right");
		strTextureName = szTextureName;

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, strTextureName
			, strTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	for (_uint i = 0; i < 8; i++)
	{
		// 2. Transform
		TCHAR szTransform[MAX_PATH] = L"";
		StringCchPrintf(szTransform, _countof(szTransform),
			L"Com_LeftTransform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformLeftSlot[i])))
			return E_FAIL;

		TCHAR szRightTransform[MAX_PATH] = L"";
		StringCchPrintf(szRightTransform, _countof(szTransform),
			L"Com_RightTransform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szRightTransform, (CComponent**)&m_pTransformRightSlot[i])))
			return E_FAIL;
	}

	if (FAILED(CGameObject::Add_Component(
		SCENE_STATIC, L"Component_Texture_Item_Empty",
		L"Com_EmptyItemTexture", (CComponent**)&m_pEmptyTexture)))
		return E_FAIL;

	return S_OK;
}

CMainUI * CMainUI::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CMainUI* pInstance = new CMainUI(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMainUI", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMainUI::Clone_GameObject(void * pArg)
{
	CMainUI* pInstance = new CMainUI(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMainUI", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainUI::Free()
{
	Safe_Release(m_pEmptyTexture);
	Safe_Release(m_pTexture_GoingItem);

	for (_uint i = 0; i < MAINUI_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pVIBufferCom[i]);
	}

	for (_uint i = 0; i < 8; i++)
	{
		Safe_Release(m_pTransformLeftSlot[i]);
		Safe_Release(m_pTransformRightSlot[i]);
		//Safe_Release(m_pTextureLeftQuickSlot[i]);
	}

	CUIObject::Free();
}
