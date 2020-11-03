#pragma once
#ifndef __VIBUFFER_RECTCOLOR_H__
#define __VIBUFFER_RECTCOLOR_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectColor final : public CVIBuffer
{
private:
	explicit CVIBuffer_RectColor(LPDIRECT3DDEVICE9 _pDevice, _uint _iColor);
	explicit CVIBuffer_RectColor(const CVIBuffer_RectColor& _rOther);
	virtual ~CVIBuffer_RectColor() = default;

public:
	virtual void* Get_Vertices() const override;
	virtual void* Get_Indices() const override;

public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void* _pArg) override;
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const CCamera* _pCamera);
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const _matrix* _pMatView, const _matrix* _pMatProj);

public:
	static CVIBuffer_RectColor* Create(LPDIRECT3DDEVICE9 _pDevice, _uint _iColor = D3DCOLOR_ARGB(255, 255, 0, 0));
	virtual CComponent* Clone_Component(void* _pArg) override;
	virtual void Free() override;



private:
	VTX_COLOR*	m_pVTXOrigin	= nullptr;
	VTX_COLOR*	m_pVTXConvert	= nullptr;
	INDEX16*	m_pIndices		= nullptr;

	_uint		m_iColor		= 0;
};

END

#endif
