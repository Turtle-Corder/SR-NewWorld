#pragma once
#ifndef __TREEHEAD_H__
#define __TREEHEAD_H__

#include "VIBuffer.h"

USING(Engine)
BEGIN(Client)

class CVIBuffer_TreeHead final : public CVIBuffer
{
private:
	explicit CVIBuffer_TreeHead(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_TreeHead(const CVIBuffer_TreeHead& other);
	virtual ~CVIBuffer_TreeHead() = default;

public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void * pArg) override;

public:
	virtual void* Get_Vertices() const override;
	virtual void* Get_Indices() const override;

public:
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const _matrix* _pMatView, const _matrix* _pMatProj) override;
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const CCamera* _pCamera) override;

public:
	static CVIBuffer_TreeHead* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;

private:
	VTX_CUBETEXTURE*	m_pVertices = nullptr;
	VTX_CUBETEXTURE*	m_pConvertVertices = nullptr;
};

END

#endif // !__TREEHEAD_H__
