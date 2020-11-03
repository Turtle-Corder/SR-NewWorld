


#ifndef __FLOWER_H__
#define __FLOWER_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CFlower : public CGameObject
{
private:
	explicit CFlower(LPDIRECT3DDEVICE9 pDevice);
	explicit CFlower(const CFlower& other);
	virtual ~CFlower() = default;

	enum BODY_ID
	{
		FLOWER_PETAL1, FLOWER_PETAL2, FLOWER_PETAL3, FLOWER_PETAL4, FLOWER_END
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
	static CFlower* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*	m_pVIBufferCom[FLOWER_END];
	CTransform*	m_pTransformCom[FLOWER_END];
	CTexture*	m_pTextureCom[FLOWER_END];
};

END

#endif // !__TREE_H__
