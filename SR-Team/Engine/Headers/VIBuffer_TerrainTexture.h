#pragma once
#ifndef __VIBUFFER_TERRAINTEXTURE_H__
#define __VIBUFFER_TERRAINTEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_TerrainTexture final : public CVIBuffer
{
private:
	explicit CVIBuffer_TerrainTexture(LPDIRECT3DDEVICE9 _pDevice);
	explicit CVIBuffer_TerrainTexture(const CVIBuffer_TerrainTexture& _rOther);
	virtual ~CVIBuffer_TerrainTexture() = default;

public:
	virtual void* Get_Vertices() const override;
	virtual void* Get_Indices() const override;

private:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component_Prototype(const wstring& _strFilePath);
	virtual HRESULT Setup_Component_Prototype(const wstring& _strFilePath, const wstring& _strHeightPath);
	virtual HRESULT Setup_Component(void* _pArg) override;

public:
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const CCamera* _pCamera) override;
	virtual HRESULT Set_Transform(const _matrix* _pMatWorld, const _matrix* _pMatView, const _matrix* _pMatProj) override;
	_bool IsOnTerrain(_vec3* _pInOutPos);

private:
	TILEINFO*	Load_TerrainData_FromFile(const wstring& _strFIlePath);
	_pixel*		Load_HeightMap_FromFile(const wstring& _strFilePath);

public:
	static CVIBuffer_TerrainTexture* Create(LPDIRECT3DDEVICE9 _pDevice, _uint _iVertexCountX, _uint _iVertexCountZ, _float _fVertexInterval = 1.f);
	static CVIBuffer_TerrainTexture* Create(LPDIRECT3DDEVICE9 _pDevice, const wstring& _strDataPath);
	static CVIBuffer_TerrainTexture* Create(LPDIRECT3DDEVICE9 _pDevice, const wstring& _strDataPath, const wstring& _strHeightPath);

	virtual CComponent* Clone_Component(void* _pArg) override;
	virtual void Free() override;


	
private:
	_uint	m_iVertexCountX		= 0;
	_uint	m_iVertexCountZ		= 0;
	_float	m_fVertexInterval	= 1.f;

	VTX_TEXTURE*	m_pVTXOrigin	= nullptr;
	VTX_TEXTURE*	m_pVTXConvert	= nullptr;
	INDEX16*		m_pIndices		= nullptr;
};

END

#endif
