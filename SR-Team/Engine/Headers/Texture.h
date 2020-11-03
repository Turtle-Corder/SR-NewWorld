#pragma once
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TEXTURE_TYPE
	{
		TEXTURE_NORMAL,
		TEXTURE_CUBE,
		TEXTURE_SPRITE,
	};

private:
	explicit CTexture(LPDIRECT3DDEVICE9 _pDevice, TEXTURE_TYPE _eTextureType, const wstring& _strFilePath, _uint _iMaxCount);
	explicit CTexture(const CTexture& _rOther);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void* _pArg) override;

	HRESULT Update_Frame(_float _fDeltaTime, _uint* _pCurFrame);

	_uint	Get_TextureCount();
	HRESULT SetTexture(_uint _iIndex);
	HRESULT SetFrameRange(_uint _iFrameBegin, _uint _iFrameEnd, _float _fFrameSpeed = 1.f);

	const IDirect3DBaseTexture9* GetTexture(_uint _iIndex) const;
	const IDirect3DBaseTexture9* Get_CurrentTexture() const;
	const D3DXIMAGE_INFO* Get_TexInfo(_uint _iIndex) const;


public:
	static CTexture* Create(LPDIRECT3DDEVICE9 _pDevice, TEXTURE_TYPE _eTextureType, const wstring& _strFilePath, _uint _iMaxCount = 1);
	virtual CComponent* Clone_Component(void* _pArg) override;
	virtual void Free() override;

private:
	typedef vector<IDirect3DBaseTexture9*> TEXTURES;
	TEXTURES m_Textures;

	vector<D3DXIMAGE_INFO*> m_TexInfo;

	TEXTURE_TYPE	m_eTextureType	= TEXTURE_NORMAL;
	wstring			m_strFilePath	= L"";
	_uint			m_iMaxCount		= 0;
	_uint			m_iCurCount		= 0;

	_float			m_fFrameSpeed		= 0.f;
	_float			m_fFrameBegin		= 0.f;
	_float			m_fFrameCur			= 0.f;
	_float			m_fFrameEnd			= 0.f;
};

END

#endif
