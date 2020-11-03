#include "..\Headers\Key_Manager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CKey_Manager)

CKey_Manager::CKey_Manager()
{
}

bool CKey_Manager::Key_Pressing(_int _key)
{
	if (GetAsyncKeyState(_key) & 0x8000)
		return true;

	return false;
}

bool CKey_Manager::Key_Down(_int _key)
{
	if (!m_bKeyState[_key] && (GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = !m_bKeyState[_key];
		return true;
	}
	return false;
}

bool CKey_Manager::Key_Up(_int _key)
{
	if (m_bKeyState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = !m_bKeyState[_key];
		return true;
	}
	return false;
}

HRESULT CKey_Manager::Setup_KeyManager()
{
	ZeroMemory(m_bKeyState, sizeof(_bool) * VK_MAX);

	return S_OK;
}

void CKey_Manager::Key_Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
}

void CKey_Manager::Free()
{
}
