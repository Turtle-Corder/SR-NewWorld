#pragma once
#ifndef __MONSUB_H__
#define __MONSUB_H__


#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CMonSub final : public CGameObject
{
	enum MONSUB
	{
		MONSUB_BASE,
		MONSUB_RIGHT,
		MONSUB_END
	};
private:
	explicit CMonSub(LPDIRECT3DDEVICE9 _pDevice);
	explicit CMonSub(const CMonSub& _rOther);
	virtual ~CMonSub() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;
private:
	HRESULT Add_Component();
	HRESULT Movement(_float _fDeltaTime);
	HRESULT IsOnTerrain(_float _fDeltaTime);
	HRESULT Move(_float _fDeltaTime);
	HRESULT Roll(_float _fDeltaTime);
public:
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;
	static CMonSub* Create(LPDIRECT3DDEVICE9 _pDevice);
	HRESULT Setting_Dir();
	HRESULT LookAtPlayer(_float _fDeltaTime);
private:
	CVIBuffer*		m_pVIBufferCom[MONSUB_END] = {};
	CTransform*		m_pTransformCom[MONSUB_END] = {};
	CTexture*		m_pTextureCom[MONSUB_END] = {};


	_vec3			m_vStartPos = {};
	_float			m_fJumpPower = 2.f;
	_float			m_fJumpTime = 0.f;
	_vec3			m_vDir = {};
	_bool			m_bOnece = false;
	_bool			m_bFallDown = false;
	_bool			m_bAllStop = false;
	_bool			m_bMoveOn = false;
	_bool			m_bCheck = false;
};

END

#endif //__MONSUB_H__