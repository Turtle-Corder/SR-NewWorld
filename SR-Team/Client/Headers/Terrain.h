#pragma once
#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "GameObject.h"

BEGIN(Client)
USING(Engine)

class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 _pDevice);
	explicit CTerrain(const CTerrain& _rOther);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;

	

private:
	CVIBuffer*	m_pVIBufferCom	= nullptr;
	CTexture*	m_pTextureCom	= nullptr;
	CTransform* m_pTransformCom = nullptr;
};

END

#endif
