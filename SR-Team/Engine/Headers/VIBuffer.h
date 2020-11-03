#pragma once
#ifndef __VIBUFFER_H__
#define __VIBUFFER_H__

#include "Component.h"
#include "Pipeline.h"
#include "Camera.h"

BEGIN(Engine)

class CCamera;
class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 _pDevice);
	explicit CVIBuffer(const CVIBuffer& _rOther);
	virtual ~CVIBuffer() = default;

public:
	_uint Get_TriCount() const;
	virtual void* Get_Vertices() const;
	virtual void* Get_Indices() const;

public:
	virtual HRESULT Setup_Component_Prototype() = 0;
	virtual HRESULT Setup_Component(void* _pArg) = 0;
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const _matrix* _pMatView, const _matrix* _pMatProj);
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const CCamera* _pCamera);
	virtual HRESULT Render_VIBuffer();

public:
	virtual CComponent* Clone_Component(void* _pArg) = 0;
	virtual void Free() override;


protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;

	_uint		m_iVertexSize	= 0;
	_uint		m_iVertexCount	= 0;
	_uint		m_iFVF			= 0;
	_uint		m_iTriCount		= 0;

	_uint		m_iIndexSize	= 0;
	D3DFORMAT	m_IndexFmt;
};

END

#endif
