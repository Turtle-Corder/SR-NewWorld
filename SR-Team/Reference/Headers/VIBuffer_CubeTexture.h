#pragma once
#ifndef __VIBUFER_CUBETEXTURE_H__
#define __VIBUFER_CUBETEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_CubeTexture final : public CVIBuffer
{
private:
	explicit CVIBuffer_CubeTexture(LPDIRECT3DDEVICE9 _pDevice);
	explicit CVIBuffer_CubeTexture(const CVIBuffer_CubeTexture& _rOther);
	virtual ~CVIBuffer_CubeTexture() = default;

public:
	virtual void* Get_Vertices() const override;
	virtual void* Get_Indices() const override;

private:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void* _pArg) override;


public:
	HRESULT Set_Transform_Nothing(const _matrix * _pMatWorld);
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const CCamera* _pCamera) override;
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const _matrix* _pMatView, const _matrix* _pMatProj) override;

public:
	static CVIBuffer_CubeTexture* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent* Clone_Component(void* _pArg) override;
	virtual void Free() override;



private:
	VTX_CUBETEXTURE*	m_pVTXOrigin	= nullptr;
	VTX_CUBETEXTURE*	m_pVTXConvert	= nullptr;
	INDEX16*			m_pIndices		= nullptr;
};

END

#endif
