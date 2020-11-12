#include "stdafx.h"
#include "..\Headers\DamageInfo.h"

USING(Client)

CDamageInfo::CDamageInfo(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CDamageInfo::CDamageInfo(const CDamageInfo & _rOther)
	: CComponent(_rOther)
{
}

const CDamageInfo::DAMAGE_DESC & CDamageInfo::Get_Desc() const
{
	return m_tDamageDesc;
}

_int CDamageInfo::Get_Att()
{
	_int iSimulateAtt = 0;
	_int iAttBound = m_tDamageDesc.iMaxAtt - m_tDamageDesc.iMinAtt;
	if (0 == iAttBound)	iSimulateAtt = m_tDamageDesc.iMaxAtt;
	else				iSimulateAtt = (_int)(rand() % iAttBound + m_tDamageDesc.iMinAtt);

	_int iSimulateCriticalChance = rand() % 100;
	if (iSimulateCriticalChance < m_tDamageDesc.iCriticalChance)
		iSimulateAtt *= m_tDamageDesc.iCriticalRate;

	return iSimulateAtt;
}

HRESULT CDamageInfo::Setup_Component_Prototype()
{
	return S_OK;
}

HRESULT CDamageInfo::Setup_Component(void * _pArg)
{
	if (_pArg)
		m_tDamageDesc = *(DAMAGE_DESC*)_pArg;

	return S_OK;
}

CDamageInfo * CDamageInfo::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CDamageInfo* pInstance = new CDamageInfo(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CStat", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CDamageInfo::Clone_Component(void * _pArg)
{
	CDamageInfo* pInstance = new CDamageInfo(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CStat", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamageInfo::Free(void)
{
	CComponent::Free();
}
