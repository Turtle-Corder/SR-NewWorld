#pragma once
#ifndef __COMPONENT_MANAGER_H__
#define __COMPONENT_MANAGER_H__

#include "Base.h"
#include "VIBuffer_TriColor.h"
#include "VIBuffer_RectColor.h"
#include "VIBuffer_RectTexture.h"
#include "VIBuffer_TerrainTexture.h"
#include "VIBuffer_CubeTexture.h"
#include "Transform.h"
#include "Texture.h"
#include "Raycast.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

BEGIN(Engine)

class CComponent;
class CComponent_Manager : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)

private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT		Setup_Component_Manager(_int _iSceneCount);
	HRESULT		Add_Component_Prototype(_int _iSceneID, const wstring& _strPrototypeTag, CComponent* _pPrototype);
	CComponent* Clone_Component(_int _iSceneID, const wstring& _strPrototypeTag, void* _pArg = nullptr);
	HRESULT		Clear_ForScene(_int _iSceneID);

public:
	virtual void Free() override;



private:
	typedef unordered_map<wstring, CComponent*> COMPONENTS;
	COMPONENTS* m_pComponents = nullptr;

	_int		m_iSceneCount = 0;
};

END

#endif
