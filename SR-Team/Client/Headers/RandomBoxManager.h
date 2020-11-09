#pragma once
#ifndef __RANDOMBOX_MANAGER_H__
#define __RANDOMBOX_MANAGER_H__

USING(Engine)
BEGIN(Client)

class CRandomBox;
class CRandomBoxManager
{
private:
	CRandomBoxManager();
	~CRandomBoxManager();

public:
	static CRandomBoxManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CRandomBoxManager;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

public:
	void Initialize();
	void Release();

	_bool Gatcha_PotionBox(wstring& _strItemTag);
	_bool Gatcha_EquipBox(wstring& _strItemTag);

private:
	_bool Setup_PotionBox();
	_bool Setup_EquipBox();

private:
	static CRandomBoxManager*	m_pInstance;

	CRandomBox*	m_pPotionBox = nullptr;
	CRandomBox*	m_pEquipBox = nullptr;
};

END

#endif // !__RANDOMBOX_MANAGER_H__
