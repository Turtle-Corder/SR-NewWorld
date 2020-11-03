#include "..\Headers\VIBuffer_TriColor.h"

USING(Engine)

CVIBuffer_TriColor::CVIBuffer_TriColor(LPDIRECT3DDEVICE9 _pDevice, _uint _iColor)
	: CVIBuffer(_pDevice)
	, m_iColor(_iColor)
{
}

CVIBuffer_TriColor::CVIBuffer_TriColor(const CVIBuffer_TriColor & _rOther)
	: CVIBuffer(_rOther)
	, m_pVTXOrigin(_rOther.m_pVTXOrigin)
	, m_pVTXConvert(_rOther.m_pVTXConvert)
	, m_iColor(_rOther.m_iColor)
{
}

HRESULT CVIBuffer_TriColor::Setup_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_COLOR);
	m_iIndexSize = sizeof(INDEX16);
	m_iFVF = VTX_CLOLOR_FVF;
	m_iVertexCount = 3;
	m_iTriCount = 1;

	if (FAILED(CVIBuffer::Setup_Component_Prototype()))
		return E_FAIL;

	VTX_COLOR* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(0.f, 1.f, 0.f);
	pVertex[0].iColor = m_iColor;

	pVertex[1].vPosition = _vec3(1.f, -1.f, 0.f);
	pVertex[1].iColor = m_iColor;

	pVertex[2].vPosition = _vec3(-1.f, -1.f, 0.f);
	pVertex[2].iColor = m_iColor;

	m_pVTXOrigin = new VTX_COLOR[m_iVertexCount];
	memcpy_s(m_pVTXOrigin, sizeof(VTX_COLOR) * m_iVertexCount, pVertex, sizeof(VTX_COLOR) * m_iVertexCount);

	m_pVTXConvert = new VTX_COLOR[m_iVertexCount];
	memcpy_s(m_pVTXConvert, sizeof(VTX_COLOR) * m_iVertexCount, pVertex, sizeof(VTX_COLOR) * m_iVertexCount);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_TriColor::Setup_Component(void * _pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_TriColor::Set_Transform(const _matrix * _pMatWorld, const CCamera * _pCamera)
{
	const _matrix* matView = _pCamera->Get_ViewMatrix();
	const _matrix* matProj = _pCamera->Get_ProjMatrix();

	for (_uint iCnt = 0; iCnt < m_iVertexCount; ++iCnt)
	{
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXOrigin[iCnt].vPosition, _pMatWorld);

		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, matView);

		if (1.f >= m_pVTXConvert[iCnt].vPosition.z)
			continue;

		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, matProj);
	}

	VTX_COLOR* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy_s(pVertex, sizeof(VTX_COLOR) * m_iVertexCount, m_pVTXConvert, sizeof(VTX_COLOR) * m_iVertexCount);
	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_TriColor::Set_Transform(const _matrix * _pMatWorld, const _matrix * _pMatView, const _matrix * _pMatProj)
{
	for (_uint iCnt = 0; iCnt < m_iVertexCount; ++iCnt)
	{
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXOrigin[iCnt].vPosition, _pMatWorld);

		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, _pMatView);

		if (1.f >= m_pVTXConvert[iCnt].vPosition.z)
			continue;

		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, _pMatProj);
	}

	VTX_COLOR* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy_s(pVertex, sizeof(VTX_COLOR) * m_iVertexCount, m_pVTXConvert, sizeof(VTX_COLOR) * m_iVertexCount);
	m_pVB->Unlock();

	return S_OK;
}

void * CVIBuffer_TriColor::Get_Vertices() const
{
	return m_pVTXOrigin;
}

void * CVIBuffer_TriColor::Get_Indices() const
{
	return nullptr;
}

CVIBuffer_TriColor * CVIBuffer_TriColor::Create(LPDIRECT3DDEVICE9 _pDevice, _uint _iColor)
{
	if (nullptr == _pDevice)
		return nullptr;

	CVIBuffer_TriColor* pInstance = new CVIBuffer_TriColor(_pDevice, _iColor);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CVIBuffer_TriColor", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_TriColor::Clone_Component(void * _pArg)
{
	CVIBuffer_TriColor* pInstance = new CVIBuffer_TriColor(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CVIBuffer_TriColor", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_TriColor::Free()
{
	if (!m_bIsClone)
	{
		Safe_Delete_Array(m_pVTXOrigin);
		Safe_Delete_Array(m_pVTXConvert);
	}

	CVIBuffer::Free();
}
