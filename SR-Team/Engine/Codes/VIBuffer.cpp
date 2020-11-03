#include "..\Headers\VIBuffer.h"

USING(Engine)

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & _rOther)
	: CComponent(_rOther)
	, m_pVB(_rOther.m_pVB)
	, m_pIB(_rOther.m_pIB)
	, m_iVertexSize(_rOther.m_iVertexSize)
	, m_iIndexSize(_rOther.m_iIndexSize)
	, m_iVertexCount(_rOther.m_iVertexCount)
	, m_iFVF(_rOther.m_iFVF)
	, m_iTriCount(_rOther.m_iTriCount)
	, m_IndexFmt(_rOther.m_IndexFmt)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

_uint CVIBuffer::Get_TriCount() const
{
	return m_iTriCount;
}

void * CVIBuffer::Get_Vertices() const
{
	return nullptr;
}

void * CVIBuffer::Get_Indices() const
{
	return nullptr;
}

HRESULT CVIBuffer::Setup_Component_Prototype()
{
	if (FAILED(m_pDevice->CreateVertexBuffer(
		m_iVertexSize * m_iVertexCount,
		0,
		m_iFVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		nullptr)))
	{
		PRINT_LOG(L"Failed To CreateVertexBuffer", LOG::SYSTEM);
		return E_FAIL;
	}

	if (FAILED(m_pDevice->CreateIndexBuffer(
		m_iIndexSize * m_iTriCount,
		0,
		m_IndexFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		nullptr)))
	{
		PRINT_LOG(L"Failed To CreateIndexBuffer", LOG::SYSTEM);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CVIBuffer::Setup_Component(void * pArg)
{
	return E_NOTIMPL;
}

HRESULT CVIBuffer::Render_VIBuffer()
{
	if (FAILED(m_pDevice->SetStreamSource(0, m_pVB, 0, m_iVertexSize)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetIndices(m_pIB)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetFVF(m_iFVF)))
		return E_FAIL;

	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

HRESULT CVIBuffer::Set_Transform(const _matrix * _pMatWorld, const _matrix * _pMatView, const _matrix * _pMatProj)
{
	return E_NOTIMPL;
}

HRESULT CVIBuffer::Set_Transform(const _matrix * _pMatWorld, const CCamera * _pCamera)
{
	return E_NOTIMPL;
}

CComponent * CVIBuffer::Clone_Component(void * _pArg)
{
	return nullptr;
}

void CVIBuffer::Free()
{
	if (Safe_Release(m_pVB))
	{
		if (!m_bIsClone)
			PRINT_LOG(L"Failed To Release Vertex Buffer", LOG::ENGINE);
	}

	if (Safe_Release(m_pIB))
	{
		if (!m_bIsClone)
			PRINT_LOG(L"Failed To Release Index Buffer", LOG::ENGINE);
	}

	CComponent::Free();
}
