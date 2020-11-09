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
	// 없음
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
	// 없음
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
	// 사이클을 1회 돌았다면 리셋해준다.
	//--------------------------------------------------
	if (m_iLastOrder <= m_iCurrOrder)
		Reset();


	//--------------------------------------------------
	// 레어가 뽑혀야 할 타이밍을 체크
	//--------------------------------------------------
	++m_iCurrOrder;
	for (auto& pItem : *m_pRandomBox)
	{
		// 이미 뽑힌 아이템은 다시 뽑힐 수 없으므로 건너 뛴다.
		if (!pItem->bRemain)
			continue;

		// 뽑히지 않은 아이템들 중에서 뽑힐 차례인 아이템이 있는지 찾아본다.
		if (m_iCurrOrder == pItem->iOrder)
		{
			pItem->bRemain = false;
			_strItemTag = pItem->szItemTag;
			return true;
		}
	}


	//--------------------------------------------------
	// 남아있는 걸 계산 -> 미리 계산해놓는걸로 바꾸던지..
	//--------------------------------------------------
	for (auto& pItem : *m_pRandomBox)
	{
		// 남아있지 않다면 건너 뛴다.
		if (!pItem->bRemain)
			continue;

		// 뽑힐 타이밍이 정해져있지 않은 아이템만 계산한다.
		if (0 == pItem->iOrder)
			iTotalRate += pItem->iCurrentRate;
	}


	//--------------------------------------------------
	// 남아 있는 것들 중에 랜덤 뽑기
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

		// 마지막 뽑기 횟수, 현재는 마지막 레어 아이템이 뽑히는 순간 리셋된다.
		// 멤버 변수로 만든 이유는 리셋 되는 타이밍을 다르게 조작 가능하게..
		if (m_iLastOrder < pItem->iOrder)
			m_iLastOrder = pItem->iOrder;
	}
}
