#include "stdafx.h"
#include "..\Headers\RandomBox_Chat.h"
#include "RandomBoxManager.h"
#include "Mouse.h"
#include "Inventory.h"

USING(Client)

CRandomBox_Chat::CRandomBox_Chat(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < RANDOMBOX_END; ++i)
		m_pTextureWnd[i] = nullptr;
	for (_uint i = 0; i < END_BOX; ++i)
	{
		m_pTextureInfo[i] = nullptr;
		m_pTextureBox[i] = nullptr;
	}
}

CRandomBox_Chat::CRandomBox_Chat(const CRandomBox_Chat & other)
	: CUIObject(other)
{
}

HRESULT CRandomBox_Chat::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CRandomBox_Chat::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return S_OK;

	m_vBoxPos[POTION_BOX] = { WINCX* 0.4f, WINCY * 0.5f, 0.f };
	m_vBoxPos[EQUIP_BOX] = { WINCX * 0.6f, WINCY * 0.5f, 0.f };

	return S_OK;
}

_int CRandomBox_Chat::Update_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (pMouse == nullptr)
		return E_FAIL;

	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
	if (pInven == nullptr)
		return E_FAIL;

	switch (m_eSituation)
	{
	case RANDOMBOX_GREETINGS:
		if (pManagement->Key_Down(VK_LBUTTON) || pManagement->Key_Down(VK_SPACE))
		{
			//if (!m_bEnd)
			m_eSituation = RANDOMBOX_ASK1;
			//else
			//{
			//	m_eSituation = RANDOMBOX_END;
			//	m_bStartChat = false;
			//}
		}
		if (pManagement->Key_Pressing(VK_ESCAPE))
		{
			m_eSituation = RANDOMBOX_FINISH;
			m_bStartChat = false;
		}
		break;

	case RANDOMBOX_ASK1:
		// 수락
		if (pManagement->Key_Pressing(VK_RETURN))
			m_eSituation = RANDOMBOX_ASK1_AGREE;
		else if (pManagement->Key_Pressing(VK_ESCAPE))
			m_eSituation = RANDOMBOX_ASK1_REJECT;
		break;

	case RANDOMBOX_ASK1_REJECT:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
		{
			m_eSituation = RANDOMBOX_END;
			m_bStartChat = false;
		}
		break;

	case RANDOMBOX_ASK1_AGREE:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
			m_eSituation = RANDOMBOX_ASK2;
		break;

	case RANDOMBOX_ASK2:
		if (pManagement->Key_Pressing(VK_RETURN))
			m_eSituation = RANDOMBOX_ASK2_AGREE;
		else if (pManagement->Key_Pressing(VK_ESCAPE))
			m_eSituation = RANDOMBOX_ASK2_REJECT;
		break;

	case RANDOMBOX_ASK2_REJECT:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
		{
			m_eSituation = RANDOMBOX_END;
			m_bStartChat = false;
		}
		break;

	case RANDOMBOX_ASK2_AGREE:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Pressing(VK_LBUTTON))
			m_eSituation = RANDOMBOX_SHOW_SORT;
		break;

	case RANDOMBOX_SHOW_SORT:
		m_bIsColl = false;
		// 랜덤박스들과 충돌 확인해야 함
		for (_uint i = 0; i < END_BOX; ++i)
		{
			if (PtInRect(&m_tRBoxCollRt[i], pMouse->Get_Point()))
			{
				m_bIsColl = true;
				if (i == POTION_BOX)
				{
					m_bRenderEquipInfo = false;
					m_bRenderPotionInfo = true;
				}
				else if (i == EQUIP_BOX)
				{
					m_bRenderEquipInfo = true;
					m_bRenderPotionInfo = false;
				}

				if (pManagement->Key_Pressing(VK_LBUTTON))
				{
					if (i == 0)
						m_eSelectedBox = POTION_BOX;
					if (i == 1)
						m_eSelectedBox = EQUIP_BOX;

					m_eSituation = RANDOMBOX_OPEN;

					m_bRenderEquipInfo = false;
					m_bRenderPotionInfo = false;

					m_bRenderClearWnd = true;
				}
			}
			if (!m_bIsColl)
			{
				m_bRenderEquipInfo = false;
				m_bRenderPotionInfo = false;
			}
		}
		break;

	case RANDOMBOX_OPEN:
		if (m_eSelectedBox == POTION_BOX)
		{
			pInven->Get_RewardItem(L"Potion_Box");
			//if (FAILED(Getcha_PotionBox()))
			//	return GAMEOBJECT::WARN;
		}
		else if (m_eSelectedBox == EQUIP_BOX)
		{
			pInven->Get_RewardItem(L"Equip_Box");
			//if (FAILED(Getcha_EquipBox()))
			//	return GAMEOBJECT::WARN;
		}
		pInven->Set_Gold(-500);
		m_eSituation = RANDOMBOX_GET;
		break;

	case RANDOMBOX_GET:
		if (pManagement->Key_Pressing(VK_RETURN))
		{
			m_bRenderClearWnd = false;
			m_eSituation = RANDOMBOX_FINAL;
		}

	case RANDOMBOX_FINAL:
		if (pManagement->Key_Down(VK_SPACE) || pManagement->Key_Down(VK_LBUTTON))
		{
			m_eSituation = RANDOMBOX_END;
			m_bStartChat = false;
		}
		break;

	case RANDOMBOX_END:
		if (m_bStartChat)
			m_eSituation = RANDOMBOX_GREETINGS;
		break;
	}

	return GAMEOBJECT::NOEVENT;
}

