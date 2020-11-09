#include "stdafx.h"
#include "..\Headers\RandomBox.h"

USING(Client)

CRandomBox::CRandomBox(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CRandomBox::CRandomBox(const CRandomBox & other)
	:CGameObject(other)
{
}

HRESULT CRandomBox::Setup_GameObject_Prototype()
{
	m_pRandomBox = new BOXLIST;

	return S_OK;
}

HRESULT CRandomBox::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CRandomBox::Update_GameObject(float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

int CRandomBox::LateUpdate_GameObject(float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

HRESULT CRandomBox::Render_BlendAlpha()
{
	// ����
	return S_OK;
}

CRandomBox * CRandomBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CRandomBox* pInstance = new CRandomBox(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Crate CRandomBox", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRandomBox::Clone_GameObject(void * pArg)
{
	CRandomBox* pInstance = new CRandomBox(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To clone CRandomBox", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRandomBox::Free()
{
	for (auto& pInfo : *m_pRandomBox)
		Safe_Delete(pInfo);

	m_pRandomBox->clear();
	m_pRandomBox->shrink_to_fit();

	Safe_Delete(m_pRandomBox);

	CGameObject::Free();
}

HRESULT CRandomBox::Add_Component()
{
	// ����
	return S_OK;
}

_bool CRandomBox::Add_Item(const wstring & _strItemTag, int _iRate, int _iOrder)
{
	auto iter_find = m_pRandomBox->begin();
	for (int iCnt = 0; iCnt < (int)m_pRandomBox->size(); ++iCnt, ++iter_find)
	{
		if (!_tcscmp(_strItemTag.c_str(), (*iter_find)->szItemTag))
			return false;
	}

	RandomBoxInfo* pItem = new RandomBoxInfo;
	pItem->bRemain = true;
	StringCchCopy(pItem->szItemTag, _countof(pItem->szItemTag), _strItemTag.c_str());
	pItem->iOriginRate = pItem->iCurrentRate = _iRate;
	pItem->iOrder = _iOrder;

	m_pRandomBox->emplace_back(pItem);
	return true;
}

_bool CRandomBox::Remove_Item(const wstring & _strItemTag)
{
	auto iter_find = m_pRandomBox->begin();
	for (int iCnt = 0; iCnt < (int)m_pRandomBox->size(); ++iCnt, ++iter_find)
	{
		if (!_tcscmp(_strItemTag.c_str(), (*iter_find)->szItemTag))
			return false;
	}

	if (m_pRandomBox->end() != iter_find)
		return false;

	RandomBoxInfo* pItem = *iter_find;
	m_pRandomBox->erase(iter_find);

	delete pItem;
	return true;
}

_bool CRandomBox::Gatcha(wstring & _strItemTag)
{
	int iTotalRate = 0;
	int iRand = 0;

	//--------------------------------------------------
	// ����Ŭ�� 1ȸ ���Ҵٸ� �������ش�.
	//--------------------------------------------------
	if (m_iLastOrder <= m_iCurrOrder)
		Reset();


	//--------------------------------------------------
	// ��� ������ �� Ÿ�̹��� üũ
	//--------------------------------------------------
	++m_iCurrOrder;
	for (auto& pItem : *m_pRandomBox)
	{
		// �̹� ���� �������� �ٽ� ���� �� �����Ƿ� �ǳ� �ڴ�.
		if (!pItem->bRemain)
			continue;

		// ������ ���� �����۵� �߿��� ���� ������ �������� �ִ��� ã�ƺ���.
		if (m_iCurrOrder == pItem->iOrder)
		{
			pItem->bRemain = false;
			_strItemTag = pItem->szItemTag;
			return true;
		}
	}


	//--------------------------------------------------
	// �����ִ� �� ��� -> �̸� ����س��°ɷ� �ٲٴ���..
	//--------------------------------------------------
	for (auto& pItem : *m_pRandomBox)
	{
		// �������� �ʴٸ� �ǳ� �ڴ�.
		if (!pItem->bRemain)
			continue;

		// ���� Ÿ�̹��� ���������� ���� �����۸� ����Ѵ�.
		if (0 == pItem->iOrder)
			iTotalRate += pItem->iCurrentRate;
	}


	//--------------------------------------------------
	// ���� �ִ� �͵� �߿� ���� �̱�
	//--------------------------------------------------
	iRand = rand() % iTotalRate;
	iTotalRate = 0;

	for (auto& pItem : *m_pRandomBox)
	{
		if (!pItem->bRemain)
			continue;

		iTotalRate += pItem->iCurrentRate;
		if (iRand < iTotalRate)
		{
			--pItem->iCurrentRate;
			if (!pItem->iCurrentRate)
				pItem->bRemain = false;

			_strItemTag = pItem->szItemTag;
			return true;
		}
	}

	return false;
}

void CRandomBox::Reset(int _iStartOrder)
{
	m_iCurrOrder = _iStartOrder;
	for (auto& pItem : *m_pRandomBox)
	{
		pItem->bRemain = true;
		pItem->iCurrentRate = pItem->iOriginRate;

		// ������ �̱� Ƚ��, ����� ������ ���� �������� ������ ���� ���µȴ�.
		// ��� ������ ���� ������ ���� �Ǵ� Ÿ�̹��� �ٸ��� ���� �����ϰ�..
		if (m_iLastOrder < pItem->iOrder)
			m_iLastOrder = pItem->iOrder;
	}
}
