#include "stdafx.h"
#include "..\Headers\PinkBean.h"

USING(Client)

CPinkBean::CPinkBean(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CPinkBean::CPinkBean(const CPinkBean & _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CPinkBean::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CPinkBean::Setup_GameObject(void * _pArg)
{
	//if(_pArg)

	return S_OK;
}

_int CPinkBean::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CPinkBean::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

HRESULT CPinkBean::Render_NoneAlpha()
{
	return S_OK;
}

CGameObject * CPinkBean::Clone_GameObject(void * _pArg)
{
	return nullptr;
}

void CPinkBean::Free()
{
}

CPinkBean * CPinkBean::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	return nullptr;
}

HRESULT CPinkBean::Add_Component()
{
	return S_OK;
}

HRESULT CPinkBean::Movement(float _fDeltaTime)
{
	return S_OK;
}

HRESULT CPinkBean::IsOnTerrain()
{
	return S_OK;
}
