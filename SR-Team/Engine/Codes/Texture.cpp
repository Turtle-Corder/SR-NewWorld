#include "..\Headers\Texture.h"

USING(Engine)

CTexture::CTexture(LPDIRECT3DDEVICE9 _pDevice, TEXTURE_TYPE _eTextureType, const wstring & _strFilePath, _uint _iMaxCount)
	: CComponent(_pDevice)
	, m_eTextureType(_eTextureType)
	, m_strFilePath(_strFilePath)
	, m_iMaxCount(_iMaxCount)
	, m_iCurCount(0)
	, m_fFrameSpeed(0.f)
	, m_fFrameBegin(0.f)
	, m_fFrameCur(0.f)
	, m_fFrameEnd((_float)_iMaxCount)
{
}

CTexture::CTexture(const CTexture & _rOther)
	: CComponent(_rOther)
	, m_Textures(_rOther.m_Textures)
	, m_TexInfo(_rOther.m_TexInfo)
	, m_eTextureType(_rOther.m_eTextureType)
	, m_strFilePath(_rOther.m_strFilePath)
	, m_iMaxCount(_rOther.m_iMaxCount)
	, m_iCurCount(_rOther.m_iCurCount)
	, m_fFrameSpeed(_rOther.m_fFrameSpeed)
	, m_fFrameBegin(_rOther.m_fFrameBegin)
	, m_fFrameCur(_rOther.m_fFrameCur)
	, m_fFrameEnd(_rOther.m_fFrameEnd)
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);
}

HRESULT CTexture::Setup_Component_Prototype()
{
	IDirect3DBaseTexture9* pTexture = nullptr;
	D3DXIMAGE_INFO* pTexInfo = nullptr;
	TCHAR szFullPath[MAX_STR] = L"";
	HRESULT hr = 0;

	for (_uint iCnt = 0; iCnt < m_iMaxCount; ++iCnt)
	{
		StringCchPrintf(szFullPath, _countof(szFullPath), m_strFilePath.c_str(), iCnt);

		switch (m_eTextureType)
		{
		case Engine::CTexture::TEXTURE_NORMAL:
			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;

		case Engine::CTexture::TEXTURE_CUBE:
			hr = D3DXCreateCubeTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
			break;

		case Engine::CTexture::TEXTURE_SPRITE:
		{

			pTexInfo = new D3DXIMAGE_INFO;
			ZeroMemory(pTexInfo, sizeof(D3DXIMAGE_INFO));

			hr = D3DXGetImageInfoFromFile(szFullPath, pTexInfo);
			if (FAILED(hr))	break;

			hr = D3DXCreateTextureFromFileEx(m_pDevice,
				szFullPath,
				pTexInfo->Width,
				pTexInfo->Height,
				pTexInfo->MipLevels,
				0,
				pTexInfo->Format,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0,
				nullptr,
				nullptr,
				(LPDIRECT3DTEXTURE9*)&pTexture);
		}
		break;

		}

		if (FAILED(hr))
		{
			PRINT_LOG(szFullPath, LOG::SYSTEM);
			return E_FAIL;
		}

		m_Textures.emplace_back(pTexture);

		if (m_eTextureType == TEXTURE_TYPE::TEXTURE_SPRITE)
			m_TexInfo.emplace_back(pTexInfo);
	}

	return S_OK;
}

HRESULT CTexture::Setup_Component(void * _pArg)
{
	return S_OK;
}

HRESULT CTexture::Update_Frame(_float _fDeltaTime, _uint* _pCurFrame)
{
	m_fFrameCur += m_fFrameEnd * m_fFrameSpeed * _fDeltaTime;
	if (m_fFrameCur > m_fFrameEnd)
	{
		m_fFrameCur = m_fFrameBegin;
	}

	*_pCurFrame = (_uint)m_fFrameCur;

	return S_OK;
}

_uint CTexture::Get_TextureCount()
{
	return (_uint)m_Textures.size();
}

HRESULT CTexture::SetTexture(_uint _iIndex)
{
	if (m_iMaxCount <= _iIndex)
		return E_FAIL;

	m_iCurCount = _iIndex;

	return m_pDevice->SetTexture(0, m_Textures[_iIndex]);
}

HRESULT CTexture::SetFrameRange(_uint _iFrameBegin, _uint _iFrameEnd, _float _fFrameSpeed)
{
	if (_iFrameBegin + 1 > m_iMaxCount || _iFrameEnd + 1 > m_iMaxCount)
	{
		PRINT_LOG(L"Inavlidate Frame Range", LOG::ENGINE);
		return E_FAIL;
	}

	m_fFrameSpeed = _fFrameSpeed;
	m_fFrameCur = m_fFrameBegin = (_float)_iFrameBegin;
	m_fFrameEnd = (_float)_iFrameEnd + 1;
	return S_OK;
}

const IDirect3DBaseTexture9 * CTexture::GetTexture(_uint _iIndex) const
{
	if (m_iMaxCount <= _iIndex)
		return nullptr;

	return m_Textures[_iIndex];
}

const IDirect3DBaseTexture9 * CTexture::Get_CurrentTexture() const
{
	return m_Textures[m_iCurCount];
}

const D3DXIMAGE_INFO* CTexture::Get_TexInfo(_uint _iIndex) const
{
	if (m_iMaxCount <= _iIndex)
		return nullptr;

	return m_TexInfo[_iIndex];
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 _pDevice, TEXTURE_TYPE _eTextureType, const wstring & _strFilePath, _uint _iMaxCount)
{
	if (nullptr == _pDevice)
		return nullptr;

	CTexture* pInstance = new CTexture(_pDevice, _eTextureType, _strFilePath, _iMaxCount);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CTexture", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTexture::Clone_Component(void * _pArg)
{
	CTexture* pInstance = new CTexture(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone_Component", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

	m_Textures.clear();

	if (!m_bIsClone)
	{
		for (auto& pTexInfo : m_TexInfo)
			Safe_Delete(pTexInfo);

		m_TexInfo.clear();
	}

	CComponent::Free();
}
