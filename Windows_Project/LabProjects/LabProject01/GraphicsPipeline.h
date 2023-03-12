#pragma once

#include "GameObject.h"
#include "Camera.h"

class CGraphicsPipeline
{
	//SINGLETON(CGraphicsPipeline);

private:
	static CGraphicsPipeline* thisP; 
	CGraphicsPipeline(); 
	~CGraphicsPipeline(); 
public:
	static CGraphicsPipeline* GetInst()
	{
		if (thisP == nullptr) { thisP = new CGraphicsPipeline(); }
		return thisP; 
	}

public:
	CGameObject* m_pGameObject;
	CCamera* m_pCamera;

public:
	void SetGameObject(CGameObject* pGameObject) { m_pGameObject = pGameObject; }
	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }

	CPoint3D ScreenTransform(CPoint3D& f3Projection);
	CPoint3D Project(CPoint3D& f3Model);
	CPoint3D Transform(CPoint3D& f3Model);
};


