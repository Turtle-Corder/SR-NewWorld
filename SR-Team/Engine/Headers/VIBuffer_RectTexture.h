#pragma once
#ifndef __VIBUFFER_RECTTEXTURE_H__
#define __VIBUFFER_RECTTEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectTexture final : public CVIBuffer
{
private:
	explicit CVIBuffer_RectTexture(LPDIRECT3DDEVICE9 _pDevice);
	explicit CVIBuffer_RectTexture(const CVIBuffer_RectTexture& _rOther);
	virtual ~CVIBuffer_RectTexture() = default;

public:
	virtual void* Get_Vertices() const override;
	virtual void* Get_Indices() const override;

public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void* _pArg) override;

	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const CCamera* _pCamera) override;
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const _matrix* _pMatView, const _matrix* _pMatProj) override;

public:
	static CVIBuffer_RectTexture* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent* Clone_Component(void* _pArg) override;
	virtual void Free() override;



private:
	VTX_TEXTURE*	m_pVTXOrigin	= nullptr;
	VTX_TEXTURE*	m_pVTXConvert	= nullptr;

	INDEX16*		m_pIndices		= nullptr;
};

END

#endif
