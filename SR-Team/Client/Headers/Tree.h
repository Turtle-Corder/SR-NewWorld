#pragma once
#ifndef __TREE_H__
#define __TREE_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CTree : public CGameObject
{
private:
	explicit CTree(LPDIRECT3DDEVICE9 pDevice);
	explicit CTree(const CTree& other);
	virtual ~CTree() = default;

	enum BODY_ID
	{
		TREE_HEAD, TREE_BODY, TREE_END
	};

	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float DeltaTime) override;
	virtual int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

	

private:
	virtual HRESULT Add_Component();
	HRESULT Add_Component(_vec3 _vPos);
public:
	static CTree* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*	m_pVIBufferCom[TREE_END];
	CTransform*	m_pTransformCom[TREE_END];
	CTexture*	m_pTextureCom[TREE_END];
};

END

#endif // !__TREE_H__
