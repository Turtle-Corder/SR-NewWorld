#pragma once
#ifndef __LAYER_H__
#define __LAYER_H__

#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CComponent;

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	CGameObject*	Get_GameObject(_uint _iIndex = 0);
	CComponent*		Get_Component(const wstring& _strComponentTag, _uint _iIndex = 0);

public:
	HRESULT Add_GameObject_InLayer(CGameObject* _pObject);
	_int	Update_Layer(_float _fDeltaTime);
	_int	LateUpdate_Layer(_float _fDeltaTime);

	HRESULT CollisionSphere_Detection_Layers(CLayer* _pSrcLayer, const wstring& _strColliderTag, const wstring& _strDmgInfoTag);
	HRESULT CollisionSphere_Detection_Layers_Both(CLayer* _pSrcLayer, const wstring& _strColliderTag, const wstring& _strDmgInfoTag);

	HRESULT CollisionBox_Detection_Layers(CLayer* _pSrcLayer, const wstring& _strColliderTag, const wstring& _strDmgInfoTag);
	HRESULT CollisionBox_Detection_Layers_Both(CLayer* _pSrcLayer, const wstring& _strColliderTag, const wstring& _strDmgInfoTag);

	// Src가 밀린다.
	HRESULT CollisionSphere_Impulse_Layers(CLayer* _pSrcLayer, const wstring& _strColliderTag, const wstring& _strTransformTag, _bool bSame);

	// Src가 밀린다.
	HRESULT CollisionBox_Impulse_Layers(CLayer* _pSrcLayer, const wstring& _strColliderTag, const wstring& _strTransformTag, _bool _bSame);


public:
	static CLayer* Create();
	virtual void Free() override;

private:
	typedef list<CGameObject*> GAMEOBJECTS;
	GAMEOBJECTS	m_GameObjects;
};

END

#endif
