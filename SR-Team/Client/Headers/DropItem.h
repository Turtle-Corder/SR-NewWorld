#pragma once
#ifndef __DROPITEM_H__
#define __DROPITEM_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDropItem final : public CGameObject
{
	enum ITEM
	{
		ITEM_BASE ,
		ITEM_BOX = ITEM_BASE,
		ITEM_TEXTURE,
		ITEM_END
	};
private:
	explicit CDropItem(LPDIRECT3DDEVICE9 pDevice);
	explicit CDropItem(const CDropItem& other);
	virtual ~CDropItem() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual int Update_GameObject(float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(float _fDeltaTime) override;
	virtual HRESULT Render_OnlyAlpha() override;
	virtual HRESULT Render_BlendAlpha() override;
private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT Move(float _fDeltaTime);
	HRESULT Setting_ItemBox();
	HRESULT Setting_ItemTexture();
	HRESULT Floating(_float _fDeltaTime);

public:
	static CDropItem* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject(void * pArg) override;
	virtual void Free() override;
private:
	CVIBuffer*	m_pVIBufferCom[ITEM_END] = {};
	CTransform*	m_pTransformCom[ITEM_END] = {};
	CTexture*	m_pTextureCom[ITEM_END] = {};


	_bool		m_bDead = false;
	_vec3		m_vPos = { 0.f , 0.f , 0.f };
	_int		m_iRand = 0;

	_uint		m_iCurFrame = 0;

	_float		m_fTimeFlow = 0.f;
};

END

#endif // !__DROPITEM_H__