_int CRandomBox_Chat::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CRandomBox_Chat::Render_UI()
{
	if (m_eSituation != RANDOMBOX_END && m_eSituation != RANDOMBOX_SHOW_SORT && m_eSituation != RANDOMBOX_FINISH &&
		m_eSituation != RANDOMBOX_OPEN && m_eSituation != RANDOMBOX_GET)
	{
		D3DXMATRIX matTrans, matWorld;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matTrans);
		const D3DXIMAGE_INFO* pTexInfo = m_pTextureWnd[m_eSituation]->Get_TexInfo(0);
		if (nullptr == pTexInfo)
			return E_FAIL;

		_vec3 vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };

		D3DXMatrixTranslation(&matTrans, 950.f, 800.f, 0.f);
		matWorld = matTrans;

		m_pSprite->SetTransform(&matWorld);
		m_pSprite->Draw(
			(LPDIRECT3DTEXTURE9)m_pTextureWnd[m_eSituation]->GetTexture(0),
			nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_bChat = true;
	}
	else
		m_bChat = false;

	if (FAILED(Render_RandomBox()))
		return E_FAIL;
	if (FAILED(Render_RandomBoxInfo()))
		return E_FAIL;
	if (FAILED(Render_ClearWnd()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRandomBox_Chat::Render_RandomBox()
{
	if (m_eSituation == RANDOMBOX_SHOW_SORT)
	{
		for (_uint i = 0; i < END_BOX; ++i)
		{
			D3DXMATRIX matTrans, matWorld;
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureBox[i]->Get_TexInfo(0);
			if (nullptr == pTexInfo)
				return E_FAIL;

			_vec3 vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };

			D3DXMatrixTranslation(&matTrans, m_vBoxPos[i].x, m_vBoxPos[i].y, 0.f);
			matWorld = matTrans;

			m_tRBoxCollRt[i].left = (LONG)(m_vBoxPos[i].x - vCenter.x);
			m_tRBoxCollRt[i].right = (LONG)(m_vBoxPos[i].x + vCenter.x);
			m_tRBoxCollRt[i].top = (LONG)(m_vBoxPos[i].y - vCenter.y);
			m_tRBoxCollRt[i].bottom = (LONG)(m_vBoxPos[i].y + vCenter.y);

			m_pSprite->SetTransform(&matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureBox[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		m_bChat = true;
	}
	return S_OK;
}

HRESULT CRandomBox_Chat::Render_RandomBoxInfo()
{
	_int iIndex = -1;
	if (m_bRenderEquipInfo)
		iIndex = 1;
	else if (m_bRenderPotionInfo)
		iIndex = 0;

	if (iIndex != -1 && m_eSituation == RANDOMBOX_SHOW_SORT)
	{
		D3DXMATRIX matTrans, matWorld;
		const D3DXIMAGE_INFO* pTexInfo = m_pTextureInfo[iIndex]->Get_TexInfo(0);
		if (nullptr == pTexInfo)
			return E_FAIL;

		_vec3 vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };

		if (iIndex == 0)
			D3DXMatrixTranslation(&matTrans, m_vBoxPos[iIndex].x - 200.f, m_vBoxPos[iIndex].y, 0.f);
		else
			D3DXMatrixTranslation(&matTrans, m_vBoxPos[iIndex].x + 200.f, m_vBoxPos[iIndex].y, 0.f);
		matWorld = matTrans;

		m_pSprite->SetTransform(&matWorld);
		m_pSprite->Draw(
			(LPDIRECT3DTEXTURE9)m_pTextureInfo[iIndex]->GetTexture(0),
			nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	return S_OK;
}

HRESULT CRandomBox_Chat::Render_ClearWnd()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (m_bRenderClearWnd)
	{
		D3DXMATRIX matTrans, matWorld;
		const D3DXIMAGE_INFO* pTexInfo = m_pTextureClear->Get_TexInfo(0);
		if (nullptr == pTexInfo)
			return E_FAIL;

		_vec3 vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };

		D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);
		matWorld = matTrans;

		m_pSprite->SetTransform(&matWorld);
		m_pSprite->Draw(
			(LPDIRECT3DTEXTURE9)m_pTextureClear->GetTexture(0),
			nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (pManagement->Key_Pressing(VK_RETURN))
			m_bRenderClearWnd = false;
	}

	return S_OK;
}

HRESULT CRandomBox_Chat::Add_Component()
{
	// 대화 윈도우
	TCHAR szTextureNameTag[][MAX_STR] =
	{
		L"Component_Texture_RandomBoxChat_Greeting",

		L"Component_Texture_RandomBoxChat_Ask1",
		L"Component_Texture_RandomBoxChat_Ask1_Reject",
		L"Component_Texture_RandomBoxChat_Ask1_Agree",

		L"Component_Texture_RandomBoxChat_Ask2",
		L"Component_Texture_RandomBoxChat_Ask2_Reject",
		L"Component_Texture_RandomBoxChat_Ask2_Agree",

		L"Component_Texture_RandomBoxChat_Final",
	};
	TCHAR szTexture[MIN_STR] = L"Com_TextureWnd%d";
	TCHAR szCombine[MIN_STR] = L"";

	// 랜덤박스 2개
	TCHAR szTextureBoxTag[][MAX_STR] =
	{
		L"Component_Texture_RandomBoxChat_PotionBox",

		L"Component_Texture_RandomBoxChat_EquipBox",
	};
	TCHAR szTextureBox[MIN_STR] = L"Com_TextureBox%d";
	TCHAR szCombineBox[MIN_STR] = L"";

	// 랜덤박스 2개 설명
	TCHAR szTextureBoxInfoTag[][MAX_STR] =
	{
		L"Component_Texture_RandomBoxChat_PotionBoxInfo",

		L"Component_Texture_RandomBoxChat_EquipBoxInfo",
	};
	TCHAR szTextureInfoBox[MIN_STR] = L"Com_TextureBoxInfo%d";
	TCHAR szCombineInfoBox[MIN_STR] = L"";

	for (_uint i = 0; i < RANDOMBOX_END; i++)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureNameTag[i],
			szCombine, (CComponent**)&m_pTextureWnd[i])))
			return E_FAIL;
	}

	for (_uint i = 0; i < END_BOX; i++)
	{
		StringCchPrintf(szCombineBox, _countof(szCombineBox), szTextureBox, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureBoxTag[i],
			szCombineBox, (CComponent**)&m_pTextureBox[i])))
			return E_FAIL;
	}

	for (_uint i = 0; i < END_BOX; i++)
	{
		StringCchPrintf(szCombineInfoBox, _countof(szCombineInfoBox), szTextureInfoBox, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureBoxInfoTag[i],
			szCombineInfoBox, (CComponent**)&m_pTextureInfo[i])))
			return E_FAIL;
	}

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_RandomBoxChat_ClearWnd",
		L"Com_TextureClear", (CComponent**)&m_pTextureClear)))
		return E_FAIL;

	return S_OK;
}

