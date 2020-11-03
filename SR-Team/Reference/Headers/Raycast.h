#pragma once
#ifndef __RAYCAST_H__
#define __RAYCAST_H__

#include "Component.h"
#include "VIBuffer.h"

BEGIN(Engine)

class CCamera;
class ENGINE_DLL CRaycast final : public CComponent
{
private:
	explicit CRaycast(LPDIRECT3DDEVICE9 _pDevice);
	explicit CRaycast(const CRaycast& _rOther);
	virtual ~CRaycast() = default;

public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void* _pArg) override;

public:
	template <typename VTX_TYPE, typename IDX_TYPE>
	bool IsSimulate(HWND _hWnd, _uint _iWinCX, _uint _iWinCY,
		CVIBuffer* _pVIBuffer, _matrix* _pMatWorld, CCamera* _pCamera, _vec3* _pOutPos = nullptr);

private:
	HRESULT Convert_ViewportToLocal(HWND _hWnd, _uint _iWinCX, _uint _iWinCY, _matrix* _pMatWorld, CCamera* _pCamera);
	
public:
	static CRaycast* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent* Clone_Component(void* _pArg) override;
	virtual void Free() override;



private:
	_vec3 m_vRayPos = {};
	_vec3 m_vRayDir = {};
};

template<typename VTX_TYPE, typename IDX_TYPE>
bool CRaycast::IsSimulate(HWND _hWnd, _uint _iWinCX, _uint _iWinCY, CVIBuffer * _pVIBuffer, _matrix * _pMatWorld, CCamera * _pCamera, _vec3 * _pOutPos)
{
	bool bRet = false;

	if (nullptr == _pVIBuffer || nullptr == _pMatWorld || nullptr == _pCamera)
		return bRet;

	if (FAILED(Convert_ViewportToLocal(_hWnd, _iWinCX, _iWinCY, _pMatWorld, _pCamera)))
		return bRet;

	_float fU = 0.f, fV = 0.f, fDist = FLT_MAX;
	VTX_TYPE* pVTX = (VTX_TYPE*)_pVIBuffer->Get_Vertices();
	if (!pVTX) return bRet;

	IDX_TYPE* pIDX = (IDX_TYPE*)_pVIBuffer->Get_Indices();
	if (!pIDX) return bRet;

	_vec3 V1, V2, V3;
	_uint iTriCount = _pVIBuffer->Get_TriCount();
	for (_uint iCnt = 0; iCnt < iTriCount; ++iCnt)
	{
		V1 = pVTX[pIDX[iCnt]._1].vPosition;
		V2 = pVTX[pIDX[iCnt]._2].vPosition;
		V3 = pVTX[pIDX[iCnt]._3].vPosition;

		_float fTemp = 0.f;
		if (D3DXIntersectTri(&V1, &V2, &V3, &m_vRayPos, &m_vRayDir, &fU, &fV, &fTemp))
		{
			if (_pOutPos && fDist > fTemp)
			{
				bRet = true;
				fDist = fTemp;
				*_pOutPos = V1 + fU * (V2 - V1) + fV * (V3 - V1);
			}
		}
	}

	if (bRet)
		D3DXVec3TransformCoord(_pOutPos, _pOutPos, _pMatWorld);

	return bRet;
}

END

#endif