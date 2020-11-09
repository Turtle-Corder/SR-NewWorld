#pragma once
#ifndef __RANDOMBOX_H__
#define __RANDOMBOX_H__

USING(Engine)
BEGIN(Client)

class CRandomBox final : public CGameObject
{
public:
	typedef struct tagRandomBoxInfo
	{
		_bool	bRemain;
		WCHAR	szItemTag[MIN_STR] = L"";
		_int	iOriginRate;
		_int	iCurrentRate;
		_int	iOrder;
	} RandomBoxInfo;

private:
	explicit CRandomBox(LPDIRECT3DDEVICE9 pDevice);
	explicit CRandomBox(const CRandomBox& other);
	virtual ~CRandomBox() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual int Update_GameObject(float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(float _fDeltaTime) override;
	virtual HRESULT Render_BlendAlpha() override;

	static CRandomBox* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	HRESULT Add_Component();

public:
	_bool Add_Item(const wstring& _strItemTag, int _iRate, int _iOrder = 0);
	_bool Remove_Item(const wstring& _strItemTag);

	_bool Gatcha(wstring& _strItemTag);
	void Reset(int _iStartOrder = 0);


protected:
	typedef vector<RandomBoxInfo*> BOXLIST;

	BOXLIST*	m_pRandomBox = nullptr;

	_int		m_iCurrOrder = 0;
	_int		m_iLastOrder = 0;

};

END

#endif // !__RANDOMBOX_H__
