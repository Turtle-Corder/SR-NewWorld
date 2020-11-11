#pragma once //CGolem_Piece
#ifndef __GOLEM_PIECE_H__
#define __GOLEM_PIECE_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CGolem_Piece final : public CGameObject
{
private:
	explicit CGolem_Piece(LPDIRECT3DDEVICE9 _pDevice);
	explicit CGolem_Piece(const CGolem_Piece& _rOther);
	virtual ~CGolem_Piece() = default;

	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual HRESULT Render_NoneAlpha() override;


private:
	HRESULT Add_Component();
	HRESULT Particle_Move(_float _fDeltaTime);

public:
	static CGolem_Piece* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp);


private:
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;



	INSTANTIMPACT		m_tInstant = {};
	_float				m_fDeadTime = 0.f;
	_float				m_fJumpPower = 3.f;
	_float				m_fJumpTime = 0.f;
};

END

#endif //__GOLEM_PIECE_H__
