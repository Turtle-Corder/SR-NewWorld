#pragma once
#ifndef __DUMMYTERRAIN_H__
#define __DUMMYTERRAIN_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDummyTerrain final : public CGameObject
{
private:
	explicit CDummyTerrain(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDummyTerrain(const CDummyTerrain& _rOther);
	virtual ~CDummyTerrain() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();

public:
	static CDummyTerrain* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CTransform* m_pTransformCom = nullptr;

	_bool		m_bRefresh = false;
};

END

#endif // !__DUMMYTERRAIN_H__