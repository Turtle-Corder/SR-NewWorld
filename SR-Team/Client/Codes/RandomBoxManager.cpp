#include "stdafx.h"
#include "RandomBox.h"
#include "..\Headers\RandomBoxManager.h"

USING(Client)

CRandomBoxManager* CRandomBoxManager::m_pInstance = nullptr;

CRandomBoxManager::CRandomBoxManager()
{
	Initialize();
}

CRandomBoxManager::~CRandomBoxManager()
{
	Release();
}

void CRandomBoxManager::Initialize()
{
	if (FAILED(Setup_PotionBox()))
	{
		PRINT_LOG(L"Failed To PotionBox Setup", LOG::CLIENT);
		return;
	}

	//if (FAILED(Setup_EquipBox()))
	//{
	//	PRINT_LOG(L"Failed To EquipBox Setup", LOG::CLIENT);
	//	return;
	//}
}

void CRandomBoxManager::Release()
{
	Safe_Release(m_pPotionBox);
	Safe_Release(m_pEquipBox);
}

_bool CRandomBoxManager::Gatcha_PotionBox(wstring & _strItemTag)
{
	if (nullptr == m_pPotionBox)
		return false;

	return m_pPotionBox->Gatcha(_strItemTag);
}

_bool CRandomBoxManager::Gatcha_EquipBox(wstring & _strItemTag)
{
	if (nullptr == m_pEquipBox)
		return false;

	return m_pEquipBox->Gatcha(_strItemTag);
}

_bool CRandomBoxManager::Setup_PotionBox()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;

	m_pPotionBox = CRandomBox::Create(pManagement->Get_Device());
	if (nullptr == m_pPotionBox)
		return false;

	if (!m_pPotionBox->Add_Item(L"Component_Texture_Item_RedPotion", 20))	return false;
	if (!m_pPotionBox->Add_Item(L"Component_Texture_Item_BluePotion", 20))	return false;

	if (!m_pPotionBox->Add_Item(L"Component_Texture_Item_RedElixir", 10))	return false;
	if (!m_pPotionBox->Add_Item(L"Component_Texture_Item_BlueElixir", 10))	return false;

	return true;
}

_bool CRandomBoxManager::Setup_EquipBox()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;

	m_pEquipBox = CRandomBox::Create(pManagement->Get_Device());
	if (nullptr == m_pEquipBox)
		return false;

	if (!m_pEquipBox->Add_Item(L"", 20))	return false;
	if (!m_pEquipBox->Add_Item(L"", 20))	return false;

	if (!m_pEquipBox->Add_Item(L"", 10))	return false;
	if (!m_pEquipBox->Add_Item(L"n", 10))	return false;

	// 10, 30, 50 : 무조건 10번째, 30번째, 50번째에 뽑힌다
	if (!m_pEquipBox->Add_Item(L"", 1, 10))	return false;
	if (!m_pEquipBox->Add_Item(L"", 1, 30))	return false;
	if (!m_pEquipBox->Add_Item(L"", 1, 50))	return false;

	return true;
}
