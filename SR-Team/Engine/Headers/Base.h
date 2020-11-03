#pragma once
#ifndef __BASE_H__
#define __BASE_H__

#include "Engine_Include.h"

BEGIN(Engine)

//--------------------------------------------------
// 레퍼런스 카운터 관리를 위한 레이어
//--------------------------------------------------
class ENGINE_DLL CBase abstract
{
protected:
	CBase(void);
	virtual ~CBase(void) = default;

public:
	_uint AddRef(void);
	_uint Release(void);

protected:
	virtual void Free(void) = 0;

protected:
	_uint m_iRefCnt		= 0;
	_bool m_bIsClone	= false;
};

END

#endif
