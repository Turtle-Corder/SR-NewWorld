#include "..\Headers\BoxCollider.h"


USING(Engine)

CBoxCollider::CBoxCollider(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CBoxCollider::CBoxCollider(const CBoxCollider & _rOther)
	: CComponent(_rOther)
{
}

const CBoxCollider::BOXCOLLIDER_DESC & CBoxCollider::Get_Desc() const
{
	return m_tColliderDesc;
}

HRESULT CBoxCollider::Setup_Component_Prototype()
{
	return S_OK;
}

HRESULT CBoxCollider::Setup_Component(void * _pArg)
{
	if (_pArg)
		memcpy_s(&m_tColliderDesc, sizeof(BOXCOLLIDER_DESC), _pArg, sizeof(BOXCOLLIDER_DESC));

	return S_OK;
}

HRESULT CBoxCollider::Update_Collider(const _vec3 & vPosition)
{
	m_tColliderDesc.vPosition = vPosition;

	return S_OK;
}

void CBoxCollider::Set_Size(_vec3 _vSize)
{
	m_tColliderDesc.vObjectSize = _vSize;
}

CBoxCollider * CBoxCollider::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CBoxCollider* pInstance = new CBoxCollider(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CCollider", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CBoxCollider::Clone_Component(void * _pArg)
{
	CBoxCollider* pInstance = new CBoxCollider(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CCollider", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoxCollider::Free()
{
	CComponent::Free();
}
