#pragma once
#ifndef __PLANE_SKILL_H__
#define __PLANE_SKILL_H__

#include"GameObject.h"

	USING(Engine)
	BEGIN(Client)

class CStatus;
class CPlaneSkill final : public CGameObject
{
private:
	explicit CPlaneSkill(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlaneSkill(const CPlaneSkill& other);
	virtual ~CPlaneSkill() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* pArg) override;
	virtual int Update_GameObject(float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(float _fDeltaTime) override;
	virtual HRESULT Render_OnlyAlpha() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
public:
	static CPlaneSkill* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;

	bool  m_bDead = false;
	_vec3 m_vPlayerPos = {};
	_vec3 m_vGoalPos = {};
	float m_fDownTime = 0.f;
};

END

#endif // !__PLANE_SKILL_H__