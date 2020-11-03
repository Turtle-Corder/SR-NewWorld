#include "..\Headers\VIBuffer_CubeColor.h"

USING(Engine)



CVIBuffer_CubeColor::CVIBuffer_CubeColor(LPDIRECT3DDEVICE9 _pDevice, _uint _iColor)
	: CVIBuffer(_pDevice)
	, m_iColor(_iColor)
{
}

CVIBuffer_CubeColor::CVIBuffer_CubeColor(const CVIBuffer_CubeColor & _rOther)
	: CVIBuffer(_rOther)
	, m_pVTXOrigin(_rOther.m_pVTXOrigin)
	, m_pVTXConvert(_rOther.m_pVTXConvert)
	, m_pIndices(_rOther.m_pIndices)
	, m_iColor(_rOther.m_iColor)
{
}

void * CVIBuffer_CubeColor::Get_Vertices() const
{
	return m_pVTXOrigin;
}

void * CVIBuffer_CubeColor::Get_Indices() const
{
	return m_pIndices;
}

HRESULT CVIBuffer_CubeColor::Setup_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_COLOR);
	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;
	m_iFVF = VTX_CLOLOR_FVF;
	m_iVertexCount = 8;
	m_iTriCount = 12;

	if (FAILED(CVIBuffer::Setup_Component_Prototype()))
		return E_FAIL;

	VTX_COLOR* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVertex[0].iColor = m_iColor;

	pVertex[1].vPosition = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVertex[1].iColor = m_iColor;

	pVertex[2].vPosition = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVertex[2].iColor = m_iColor;

	pVertex[3].vPosition = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVertex[3].iColor = m_iColor;

	pVertex[4].vPosition = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVertex[4].iColor = m_iColor;

	pVertex[5].vPosition = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVertex[5].iColor = m_iColor;

	pVertex[6].vPosition = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVertex[6].iColor = m_iColor;

	pVertex[7].vPosition = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVertex[7].iColor = m_iColor;

	m_pVTXOrigin = new VTX_COLOR[m_iVertexCount];
	memcpy_s(m_pVTXOrigin, sizeof(VTX_COLOR) * m_iVertexCount, pVertex, sizeof(VTX_COLOR) * m_iVertexCount);

	m_pVTXConvert = new VTX_COLOR[m_iVertexCount];
	memcpy_s(m_pVTXConvert, sizeof(VTX_COLOR) * m_iVertexCount, pVertex, sizeof(VTX_COLOR) * m_iVertexCount);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	pIndex[6]._1 = 6;
	pIndex[6]._2 = 7;
	pIndex[6]._3 = 3;

	pIndex[7]._1 = 6;
	pIndex[7]._2 = 3;
	pIndex[7]._3 = 2;

	pIndex[8]._1 = 0;
	pIndex[8]._2 = 1;
	pIndex[8]._3 = 2;

	pIndex[9]._1 = 0;
	pIndex[9]._2 = 2;
	pIndex[9]._3 = 3;

	pIndex[10]._1 = 5;
	pIndex[10]._2 = 4;
	pIndex[10]._3 = 7;

	pIndex[11]._1 = 5;
	pIndex[11]._2 = 7;
	pIndex[11]._3 = 6;

	m_pIndices = new INDEX16[m_iTriCount];
	memcpy_s(m_pIndices, sizeof(INDEX16) * m_iTriCount, pIndex, sizeof(INDEX16) * m_iTriCount);

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_CubeColor::Setup_Component(void * _pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_CubeColor::Set_Transform(const _matrix * _pMatWorld, const CCamera * _pCamera)
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

HRESULT CVIBuffer_CubeColor::Set_Transform(const _matrix * _pMatWorld, const _matrix * _pMatView, const _matrix * _pMatProj)
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

CVIBuffer_CubeColor * CVIBuffer_CubeColor::Create(LPDIRECT3DDEVICE9 _pDevice, _uint _iColor)
{
	if (nullptr == _pDevice)
		return nullptr;

	CVIBuffer_CubeColor* pInstance = new CVIBuffer_CubeColor(_pDevice, _iColor);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CVIBuffer_CubeColor", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_CubeColor::Clone_Component(void * _pArg)
{
	CVIBuffer_CubeColor* pInstance = new CVIBuffer_CubeColor(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CVIBuffer_CubeColor", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_CubeColor::Free()
{
	if (!m_bIsClone)
	{
		Safe_Delete_Array(m_pVTXOrigin);
		Safe_Delete_Array(m_pVTXConvert);
		Safe_Delete_Array(m_pIndices);
	}

	CVIBuffer::Free();
}
