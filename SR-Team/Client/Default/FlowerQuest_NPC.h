#pragma once

#ifndef __FLOWERQUESTNPC_H__
#define __FLOWERQUESTNPC_H__

#include "GameObject.h"
BEGIN(Client)
USING(Engine)

class CFlowerQuest_NPC : public CGameObject
{
private:
	explicit CFlowerQuest_NPC(LPDIRECT3DDEVICE9 _pDevice);
	explicit CFlowerQuest_NPC(const CFlowerQuest_NPC& _rOther);
	virtual ~CFlowerQuest_NPC() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();
	HRESULT Add_Component_Transform();
	HRESULT Add_Component_Texture();

public:
	static CFlowerQuest_NPC* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	CVIBuffer*			m_pVIBufferCom[PART_END];
	CTransform*			m_pTransformCom[PART_END];
	CTexture*			m_pTextureCom[PART_END];
	CRaycast*			m_pRaycastCom = nullptr;
};

END
#endif