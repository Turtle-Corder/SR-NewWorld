#include "..\Headers\VIBuffer_CubeTexture.h"

USING(Engine)

CVIBuffer_CubeTexture::CVIBuffer_CubeTexture(LPDIRECT3DDEVICE9 _pDevice)
	: CVIBuffer(_pDevice)
{
}

CVIBuffer_CubeTexture::CVIBuffer_CubeTexture(const CVIBuffer_CubeTexture & _rOther)
	: CVIBuffer(_rOther)
	, m_pVTXOrigin(_rOther.m_pVTXOrigin)
	, m_pVTXConvert(_rOther.m_pVTXConvert)
	, m_pIndices(_rOther.m_pIndices)
{
}

void * CVIBuffer_CubeTexture::Get_Vertices() const
{
	return m_pVTXOrigin;
}

void * CVIBuffer_CubeTexture::Get_Indices() const
{
	return m_pIndices;
}

HRESULT CVIBuffer_CubeTexture::Setup_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_CUBETEXTURE);
	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;
	m_iFVF = VTX_CUBETEXTURE_FVF;
	m_iVertexCount = 8;
	m_iTriCount = 12;

	if (FAILED(CVIBuffer::Setup_Component_Prototype()))
		return E_FAIL;


	VTX_CUBETEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, -0.5f);
	pVertex[0].vUV = pVertex[0].vPosition;

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, -0.5f);
	pVertex[1].vUV = pVertex[1].vPosition;

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, -0.5f);
	pVertex[2].vUV = pVertex[2].vPosition;

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, -0.5f);
	pVertex[3].vUV = pVertex[3].vPosition;

	pVertex[4].vPosition = _vec3(-0.5f, 0.5f, 0.5f);
	pVertex[4].vUV = pVertex[4].vPosition;

	pVertex[5].vPosition = _vec3(0.5f, 0.5f, 0.5f);
	pVertex[5].vUV = pVertex[5].vPosition;

	pVertex[6].vPosition = _vec3(0.5f, -0.5f, 0.5f);
	pVertex[6].vUV = pVertex[6].vPosition;

	pVertex[7].vPosition = _vec3(-0.5f, -0.5f, 0.5f);
	pVertex[7].vUV = pVertex[7].vPosition;

	m_pVTXOrigin = new VTX_CUBETEXTURE[m_iVertexCount];
	memcpy_s(m_pVTXOrigin, sizeof(VTX_CUBETEXTURE) * m_iVertexCount, pVertex, sizeof(VTX_CUBETEXTURE) * m_iVertexCount);

	m_pVTXConvert = new VTX_CUBETEXTURE[m_iVertexCount];
	memcpy_s(m_pVTXConvert, sizeof(VTX_CUBETEXTURE) * m_iVertexCount, pVertex, sizeof(VTX_CUBETEXTURE) * m_iVertexCount);

	m_pVB->Unlock();


	INDEX16* pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 우
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// 좌
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// 상
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// 하
	pIndex[6]._1 = 6;
	pIndex[6]._2 = 7;
	pIndex[6]._3 = 3;

	pIndex[7]._1 = 6;
	pIndex[7]._2 = 3;
	pIndex[7]._3 = 2;

	// 앞
	pIndex[8]._1 = 0;
	pIndex[8]._2 = 1;
	pIndex[8]._3 = 2;

	pIndex[9]._1 = 0;
	pIndex[9]._2 = 2;
	pIndex[9]._3 = 3;

	// 뒤
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

HRESULT CVIBuffer_CubeTexture::Setup_Component(void * _pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_CubeTexture::Set_Transform_Nothing(const _matrix * _pMatWorld)
{

	for (_uint iCnt = 0; iCnt < m_iVertexCount; ++iCnt)
	{
		// World
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXOrigin[iCnt].vPosition, _pMatWorld);
	}
	VTX_CUBETEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy_s(pVertex, sizeof(VTX_CUBETEXTURE) * m_iVertexCount, m_pVTXConvert, sizeof(VTX_CUBETEXTURE) * m_iVertexCount);
	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_CubeTexture::Set_Transform(const _matrix * _pMatWorld, const CCamera * _pCamera)
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

	VTX_CUBETEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy_s(pVertex, sizeof(VTX_CUBETEXTURE) * m_iVertexCount, m_pVTXConvert, sizeof(VTX_CUBETEXTURE) * m_iVertexCount);
	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_CubeTexture::Set_Transform(const _matrix * _pMatWorld, const _matrix * _pMatView, const _matrix * _pMatProj)
{
	for (_uint iCnt = 0; iCnt < m_iVertexCount; ++iCnt)
	{
		// World
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXOrigin[iCnt].vPosition, _pMatWorld);

		// View
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, _pMatView);

		if (1.f >= m_pVTXConvert[iCnt].vPosition.z)
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

CVIBuffer_CubeTexture * CVIBuffer_CubeTexture::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if(nullptr == _pDevice)
		return nullptr;

	CVIBuffer_CubeTexture* pInstance = new CVIBuffer_CubeTexture(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CVIBuffer_CubeTexture", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_CubeTexture::Clone_Component(void * _pArg)
{
	CVIBuffer_CubeTexture* pInstance = new CVIBuffer_CubeTexture(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CVIBuffer_CubeTexture", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_CubeTexture::Free()
{
	if (!m_bIsClone)
	{
		Safe_Delete_Array(m_pVTXOrigin);
		Safe_Delete_Array(m_pVTXConvert);
		Safe_Delete_Array(m_pIndices);
	}

	CVIBuffer::Free();
}
