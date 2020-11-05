#pragma once
#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "GameObject.h"

BEGIN(Client)
USING(Engine)

class CSkybox final : public CGameObject
{
private:
	explicit CSkybox(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSkybox(const CSkybox& _rOther);
	virtual ~CSkybox() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

	virtual HRESULT Render_Priority() override;
private:
	HRESULT Add_Component();
	HRESULT Movement(_float _fDeltaTime);
	
public:
	static CSkybox* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*	m_pVIBufferCom	= nullptr;
	CTransform* m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom	= nullptr;

	_uint m_iTextureID = 0;
};

END

#endif
