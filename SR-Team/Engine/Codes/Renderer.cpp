#include "GameObject.h"
#include "..\Headers\Renderer.h"

USING(Engine)

CRenderer::CRenderer(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: m_pDevice(_pDevice)
	, m_pSprite(_pSprite)
	, m_pFont(_pFont)
{
	Safe_AddRef(_pFont);
	Safe_AddRef(_pSprite);
	Safe_AddRef(_pDevice);
}

void CRenderer::Free()
{
	for (_int iCnt = 0; iCnt < RENDER_END; ++iCnt)
	{
		for (auto& pObject : m_GameObjects[iCnt])
			Safe_Release(pObject);

		m_GameObjects[iCnt].clear();
	}

	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
}


HRESULT CRenderer::Setup_Renderer()
{
	return S_OK;
}

HRESULT CRenderer::Add_RendererList(CRenderer::eRENDER_TYPE _eType, CGameObject * _pObject)
{
	if (nullptr == _pObject)
		return E_FAIL;

	if (0 > _eType || RENDER_END <= _eType)
		return E_FAIL;

	m_GameObjects[_eType].emplace_back(_pObject);
	Safe_AddRef(_pObject);

	return S_OK;
}

HRESULT CRenderer::Render_Renderer()
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	m_pDevice->BeginScene();

	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NoneAlpha()))
		return E_FAIL;

	if (FAILED(Render_OnlyAlpha()))
		return E_FAIL;

	if (FAILED(Render_BlendAlpha()))
		return E_FAIL;

	if (FAILED(Render_Effect()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pObject : m_GameObjects[RENDER_PRIORITY])
	{
		pObject->Render_Priority();
		Safe_Release(pObject);
	}

	m_GameObjects[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NoneAlpha()
{
	for (auto& pObject : m_GameObjects[RENDER_NONEALPHA])
	{
		pObject->Render_NoneAlpha();
		Safe_Release(pObject);
	}

	m_GameObjects[RENDER_NONEALPHA].clear();

	return S_OK;
}
              
HRESULT CRenderer::Render_BlendAlpha()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (auto& pObject : m_GameObjects[RENDER_BLNEDALPHA])
	{
		pObject->Render_BlendAlpha();
		Safe_Release(pObject);
	}

	m_GameObjects[RENDER_BLNEDALPHA].clear();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

HRESULT CRenderer::Render_OnlyAlpha()
{
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (auto& pObject : m_GameObjects[RENDER_ONLYALPHA])
	{
		pObject->Render_OnlyAlpha();
		Safe_Release(pObject);
	}

	m_GameObjects[RENDER_ONLYALPHA].clear();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CRenderer::Render_Effect()
{
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (auto& pObject : m_GameObjects[RENDER_EFFECT])
	{
		pObject->Render_Effect();
		Safe_Release(pObject);
	}

	m_GameObjects[RENDER_EFFECT].clear();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	//--------------------------------------------------
	// UI ·»´õÇÏ°í..
	//--------------------------------------------------
	for (auto& pObject : m_GameObjects[RENDER_UI])
	{
		pObject->Render_UI();
		Safe_Release(pObject);
	}

	m_GameObjects[RENDER_UI].clear();


	//--------------------------------------------------
	// ¸¶¿ì½º ·»´õ / Sprite BeginÇÑ ±è¿¡..
	//--------------------------------------------------
	for (auto& pObject : m_GameObjects[RENDER_MOUSE])
	{
		pObject->Render_UI();
		Safe_Release(pObject);
	}

	m_GameObjects[RENDER_MOUSE].clear();

	m_pSprite->End();

	return S_OK;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CRenderer* pInstance = new CRenderer(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_Renderer()))
	{
		PRINT_LOG(L"Failed To Create CRenderer", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}
