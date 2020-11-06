#pragma once
#ifndef __ICEBLAST_H__
#define __ICEBLAST_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CIceBlast final : public CGameObject
{
private:
	explicit CIceBlast(LPDIRECT3DDEVICE9 _pDevice);
	explicit CIceBlast(const CIceBlast& _rOther);
	virtual ~CIceBlast() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();

public:
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;
	static CIceBlast* Create(LPDIRECT3DDEVICE9 _pDevice);



private:
	CVIBuffer*		m_pVIBufferComp = nullptr;
	CTransform*		m_pTransformComp = nullptr;
	CTexture*		m_pTextureComp = nullptr;
};

END

#endif // !__ICEBLAST_H__
