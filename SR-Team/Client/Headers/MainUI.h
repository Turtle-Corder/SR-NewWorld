#pragma once
#ifndef __MAINUI_H__
#define __MAINUI_H__

#include "UIObject.h"

USING(Engine)
BEGIN(Client)

class CMainUI : public CUIObject
{
private:
	explicit CMainUI(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CMainUI(const CMainUI& _rOther);
	virtual ~CMainUI() = default;


public:
	// ------------------------------------------------------------------
	// 현재 퀵슬롯으로 이동하고 있는 아이템의 정보를 받아오는 함수
	// INVEN_ITEM* pItem : 현재 퀵슬롯으로 이동하고 있는 아이템/스킬
	// ------------------------------------------------------------------
	HRESULT Get_QuickSlotItem(INVEN_ITEM* pItem);
	// ------------------------------------------------------------------
	// 인벤에서 판매된 포션을 받아와서 
	// 현재 퀵슬롯에 있는지 확인하고, 있으면 개수를 0으로 변경시켜주는 함수
	// INVEN_ITEM* pItem : 인벤에서 판매된 포션
	// ------------------------------------------------------------------
	HRESULT Delete_Potion(INVEN_ITEM* pItem);
	// ------------------------------------------------------------------
	// 현재 퀵슬롯으로 이동하고 있는 스킬의 정보를 받아오는 함수
	// INVEN_ITEM* pItem : 현재 퀵슬롯으로 이동하고 있는 스킬
	// ------------------------------------------------------------------
	HRESULT Get_QuickSlotSkill(_int iSkillID);


private:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float DeltaTime) override;
	virtual int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_UI() override;

	
// 퀵슬롯 관련 함수
private:
	// 아이템을 퀵슬롯으로 이동시키는 함수
	HRESULT Move_To_QuickSlot();
	// 왼쪽/오른쪽 퀵슬롯과 아이템이 충돌했는지 검사하는 함수
	// -> 충돌했으면 현재 슬롯에 아이템이 있는지 확인하고 없으면 장착
	HRESULT Check_LeftQuickSlot_Item();
	HRESULT Check_RightQuickSlot_Item();
	// 현재 슬롯에 아이템이 있는지 검사
	// -> 있음 : false / 없음 : true 반환
	_bool Check_Item_In_Slot();
	// 인벤에서 아이템의 개수를 받아오는 함수
	// -> 아이템 개수가 0이면 출력X
	HRESULT Set_SlotItem_Count();


private:
	// 퀵슬롯으로 이동중인 아이템을 그리는 함수
	HRESULT Render_Item_GoingToQuickSlot();
	// 퀵슬롯에 있는 아이템을 그리는 함수
	HRESULT Render_QuickSlot_Item();

private:
	HRESULT Add_Component();

public:
	static CMainUI* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	// Component-------------------------------------------------------------
	CVIBuffer*		m_pVIBufferCom[MAINUI_END];
	CTransform*		m_pTransformCom[MAINUI_END];
	CTexture*		m_pTextureCom[MAINUI_END];

	_vec3			m_vMainPos = { WINCX * 0.5f, WINCY - 300.f, 0.f };

	// 충돌 렉트
	RECT			m_tCollRt[MAINUI_END];


// 퀵슬롯==================================================================
private:
	// 현재 이동시키고 있는 아이템 정보
	INVEN_ITEM*		m_pMovingItem = nullptr;

	// 왼쪽, 오른쪽 퀵슬롯 충돌 렉트
	RECT			m_tLeftSlotCollRt[8];
	RECT			m_tRightSlotCollRt[8];

	// 왼쪽, 오른쪽 퀵슬롯 개당 위치
	CTransform*		m_pTransformLeftSlot[8];
	CTransform*		m_pTransformRightSlot[8];

	// 왼쪽, 오른쪽 큰 퀵슬롯 위치
	_vec3			m_vLeftSlotPos = { 170.f, 500.f, 0.f };
	_vec3			m_vRightSlotPos = { 660.f, 500.f, 0.f };

	CTexture*		m_pEmptyTexture = nullptr;

	// 퀵슬롯으로 가는 아이템 위치, 충돌 렉트
	_vec3			m_vGoingItem_Pos = { 0.f, 0.f, 0.f };
	RECT			m_tGoingItem_CollRt = { 0, 0, 0, 0 };

	// 퀵슬롯으로 가는 아이템 그릴지 말지
	_bool			m_bRender_GoingItem = false;
	// 퀵슬롯으로 가는 아이템 그릴 텍스처
	CTexture*		m_pTexture_GoingItem = nullptr;

	// 퀵슬롯에 그릴 이미지
	CTexture*		m_pTextureLeftQuickSlot[8];
	CTexture*		m_pTextureRightQuickSlot[8];

	// 퀵슬롯이 가지고 있는 아이템 정보
	INVEN_ITEM*		m_pLeftSlotItem[8];
	INVEN_ITEM*		m_pRightSlotItem[8];
};
END

#endif // !__MAINUI_H__
