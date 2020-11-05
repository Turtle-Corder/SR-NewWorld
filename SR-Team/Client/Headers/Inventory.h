#pragma once
#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "UIObject.h"

USING(Engine)
BEGIN(Client)

class CInventory : public CUIObject
{
private:
	explicit CInventory(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CInventory(const CInventory& _rOther);
	virtual ~CInventory() = default;

// Setter
public:
	// ------------------------------------------------------------------
	// 인벤과 아이템을 그릴것인지 안그릴것인지 외부에서 설정해주는 함수
	// bool bRender : true면 그린다, false면 그리지 않는다
	// ------------------------------------------------------------------
	void Set_Render(bool bRender);


// Getter
public:
	// ------------------------------------------------------------------
	// 인벤과 아이템을 그릴것인지 안그릴것인지 외부에게 알려주는 함수
	// m_bRender : true면 그린다, false면 그리지 않는다
	// ------------------------------------------------------------------
	_bool Get_Render() { return m_bRender; }

public:
	void Use_Potion(ePotion_ID ePotionID);

public:
	// ------------------------------------------------------------------
	// 상점에서 선택한 아이템 구매 / 인벤에 추가하는 함수
	// const wstring& strItemName : 구매한 아이템 이름
	// ------------------------------------------------------------------
	HRESULT Get_ShopItem(const wstring& strItemName);
	// ------------------------------------------------------------------
	// 아이템의 개수를 반환해주는 함수
	// const wstring& strItemName : 어떤 아이템의 개수가 궁금한지
	// ------------------------------------------------------------------
	_int Get_ItemCount(const wstring& strItemName);
	// ------------------------------------------------------------------
	// 아이템을 삭제하는 함수
	// const wstring& strItemName : 삭제할 아이템의 이름
	// ------------------------------------------------------------------
	HRESULT Delete_Item(const wstring& strItemName);
	HRESULT Get_RewardItem(const wstring& strItemName);

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;

	virtual _int Update_GameObject(float DeltaTime) override;
	virtual _int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_UI() override;


private:
	// ------------------------------------------------------------------
	// 인벤에 있는 아이템 개수를 확인하는 함수
	// -> 만약 아이템 개수가 0이면 삭제되고 자동정렬 된다
	// ------------------------------------------------------------------
	HRESULT Check_ItemCount();


// 판매 관련 함수
private:
	// ------------------------------------------------------------------
	// 판매 버튼을 눌렀는지 확인하는 함수
	// -> 눌렀으면 m_bSelect_SellItem = true, 아니면 false
	// ------------------------------------------------------------------
	HRESULT Check_SellButton();

	// ------------------------------------------------------------------
	// 판매 상태일 때, 판매할 아이템을 선택하는 함수
	// ------------------------------------------------------------------
	HRESULT Select_SellItem();


// 정렬 관련 함수
private:
	// ------------------------------------------------------------------
	// 자동 정렬 버튼을 눌렀는지 확인하는 함수
	// ------------------------------------------------------------------
	HRESULT Check_AutoSortButton();


// 아이템 장착 관련 함수
private:
	// ------------------------------------------------------------------
	// 아이템을 장착할 것인지 확인하는 함수
	// ------------------------------------------------------------------
	HRESULT Check_EquipItem();


// 아이템 퀵슬롯으로 이동
private:
	// ------------------------------------------------------------------
	// 퀵슬롯으로 이동할 아이템을 선택하는 함수
	// ------------------------------------------------------------------
	HRESULT Move_To_QuickSlot();
	// ------------------------------------------------------------------
	// 아이템이 판매되었을 때, 퀵슬롯에 있는 포션 아이템들 개수 변경해주는 함수
	// ------------------------------------------------------------------
	HRESULT Change_PotionCnt();


// Render() 관련 함수
private:
	// ------------------------------------------------------------------
	// 아이템을 그리는 함수
	// ------------------------------------------------------------------
	HRESULT Render_Item();

// 움직임 관련 함수
private:
	// ------------------------------------------------------------------
	// 인벤 윈도우를 움직이는 함수
	// ------------------------------------------------------------------
	HRESULT Move_InventoryWnd();
	// ------------------------------------------------------------------
	// 인벤이 움직인 거리만큼 
	// 인벤에 있는 아이템들도 그만큼 움직여준다
	// ------------------------------------------------------------------
	HRESULT Change_AllPos();
	
private:
	HRESULT Add_Component();
	HRESULT Add_Component_Item();

public:
	static CInventory* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	_int				m_iNewInsertOrder = 0;
	_int				m_iSellGold = 0;
	// 현재 가지고 있는 골드
	_int				m_iGold = 10000;
	// true : 인벤을 그린다 / false : 인벤을 그리지 않는다
	bool				m_bRender = false;
	// 인벤 윈도우 위치
	_vec3 vWndPos =		{ 1500.f, 440.f, 0.f };
	_int				m_iDeleteCnt = 0;

// 아이템--------------------------------------------------------------------------------
private:
	// 아이템들을 리스트로 관리
	//list<INVEN_ITEM*>	m_pInvenList;
	vector<INVEN_ITEM*>	m_pInvenList;
	// 인벤에 있는 아이템들
	vector<CTexture*>	m_pTextureItem;
	vector<CTransform*>	m_pTransformItem;

	// 인벤에 있는 아이템들 위치
	_vec3				m_vItemPos[6][6];
	// 인벤에 있는 아이템들 충돌 RECT
	RECT				m_tItemCollRt[6][6];
	// 판매할 아이템
	CTexture*			m_pTextureSell = nullptr;

// 인벤창--------------------------------------------------------------------------------
private:
	CTransform*			m_pTransformCom[INVEN_END];
	CTexture*			m_pTextureCom[INVEN_END - 1];
	// 인벤창 위치, 충돌 RECT
	_vec3				m_vInvenWndPos[INVEN_END];
	RECT				m_tInvenWndCollRt[INVEN_END];


private:
	// m_bRenderEmptySell가 true 일 때,
	// 빈 텍스처를 그릴 버퍼, 텍스처
	CTexture*			m_pTextureEmpty = nullptr;


private:
	// 아이템이 인벤에 삽입되는 순서
	// == 인벤에 있는 아이템들 개수 - 1
	_int				m_iInsertOrder = 0;

	// 판매 아이템을 선택하는중 = true, 그 외 = false
	_bool				m_bSelect_SellItem = false;

	// 현재 칸에 아이템이 삽입되어 있는지
	_bool				m_bIsItemHere[36] = { false };

	// 현재 아이템이 판매 아이템으로 선택되어 있음 = true
	_bool				m_bSelectedSell[36] = { false };

	// 판매버튼을 누르고 정렬을 누르지 않았을 때
	// -> true일 때만 빈 텍스처로 바꿔주고
	// -> 이후 정렬 버튼을 누르면 이 때, 리스트에서도 삭제해준다
	_bool				m_bRenderEmptySell = false;

	// 정렬했는지
	_bool				m_bAutoSort = false;

	// 인벤 창 움직이면 안된다
	_bool				m_bMoveInvenWnd = true;

};

END

#endif // !__INVENTORY_H__
