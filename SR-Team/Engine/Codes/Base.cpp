#include "..\Headers\Base.h"

USING(Engine)

CBase::CBase(void)
{
}

_uint CBase::AddRef(void)
{
	return ++m_iRefCnt;
}

_uint CBase::Release(void)
{
	if (0 == m_iRefCnt)
	{
		Free();
		delete this;
		return 0;
	}

	return m_iRefCnt--;
}
