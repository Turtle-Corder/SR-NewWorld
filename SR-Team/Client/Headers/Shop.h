#pragma once
#ifndef __SHOP_H__
#define __SHOP_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CShop final : public CUIObject
{
private:
	explicit CShop(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CShop(const CShop& other);
	virtual ~CShop() = default;

// Setter===================================================================================
public:
	// ------------------------------------------------------------------
	// 상점과 아이템을 그릴것인지 안그릴것인지 외부에서 설정해주는 함수
	// bool bRender : true면 그린다, false면 그리지 않는다
	// ------------------------------------------------------------------
	void Set_Render(bool bRender) { m_bRender = bRender; }
	void Change_RenderState() { m_bRender = !m_bRender; }

	void Set_ShopItemTexturePos(_vec3* pPos);
	void Set_ShopItemNamePos(_vec3* pPos);
	void Set_ShopItemPricePos(_vec3* pPos);


// Setter===================================================================================
public:
	// ------------------------------------------------------------------
	// 상점과 아이템을 그릴것인지 안그릴것인지 외부에게 알려주는 함수
	// m_bRender : true면 그린다, false면 그리지 않는다
	// ------------------------------------------------------------------
	bool Get_Render() { return m_bRender; }


public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float DeltaTime) override;
	virtual int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_UI() override;

// 아이템 구매 관련 함수들===========================================================================
private:
	// ------------------------------------------------------------------
	// 구매할 아이템을 선택하는 함수
	// ------------------------------------------------------------------
	HRESULT Check_BuyItem();
	// ------------------------------------------------------------------
	// 위의 함수에서 선택한 아이템의 인덱스를 받아와서 
	// 인벤에게 해당 아이템의 Tag를 전달해준다
	// ------------------------------------------------------------------
	HRESULT Buy_Item(_uint iIndexJ, _uint iIndexI);

private:
	// ------------------------------------------------------------------
	// 상점 스크롤 막대 이동
	// ------------------------------------------------------------------
	HRESULT Move_ScrollBar();

private:
	HRESULT Add_Component();
	HRESULT Add_Component_ShopItem();

private:
	// ------------------------------------------------------------------
	// 상점에 있는 아이템들을 그리는 함수
	// ------------------------------------------------------------------
	HRESULT Render_ShopItem();

public:
	static CShop* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	HDC				m_DC;
	bool			m_bRender = false;
	RECT			m_tShopWnd;

	// Component-----------------------------------
	// 상점 윈도우
	CTransform*		m_pTransformCom[SHOP_END];
	CTexture*		m_pTextureCom[SHOP_END];
	RECT			m_tShopWndCollRt[SHOP_END];

	// 상점 아이템
	vector<INVEN_ITEM*>	m_vShopItem;

	// 상점 아이템 이미지
	CTransform*		m_pItemTransformCom[16];
	CTexture*		m_pItemTextureCom[16];

	// Pos, Scale----------------------------------
	_vec3			m_vPos[SHOP_END];
	_vec3			m_vScale[SHOP_END];

	// 상점 윈도우 내 아이템 위치
	_vec3			m_vItemTexturePos[3][4];
	_vec3			m_vItemNamePos[3][4];
	_vec3			m_vItemPricePos[3][4];

	// 지금은 일단 이미지로 충돌하는데 
	// 나중에 구입하기 버튼 rect로 충돌체크하기
	RECT			m_tItemTextureRt[3][4];

	// 구입하기 버튼 위치, RECT
	_vec3			m_vBuyPos[4][3];
	RECT			m_tBuyRect[4][3];

	// 스크롤바 충돌 RECT
	RECT			m_tScrollBarRt = {};
	int				m_iStartIndex = 0;


};

END

#endif // !__SHOP_H__
