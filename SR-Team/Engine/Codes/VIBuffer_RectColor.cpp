#include "..\Headers\VIBuffer_RectColor.h"

USING(Engine)

CVIBuffer_RectColor::CVIBuffer_RectColor(LPDIRECT3DDEVICE9 _pDevice, _uint _iColor)
	: CVIBuffer(_pDevice)
	, m_iColor(_iColor)
{
}

CVIBuffer_RectColor::CVIBuffer_RectColor(const CVIBuffer_RectColor & _rOther)
	: CVIBuffer(_rOther)
	, m_pVTXOrigin(_rOther.m_pVTXOrigin)
	, m_pVTXConvert(_rOther.m_pVTXConvert)
	, m_pIndices(_rOther.m_pIndices)
	, m_iColor(_rOther.m_iColor)
{
}

void * CVIBuffer_RectColor::Get_Vertices() const
{
	return m_pVTXOrigin;
}

void * CVIBuffer_RectColor::Get_Indices() const
{
	return m_pIndices;
}

HRESULT CVIBuffer_RectColor::Setup_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_COLOR);
	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;
	m_iFVF = VTX_CLOLOR_FVF;
	m_iVertexCount = 4;
	m_iTriCount = 2;

	if (FAILED(CVIBuffer::Setup_Component_Prototype()))
		return E_FAIL;


	VTX_COLOR* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].iColor = m_iColor;

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].iColor = m_iColor;

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].iColor = m_iColor;

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].iColor = m_iColor;

	m_pVTXOrigin = new VTX_COLOR[m_iVertexCount];
	memcpy_s(m_pVTXOrigin, sizeof(VTX_COLOR) * m_iVertexCount, pVertex, sizeof(VTX_COLOR) * m_iVertexCount);

	m_pVTXConvert = new VTX_COLOR[m_iVertexCount];
	memcpy_s(m_pVTXConvert, sizeof(VTX_COLOR) * m_iVertexCount, pVertex, sizeof(VTX_COLOR) * m_iVertexCount);

	m_pVB->Unlock();


	INDEX16* pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	
	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIndices = new INDEX16[m_iTriCount];
	memcpy_s(m_pIndices, sizeof(INDEX16) * m_iTriCount, pIndex, sizeof(INDEX16) * m_iTriCount);

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_RectColor::Setup_Component(void * _pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_RectColor::Set_Transform(const _matrix * _pMatWorld, const CCamera * _pCamera)
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

HRESULT CVIBuffer_RectColor::Set_Transform(const _matrix * _pMatWorld, const _matrix * _pMatView, const _matrix * _pMatProj)
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

CVIBuffer_RectColor * CVIBuffer_RectColor::Create(LPDIRECT3DDEVICE9 _pDevice, _uint _iColor)
{
	if (nullptr == _pDevice)
		return nullptr;

	CVIBuffer_RectColor* pInstance = new CVIBuffer_RectColor(_pDevice, _iColor);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CVIBuffer_RectColor", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_RectColor::Clone_Component(void * _pArg)
{
	CVIBuffer_RectColor* pInstance = new CVIBuffer_RectColor(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CVIBuffer_RectColor", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RectColor::Free()
{
	if (!m_bIsClone)
	{
		Safe_Delete_Array(m_pVTXOrigin);
		Safe_Delete_Array(m_pVTXConvert);
		Safe_Delete_Array(m_pIndices);
	}

	CVIBuffer::Free();
}