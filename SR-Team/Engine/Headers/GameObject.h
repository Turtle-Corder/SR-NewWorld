#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Base.h"

BEGIN(Engine)
class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 _pDevice);
	explicit CGameObject(const CGameObject& _rOther);
	virtual ~CGameObject() = default;


protected:
	HRESULT Add_Component(_int _iSceneID, const wstring& _strPrototypeTag, const wstring& _strComponentTag, CComponent** _ppComponent = nullptr, void* _pArg = nullptr);

public:
	CComponent* Get_Component(const wstring& _strComponentTag);

public:
	virtual HRESULT Setup_GameObject_Prototype() = 0;
	virtual HRESULT Setup_GameObject(void* _pArg) = 0;
	virtual _int Update_GameObject(_float _fDeltaTime) = 0;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) = 0;

	virtual HRESULT Render_Priority();
	virtual HRESULT Render_NoneAlpha();
	virtual HRESULT Render_OnlyAlpha();
	virtual HRESULT Render_BlendAlpha();
	virtual HRESULT Render_UI();

	virtual HRESULT Take_Damage(const CComponent* _pDamageComp);
	void	Set_Dead();
	_bool	IsDead();

public:
	virtual CGameObject* Clone_GameObject(void* _pArg) = 0;
	virtual void Free() override;



protected:
	typedef unordered_map<wstring, CComponent*> COMPONENTS;
	COMPONENTS			m_Components;

	LPDIRECT3DDEVICE9	m_pDevice;


	_bool				m_bDead = false;
};

END

#endif
