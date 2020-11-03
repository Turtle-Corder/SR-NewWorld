#pragma once
#ifndef __OBJECT_MANAGER_H__
#define __OBJECT_MANAGER_H__

#include "Base.h"

BEGIN(Engine)

class CComponent;
class CGameObject;
class CLayer;
class CCollider;
class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)

private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	CGameObject*	Get_GameObject(_int _iSceneID, const wstring& _strLayerTag, _uint _iIndex = 0);
	CComponent*		Get_Component(_int _iSceneID, const wstring& _strLayerTag, const wstring& _strComponentTag, _uint _iIndex = 0);

public:
	HRESULT Setup_Object_Manager(_int _iSceneCount);
	HRESULT Add_GameObject_Prototype(_int _iSceneID, const wstring& _strProtoTypeTag, CGameObject* _pProtoType);
	HRESULT Add_GameObject_InLayer(_int _iFromSceneID, const wstring& _strProtoTypeTag, _int _iToSceneID, const wstring& _strLayerTag, void* _pArg = nullptr);
	HRESULT Clear_ForScene(_int _iSceneID);

	// tag를 등록하고 Clear_Except를 하면 등록한 tag의 레이어는 보존한다.
	HRESULT Register_ExceptTag(_int _iCurSceneID, const wstring& _strLayerTag);
	HRESULT Clear_Except(_int _iFromSceneID, _int _iToSceneID);

	HRESULT CollisionSphere_Detection_Layers(_int _iSceneID, const wstring& _strSrcLayerTag, const wstring& _strDstLayerTag, const wstring& _strColliderTag, const wstring& _strDmgInfoTag);
	HRESULT CollisionSphere_Detection_Layers_Both(_int _iSceneID, const wstring& _strSrcLayerTag, const wstring& _strDstLayerTag, const wstring& _strColliderTag, const wstring& _strDmgInfoTag);

	HRESULT CollisionBox_Detection_Layers(_int _iSceneID, const wstring& _strSrcLayerTag, const wstring& _strDstLayerTag, const wstring& _strColliderTag, const wstring& _strDmgInfoTag);
	HRESULT CollisionBox_Detection_Layers_Both(_int _iSceneID, const wstring& _strSrcLayerTag, const wstring& _strDstLayerTag, const wstring& _strColliderTag, const wstring& _strDmgInfoTag);

	HRESULT CollisionSphere_Impulse_Layers(_int _iSceneID, const wstring & _strSrcLayerTag, const wstring & _strDstLayerTag, const wstring & _strColliderTag, const wstring & _strTransformTag);
	HRESULT CollisionBox_Impulse_Layers(_int _iSceneID, const wstring & _strSrcLayerTag, const wstring & _strDstLayerTag, const wstring & _strColliderTag, const wstring & _strTransformTag);

public:
	_int Update_Object_Manger(_float _fDeltaTime);
	_int LateUpdate_Object_Manager(_float _fDeltaTime);

private:
	HRESULT Add_Layer(_int _iSceneID, const wstring& _strLayerTag, CLayer* _pLayer);


public:
	virtual void Free() override;



public:
	typedef unordered_map<wstring, CGameObject*>	GAMEOBJECTS;
	typedef unordered_map<wstring, CLayer*>			LAYERS;


	GAMEOBJECTS*	m_pGameObjects	= nullptr;
	LAYERS*			m_pLayers		= nullptr;

	_int			m_iSceneCount = 0;

	LAYERS			m_PreseveLayers;

};

END

#endif
