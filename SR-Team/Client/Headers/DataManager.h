#pragma once
#ifndef __DATAMANAGER_H__
#define __DATAMANAGER_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CDataManager final : public CGameObject
{
	explicit CDataManager(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDataManager(const CDataManager& _rOther);
	virtual ~CDataManager() = default;

public:
	// ------------------------------------------------------------------
	// 필요한 아이템의 텍스처를 반환하는 함수
	// const wstring& strItemTag : 필요한 텍스처의 Tag
	// ------------------------------------------------------------------
	CTexture* Get_ItemInfo_Texture(const wstring& strItemTag);

	// ------------------------------------------------------------------
	// 필요한 아이템의 가격을 반환하는 함수
	// const wstring& strItemTag : 필요한 텍스처의 Tag
	// ------------------------------------------------------------------
	const _int Get_ItemInfo_Price(const wstring& strItemTag);
	HRESULT Get_ItemInfo(const wstring& strItemTag, INVEN_ITEM& tItem);

	// ------------------------------------------------------------------
	// 필요한 아이템의 스탯을 반환하는 함수
	// const wstring& strItemTag : 필요한 아이템의 Tag
	// ------------------------------------------------------------------
	CStatus* Get_ItemStat(const wstring& strItemTag);


public:
	// ------------------------------------------------------------------
	// 필요한 액티브 스킬아이콘의 객체를 반환하는 함수
	// _int iSkillID : 반환받을 객체의 액티브 스킬 아이디
	// ------------------------------------------------------------------
	INVEN_ITEM* Get_ActiveSkillIcon(_int iSkillID);


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float DeltaTime) override;
	virtual int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_UI() override;


private:
	HRESULT Add_Component();
	HRESULT Add_Component_Item();
	HRESULT Add_Component_SkillIcon();

public:
	static CDataManager* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	bool			m_bRenderShopItem = false;
	bool			m_bRenderInvenItem = false;

	// 아이템
private:
	CTexture*				m_pTextureCom[23];
	CStatus*				m_pStatCom[23];
	vector<INVEN_ITEM*>		m_vItemList;

	// 스킬 아이콘
private:
	CTexture*				m_pTextureSkillIcon[11];
	vector<INVEN_ITEM*>		m_vSkillIconList;

};	

END

#endif // !__ITEM_H__
