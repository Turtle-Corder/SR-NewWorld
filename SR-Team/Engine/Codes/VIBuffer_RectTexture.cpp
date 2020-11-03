#include "Pipeline.h"
#include "..\Headers\VIBuffer_RectTexture.h"

USING(Engine)

CVIBuffer_RectTexture::CVIBuffer_RectTexture(LPDIRECT3DDEVICE9 _pDevice)
	: CVIBuffer(_pDevice)
{
}

CVIBuffer_RectTexture::CVIBuffer_RectTexture(const CVIBuffer_RectTexture & _rOther)
	: CVIBuffer(_rOther)
	, m_pVTXOrigin(_rOther.m_pVTXOrigin)
	, m_pVTXConvert(_rOther.m_pVTXConvert)
	, m_pIndices(_rOther.m_pIndices)
{
}

void * CVIBuffer_RectTexture::Get_Vertices() const
{
	return m_pVTXOrigin;
}

void * CVIBuffer_RectTexture::Get_Indices() const
{
	return m_pIndices;
}

HRESULT CVIBuffer_RectTexture::Setup_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_TEXTURE);
	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;
	m_iFVF = VTX_TEXTURE_FVF;
	m_iVertexCount = 4;
	m_iTriCount = 2;

	if (FAILED(CVIBuffer::Setup_Component_Prototype()))
		return E_FAIL;

	//--------------------------------------------------
	// Vertex Buffer
	//--------------------------------------------------
	VTX_TEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vUV = _vec2(0.f, 1.f);

	m_pVTXOrigin = new VTX_TEXTURE[m_iVertexCount];
	memcpy_s(m_pVTXOrigin, sizeof(VTX_TEXTURE) * m_iVertexCount, pVertex, sizeof(VTX_TEXTURE) * m_iVertexCount);

	m_pVTXConvert = new VTX_TEXTURE[m_iVertexCount];
	memcpy_s(m_pVTXConvert, sizeof(VTX_TEXTURE) * m_iVertexCount, pVertex, sizeof(VTX_TEXTURE) * m_iVertexCount);

	m_pVB->Unlock();


	//--------------------------------------------------
	// Index Buffer
	//--------------------------------------------------
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

HRESULT CVIBuffer_RectTexture::Setup_Component(void * _pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_RectTexture::Set_Transform(const _matrix * _pMatWorld, const _matrix * _pMatView, const _matrix * _pMatProj)
{
	for (_uint iCnt = 0; iCnt < m_iVertexCount; ++iCnt)
	{
		// World
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXOrigin[iCnt].vPosition, _pMatWorld);

		// View
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, _pMatView);

		if(1.f >= m_pVTXConvert[iCnt].vPosition.z)
			continue;

		// Projection
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, _pMatProj);
	}

	VTX_TEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy_s(pVertex, sizeof(VTX_TEXTURE) * m_iVertexCount, m_pVTXConvert, sizeof(VTX_TEXTURE) * m_iVertexCount);
	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_RectTexture::Set_Transform(const _matrix * _pMatWorld, const CCamera * _pCamera)
{
	const _matrix* matView = _pCamera->Get_ViewMatrix();
	const _matrix* matProj = _pCamera->Get_ProjMatrix();

	for (_uint iCnt = 0; iCnt < m_iVertexCount; ++iCnt)
	{
		// World
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXOrigin[iCnt].vPosition, _pMatWorld);

		// View
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, matView);

		if (1.f >= m_pVTXConvert[iCnt].vPosition.z)
			continue;

		// Projection
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, matProj);
	}

	VTX_TEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy_s(pVertex, sizeof(VTX_TEXTURE) * m_iVertexCount, m_pVTXConvert, sizeof(VTX_TEXTURE) * m_iVertexCount);
	m_pVB->Unlock();

	return S_OK;
}

CVIBuffer_RectTexture * CVIBuffer_RectTexture::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CVIBuffer_RectTexture* pInstance = new CVIBuffer_RectTexture(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CVIBuffer_RectTexture", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_RectTexture::Clone_Component(void * _pArg)
{
	CVIBuffer_RectTexture* pInstance = new CVIBuffer_RectTexture(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CVIBuffer_RectTexture", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RectTexture::Free()
{
	if (!m_bIsClone)
	{
		Safe_Delete_Array(m_pVTXConvert);
		Safe_Delete_Array(m_pVTXOrigin);
		Safe_Delete_Array(m_pIndices);
	}

	CVIBuffer::Free();
}
