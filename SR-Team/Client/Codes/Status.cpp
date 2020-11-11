#include "stdafx.h"
#include "..\Headers\Status.h"

USING(Client)

CStatus::CStatus(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CStatus::CStatus(const CStatus & _rOther)
	: CComponent(_rOther)
{
}

const CStatus::STAT& CStatus::Get_Status() const
{
	return m_tStat;
}

void CStatus::Set_Status(const STAT & _rStatus)
{
	m_tStat = _rStatus;
}

void CStatus::Set_Att(_int iAtt)
{
	m_tStat.iMinAtt += iAtt;
	m_tStat.iMaxAtt += iAtt;
}

void CStatus::Set_HP(_int _iAtt)
{
	m_tStat.iHp -= _iAtt;
}

void CStatus::Set_MP(_int _iMP)
{
	m_tStat.iMp -= _iMP;
}

_int CStatus::Get_Att()
{
	_int iSimulateAtt = 0;
	_int iAttBound = m_tStat.iMaxAtt - m_tStat.iMinAtt;
	iSimulateAtt = (_int)((rand() % iAttBound + m_tStat.iMinAtt) * (m_tStat.fAttRate + 1);

	_int iSimulateCriticalChance = rand() % 100;
	if (iSimulateCriticalChance < m_tStat.iCriticalChance)
		iSimulateAtt *= m_tStat.iCriticalRate;

	return iSimulateAtt;
}

_int CStatus::Get_Def()
{
	return (_int)(m_tStat.iDef * m_tStat.fDefRate);
}

void CStatus::Set_AttRate(_float _fAttRate)
{
	m_tStat.fAttRate = _fAttRate;
}

void CStatus::Set_DefRate(_float _fDefRate)
{
	m_tStat.fDefRate = _fDefRate;
}

_int CStatus::Increase_FireStack()
{
	m_tStat.iCurFireStack = min(m_tStat.iMaxFireStack, m_tStat.iCurFireStack + 1);

	return m_tStat.iCurFireStack;
}

_int CStatus::Increase_IceStack()
{
	m_tStat.iCurIceStack = min(m_tStat.iMaxIceStack, m_tStat.iCurIceStack + 1);

	return m_tStat.iCurIceStack;
}

void CStatus::Clear_FireStack()
{
	m_tStat.iCurFireStack = 0;
}

void CStatus::Clear_IceStack()
{
	m_tStat.iCurIceStack = 0;
}

void CStatus::Set_EquipStat(const CStatus::STAT & tStat)
{
	m_tStat.iMinAtt = tStat.iMinAtt + 10;
	m_tStat.iMaxAtt = tStat.iMaxAtt + 20;
	m_tStat.iDef = tStat.iDef + 20;
	m_tStat.iCriticalRate = tStat.iCriticalRate + 1;
	m_tStat.iCriticalChance = tStat.iCriticalChance + 5;
}

HRESULT CStatus::Setup_Component_Prototype()
{
	return S_OK;
}

HRESULT CStatus::Setup_Component(void * _pArg)
{
	if (_pArg != nullptr)
		memcpy_s(&m_tStat, sizeof(STAT), _pArg, sizeof(STAT));

	return S_OK;
}

CStatus * CStatus::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CStatus* pInstance = new CStatus(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CStat", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CStatus::Clone_Component(void * pArg)
{
	CStatus* pInstance = new CStatus(*this);
	if (FAILED(pInstance->Setup_Component(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CStat", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatus::Free()
{
	CComponent::Free();
}
