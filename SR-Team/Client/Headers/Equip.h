#pragma once

#ifndef __EQUIP_H__
#define __EQUIP_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CEquip final : public CUIObject
{
public:
	explicit CEquip(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CEquip(const CEquip& other);
	virtual ~CEquip() = default;

public:
	// ------------------------------------------------------------------
	// 아이템을 착용하는 함수
	// eITEM_SORT eSort : 착용할 아이템 종류
	// const wstring& strItemTag : 착용할 아이템Tag
	// ------------------------------------------------------------------
	void Equip_Item(eITEM_SORT eSort, const wstring& strItemTag);
	// ------------------------------------------------------------------
	// 아이템을 착용하고 있는지 검사하는 함수
	// const wstring& strItemTag : 검사할 아이템Tag
	// return true : 현재 착용하고 있다
	// return false : 착용하고 있지 않다
	// ------------------------------------------------------------------
	_bool Check_IsItemEquip(const wstring& strItemTag);

public:
	CStatus::STAT Get_PlayerStat() { return m_pStatCom->Get_Status(); }

public:
	void Set_PlayerAtt(_int iAtt);
	void Set_PlayerHp(_int iHP);
	void Set_PlayerMP(_int iMP);

public:
	_bool Get_Render() { return m_bRender; }

public:
	// CUIObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_UI() override;

private:
	HRESULT Render_EquipItem();
	HRESULT Render_Stat();

private:
	// ------------------------------------------------------------------
	// 스탯을 계산하는 함수
	// ------------------------------------------------------------------
	HRESULT Count_Stat();
	// ------------------------------------------------------------------
	// 아이템 장착을 해제하는 함수
	// ------------------------------------------------------------------
	HRESULT UnEquip_Item();

private:
	HRESULT Add_Component();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	_bool					m_bRender = false;

	// 현재 플레이어의 스탯
	CStatus*				m_pStatCom = nullptr;
	// 플레이어 스탯 폰트 위치
	//CTransform*				m_pTransformFont = nullptr;

	// 장비창 윈도우
	_vec3					m_vPos = { 500.f, 400.f, 0.f };
	CTransform*				m_pTransformCom[EQUIP_END];
	CTexture*				m_pTextureCom[EQUIP_END];
	RECT					m_tEquipWndCollRt[EQUIP_END];

	// 착용하고 있는 장비 정보
	CStatus*				m_pStatItem[ITEMSORT_END];
	CTexture*				m_pTextureItem[ITEMSORT_END];
	vector<CTransform*>		m_pTransformItem;
	RECT					m_tEquipItemCollRt[ITEMSORT_END] = {};
	_bool					m_bEquip[ITEMSORT_END] = { false, };
	// 현재 착용하고 있는 아이템Tag
	TCHAR					szEquipItemTag[ITEMSORT_END][MAX_PATH] = { L"", };

	// 스탯을 적용했는지
	_bool					m_bCountStat = false;
};


END
#endif
