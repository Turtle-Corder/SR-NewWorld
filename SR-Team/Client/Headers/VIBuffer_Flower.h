#pragma once
#ifndef __VIBUFFER_FLOWER_H__
#define __VIBUFFER_FLOWER_H__

#include "VIBuffer.h"

USING(Engine)
BEGIN(Client)

class CVIBuffer_Flower final : public CVIBuffer
{
private:
	explicit CVIBuffer_Flower(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_Flower(const CVIBuffer_Flower& other);
	virtual ~CVIBuffer_Flower() = default;

public:
	virtual void* Get_Vertices() const;
	virtual void* Get_Indices() const;

public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void* _pArg) override;
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const _matrix* _pMatView, const _matrix* _pMatProj);
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const CCamera* _pCamera);

public:
	static CVIBuffer_Flower* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;



private:
	VTX_CUBETEXTURE*	m_pVertices = nullptr;
	VTX_CUBETEXTURE*	m_pConvertVertices = nullptr;
};

END

#endif // !__VIBUFFER_FLOWER_H__