CRandomBox_Chat * CRandomBox_Chat::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CRandomBox_Chat* pInstance = new CRandomBox_Chat(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CRandomBox_Chat", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRandomBox_Chat::Clone_GameObject(void * _pArg)
{
	CRandomBox_Chat* pInstance = new CRandomBox_Chat(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CRandomBox_Chat", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRandomBox_Chat::Free()
{
	for (_uint i = 0; i < RANDOMBOX_END; ++i)
		Safe_Release(m_pTextureWnd[i]);
	for (_uint i = 0; i < END_BOX; ++i)
	{
		Safe_Release(m_pTextureBox[i]);
		Safe_Release(m_pTextureInfo[i]);
	}

	Safe_Release(m_pTextureClear);

	CUIObject::Free();
}

HRESULT CRandomBox_Chat::Getcha_PotionBox()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
	if (pInven == nullptr)
		return E_FAIL;

	wstring strGetcha;
	CRandomBoxManager::Get_Instance()->Get_Instance()->Gatcha_PotionBox(strGetcha);

	if (pManagement != nullptr)
	{
		if (strGetcha == L"Component_Texture_Item_RedPotion")
			strGetcha = L"RedPotion";
		else if (strGetcha == L"Component_Texture_Item_BluePotion")
			strGetcha = L"BluePotion";
		else if (strGetcha == L"Component_Texture_Item_RedElixir")
			strGetcha = L"RedElixir";
		else if (strGetcha == L"Component_Texture_Item_BlueElixir")
			strGetcha = L"BlueElixir";

		pInven->Get_RewardItem(strGetcha);
		pInven->Set_Gold(-500);
		
		m_eSituation = RANDOMBOX_GET;
	}

	return S_OK;
}

HRESULT CRandomBox_Chat::Getcha_EquipBox()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory");
	if (pInven == nullptr)
		return E_FAIL;

	wstring strGetcha;
	CRandomBoxManager::Get_Instance()->Get_Instance()->Gatcha_EquipBox(strGetcha);

	if (pManagement != nullptr)
	{
		if (strGetcha == L"Component_Texture_Item_MagicalStaff")
			strGetcha = L"MagicalStaff";
		else if (strGetcha == L"Component_Texture_Item_AquaGloves")
			strGetcha = L"AquaGloves";
		else if (strGetcha == L"Component_Texture_Item_PupleDress")
			strGetcha = L"PupleDress";
		else if (strGetcha == L"Component_Texture_Item_BalrogWings")
			strGetcha = L"BalrogWings";

		pInven->Get_RewardItem(strGetcha);
		pInven->Set_Gold(-500);

		m_eSituation = RANDOMBOX_GET;
	}

	return S_OK;
}
