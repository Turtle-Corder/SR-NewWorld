#include "stdafx.h"
#include "..\Headers\VIBuffer_Flower.h"

USING(Client)

CVIBuffer_Flower::CVIBuffer_Flower(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_Flower::CVIBuffer_Flower(const CVIBuffer_Flower & other)
	: CVIBuffer(other)
	, m_pVertices(other.m_pVertices)
	, m_pConvertVertices(other.m_pConvertVertices)
{
}

void * CVIBuffer_Flower::Get_Vertices() const
{
	return m_pVertices;
}

void * CVIBuffer_Flower::Get_Indices() const
{
	return nullptr;
}

HRESULT CVIBuffer_Flower::Setup_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_CUBETEXTURE);
	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;
	m_iFVF = VTX_CUBETEXTURE_FVF;
	m_iVertexCount = 8;
	m_iTriCount = 12;

	if (FAILED(CVIBuffer::Setup_Component_Prototype()))
		return E_FAIL;

	VTX_CUBETEXTURE*	pVertex = nullptr;

	// 버텍스버퍼가 보관하고 있는 버텍스를 얻어오는 함수.
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = D3DXVECTOR3(-0.5f, 0.1f, -0.5f);
	pVertex[0].vUV = pVertex[0].vPosition;

	pVertex[1].vPosition = D3DXVECTOR3(0.2f, 0.1f, -0.2f);
	pVertex[1].vUV = pVertex[1].vPosition;

	pVertex[2].vPosition = D3DXVECTOR3(0.5f, -0.1f, -0.5f);
	pVertex[2].vUV = pVertex[2].vPosition;

	pVertex[3].vPosition = D3DXVECTOR3(-0.5f, -0.1f, -0.5f);
	pVertex[3].vUV = pVertex[3].vPosition;

	pVertex[4].vPosition = D3DXVECTOR3(-0.2f, 0.1f, 0.2f);
	pVertex[4].vUV = pVertex[4].vPosition;

	pVertex[5].vPosition = D3DXVECTOR3(0.5f, 0.1f, 0.5f);
	pVertex[5].vUV = pVertex[5].vPosition;

	pVertex[6].vPosition = D3DXVECTOR3(0.5f, -0.1f, 0.5f);
	pVertex[6].vUV = pVertex[6].vPosition;

	pVertex[7].vPosition = D3DXVECTOR3(-0.5f, -0.1f, 0.5f);
	pVertex[7].vUV = pVertex[7].vPosition;

	// 최초 버텍스 보관
	m_pVertices = new VTX_CUBETEXTURE[m_iVertexCount];
	memcpy(m_pVertices, pVertex, sizeof(VTX_CUBETEXTURE) * m_iVertexCount);

	m_pConvertVertices = new VTX_CUBETEXTURE[m_iVertexCount];
	memcpy(m_pConvertVertices, pVertex, sizeof(VTX_CUBETEXTURE) * m_iVertexCount);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x(오른쪽)
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// -x(왼쪽)
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// +y(위쪽)
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// -y(아래쪽)
	pIndex[6]._1 = 6;
	pIndex[6]._2 = 7;
	pIndex[6]._3 = 3;

	pIndex[7]._1 = 6;
	pIndex[7]._2 = 3;
	pIndex[7]._3 = 2;

	// +z(앞쪽)
	pIndex[8]._1 = 0;
	pIndex[8]._2 = 1;
	pIndex[8]._3 = 2;

	pIndex[9]._1 = 0;
	pIndex[9]._2 = 2;
	pIndex[9]._3 = 3;

	// -z(뒷쪽)
	pIndex[10]._1 = 5;
	pIndex[10]._2 = 4;
	pIndex[10]._3 = 7;

	pIndex[11]._1 = 5;
	pIndex[11]._2 = 7;
	pIndex[11]._3 = 6;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Flower::Setup_Component(void * _pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Flower::Set_Transform(const _matrix * _pMatWorld, const _matrix * _pMatView, const _matrix * _pMatProj)
{
	for (_uint i = 0; i < m_iVertexCount; ++i)
	{
		// 월드 변환
		CPipeline::TransformCoord(
			&m_pConvertVertices[i].vPosition,
			&m_pVertices[i].vPosition, _pMatWorld);

		// 뷰 변환
		CPipeline::TransformCoord(
			&m_pConvertVertices[i].vPosition,
			&m_pConvertVertices[i].vPosition, _pMatView);

		if (1.f >= m_pConvertVertices[i].vPosition.z)
			continue;

		CPipeline::TransformCoord(
			&m_pConvertVertices[i].vPosition,
			&m_pConvertVertices[i].vPosition, _pMatProj);
	}

	VTX_CUBETEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	memcpy(pVertex, m_pConvertVertices, sizeof(VTX_CUBETEXTURE) * m_iVertexCount);

	m_pVB->Unlock();
	return S_OK;
}

HRESULT CVIBuffer_Flower::Set_Transform(const _matrix * _pMatWorld, const CCamera * _pCamera)
{
	for (_uint i = 0; i < m_iVertexCount; ++i)
	{
		// 월드 변환
		CPipeline::TransformCoord(
			&m_pConvertVertices[i].vPosition,
			&m_pVertices[i].vPosition, _pMatWorld);

		// 뷰 변환
		CPipeline::TransformCoord(
			&m_pConvertVertices[i].vPosition,
			&m_pConvertVertices[i].vPosition, _pCamera->Get_ViewMatrix());

		if (1.f >= m_pConvertVertices[i].vPosition.z)
			continue;

		// 투영 변환 (Z나누기까지 해야 투영변환이 완료)
		CPipeline::TransformCoord(
			&m_pConvertVertices[i].vPosition,
			&m_pConvertVertices[i].vPosition, _pCamera->Get_ProjMatrix());
	}

	VTX_CUBETEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	memcpy(pVertex, m_pConvertVertices, sizeof(VTX_CUBETEXTURE) * m_iVertexCount);

	m_pVB->Unlock();
	return S_OK;
}

CVIBuffer_Flower * CVIBuffer_Flower::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CVIBuffer_Flower* pInstance = new CVIBuffer_Flower(pDevice);

	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CVIBuffer_CubeTexture", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Flower::Clone_Component(void * pArg)
{
	CVIBuffer_Flower* pInstance = new CVIBuffer_Flower(*this);
	if (FAILED(pInstance->Setup_Component(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CVIBuffer_CubeTexture", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Flower::Free()
{
	if (!m_bIsClone)
	{
		Safe_Delete_Array(m_pConvertVertices);
		Safe_Delete_Array(m_pVertices);
	}

	CVIBuffer::Free();
}
