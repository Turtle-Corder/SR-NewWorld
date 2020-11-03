#pragma once
#ifndef __KEY_MANAGER_H__
#define __KEY_MANAGER_H__

#include "Base.h"

BEGIN(Engine)

#define VK_MAX 0xff

class CKey_Manager final : public CBase
{
	DECLARE_SINGLETON(CKey_Manager)

private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	HRESULT Setup_KeyManager();
	void Key_Update();

public:
	_bool Key_Pressing(_int _key);
	_bool Key_Down(_int _key);
	_bool Key_Up(_int _key);

public:
	virtual void Free() override;


private:
	_bool	m_bKeyState[VK_MAX];
};

END

#endif // !__KEY_MANAGER_H__
