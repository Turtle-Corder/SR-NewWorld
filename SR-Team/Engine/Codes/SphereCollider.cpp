#include "..\Headers\SphereCollider.h"

USING(Engine)

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CSphereCollider::CSphereCollider(const CSphereCollider & _rOther)
	: CComponent(_rOther)
{
}

const CSphereCollider::COLLIDER_DESC & CSphereCollider::Get_Desc() const
{
	return m_tColliderDesc;
}

HRESULT CSphereCollider::Setup_Component_Prototype()
{
	return S_OK;
}

HRESULT CSphereCollider::Setup_Component(void * _pArg)
{
	if (_pArg)
		memcpy_s(&m_tColliderDesc, sizeof(COLLIDER_DESC), _pArg, sizeof(COLLIDER_DESC));

	return S_OK;
}

HRESULT CSphereCollider::Update_Collider(const _vec3 & vPosition)
{
	m_tColliderDesc.vPosition = vPosition;

	return S_OK;
}

void CSphereCollider::Set_Radius(_float _fRadius)
{
	m_tColliderDesc.fRadius = _fRadius;
}

CSphereCollider * CSphereCollider::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSphereCollider* pInstance = new CSphereCollider(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CCollider", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CSphereCollider::Clone_Component(void * _pArg)
{
	CSphereCollider* pInstance = new CSphereCollider(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CCollider", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSphereCollider::Free()
{
	CComponent::Free();
}
