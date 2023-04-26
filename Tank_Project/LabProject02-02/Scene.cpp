#include "stdafx.h"
#include "Scene.h"
#include "AI.h"
#include "GraphicsPipeline.h"

CScene::CScene(CPlayer* pPlayer, int nAITanks, CAI** ppAITanks)
{
	m_pPlayer = pPlayer;
	m_nAITanks = nAITanks;
	m_ppAITanks = ppAITanks;
}

CScene::~CScene()
{
	for (int i = 0; i < m_nAITanks; i++) if (m_ppAITanks[i]) delete m_ppAITanks[i];
}

void CScene::BuildObjects()
{
	CExplosiveObject::PrepareExplosion();

	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 200.0f;
	CFloorMesh* pFloorMesh = new CFloorMesh(fHalfWidth * 2.0f, fHalfDepth * 2.0f, 30);

	m_pFloorObject = new CFloorObject();
	m_pFloorObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_pFloorObject->SetMesh(pFloorMesh);
	m_pFloorObject->SetColor(RGB(0, 0, 0));
	m_pFloorObject->m_pxmf4FloorPlane = XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

	CCubeMesh* pHealMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);

	m_nObjects = 1;
	m_ppObjects = new CGameObject * [m_nObjects];

	CExplosiveObject *pExplosiveObject = new CExplosiveObject();
	pExplosiveObject->SetMesh(pHealMesh);
	pExplosiveObject->SetColor(RGB(0, 255, 0));
	pExplosiveObject->SetPosition(-43.5f, 0.0f, -44.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(90.0f);
	m_ppObjects[0] = pExplosiveObject;

	m_nHealObject = 1;
	m_ppHealObject = new CHealObject * [m_nHealObject];

	CHealObject* pHealObject = new CHealObject();
	pHealObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pHealObject->SetRotationSpeed(90.0f);
	m_ppHealObject[0] = pHealObject;


#ifdef _WITH_DRAW_AXIS
	m_pWorldAxis = new CGameObject();
	CAxisMesh* pAxisMesh = new CAxisMesh(0.5f, 0.5f, 0.5f);
	m_pWorldAxis->SetMesh(pAxisMesh);
#endif
}

void CScene::ReleaseObjects()
{
	if (CExplosiveObject::m_pExplosionMesh) CExplosiveObject::m_pExplosionMesh->Release();

	for (int i = 0; i < m_nObjects; i++) if (m_ppObjects[i]) delete m_ppObjects[i];
	if (m_ppObjects) delete[] m_ppObjects;

	for (int i = 0; i < m_nHealObject; i++) if (m_ppHealObject[i]) delete m_ppHealObject[i];
	if (m_ppHealObject) delete[] m_ppHealObject;

	if (m_pFloorObject) delete m_pFloorObject;

#ifdef _WITH_DRAW_AXIS
	if (m_pWorldAxis) delete m_pWorldAxis;
#endif
}

void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		default:
			break;
		}
		break;
	default:
		break;
	}
}

CGameObject* CScene::PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera)
{
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / (float)pCamera->m_Viewport.m_nWidth) - 1) / pCamera->m_xmf4x4PerspectiveProject._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / (float)pCamera->m_Viewport.m_nHeight) - 1) / pCamera->m_xmf4x4PerspectiveProject._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmmtxView = XMLoadFloat4x4(&pCamera->m_xmf4x4View);

	int nIntersected = 0;
	float fNearestHitDistance = FLT_MAX;
	CGameObject* pNearestObject = NULL;
	for (int i = 0; i < m_nObjects; i++)
	{
		float fHitDistance = FLT_MAX;
		nIntersected = m_ppObjects[i]->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);
		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
		{
			fNearestHitDistance = fHitDistance;
			pNearestObject = m_ppObjects[i];
		}
	}
	return(pNearestObject);
}

void CScene::CheckObjectByObjectCollisions()
{
	//m_pPlayer->m_pObjectCollided = NULL;
	//m_pPlayer->

	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->m_pObjectCollided = NULL;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = (i + 1); j < m_nObjects; j++)
		{
			if (m_ppObjects[i]->m_xmOOBB.Intersects(m_ppObjects[j]->m_xmOOBB))
			{
				m_ppObjects[i]->m_pObjectCollided = m_ppObjects[j];
				m_ppObjects[j]->m_pObjectCollided = m_ppObjects[i];
			}
		}
	}


	for (int i = 0; i < m_nAITanks; i++) m_ppAITanks[i]->m_pObjectCollided = NULL;
	for (int i = 0; i < m_nAITanks; i++)
	{
		for (int j = (i + 1); j < m_nAITanks; j++)
		{
			if (m_ppAITanks[i]->m_xmOOBB.Intersects(m_ppAITanks[j]->m_xmOOBB))
			{
				m_ppAITanks[i]->m_pObjectCollided = m_ppAITanks[j];
				m_ppAITanks[j]->m_pObjectCollided = m_ppAITanks[i];
			}
		}
	}

	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i]->m_pObjectCollided)
		{
			XMFLOAT3 xmf3MovingDirection = m_ppObjects[i]->m_xmf3MovingDirection;
			float fMovingSpeed = m_ppObjects[i]->m_fMovingSpeed;
			m_ppObjects[i]->m_xmf3MovingDirection = m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection;
			m_ppObjects[i]->m_fMovingSpeed = m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
			m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed = fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_pObjectCollided = NULL;
			m_ppObjects[i]->m_pObjectCollided = NULL;
		}
	}

	for (int i = 0; i < m_nAITanks; i++)
	{
		if (m_ppAITanks[i]->m_pObjectCollided)
		{
			XMFLOAT3 xmf3MovingDirection = m_ppAITanks[i]->m_xmf3MovingDirection;
			float fMovingSpeed = m_ppAITanks[i]->m_fMovingSpeed;
			m_ppAITanks[i]->m_xmf3MovingDirection = m_ppAITanks[i]->m_pObjectCollided->m_xmf3MovingDirection;
			m_ppAITanks[i]->m_fMovingSpeed = m_ppAITanks[i]->m_pObjectCollided->m_fMovingSpeed;
			m_ppAITanks[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
			m_ppAITanks[i]->m_pObjectCollided->m_fMovingSpeed = fMovingSpeed;
			m_ppAITanks[i]->m_pObjectCollided->m_pObjectCollided = NULL;
			m_ppAITanks[i]->m_pObjectCollided = NULL;
		}
	}

}

void CScene::CheckObjectByBulletCollisions()
{
	CBulletObject** ppBullets = ((CTankPlayer*)m_pPlayer)->m_ppBullets;
	CBulletObject** ppBulletsEnemy = NULL;


	/*for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBullets[j]->m_bActive && m_ppObjects[i]->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[i];
				pExplosiveObject->m_bBlowingUp = true;
				ppBullets[j]->Reset();
			}
		}
	}*/

	for (int i = 0; i < m_nAITanks; i++)
	{
		ppBulletsEnemy = ((CTankAI*)m_ppAITanks[i])->m_ppBullets;

		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBullets[j]->m_bActive && 
				(m_ppAITanks[i]->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB) || ((CTankAI*)m_ppAITanks[i])->m_pTurret->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
				|| ((CTankAI*)m_ppAITanks[i])->m_pGun->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
			{
				if (((CTankAI*)m_ppAITanks[i])->DecreaseHP(ppBullets[j]->m_fBulletDamage) == false)
				{
					((CTankAI*)m_ppAITanks[i])->ResetHP();
					CExplosiveObject* pExplosiveObject = m_ppAITanks[i];
					pExplosiveObject->m_bBlowingUp = true;
					ppBullets[j]->Reset();
				}
			}
		}

		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBulletsEnemy[j]->m_bActive &&
				(m_pPlayer->m_xmOOBB.Intersects(ppBulletsEnemy[j]->m_xmOOBB) || ((CTankPlayer*)m_pPlayer)->m_pTurret->m_xmOOBB.Intersects(ppBulletsEnemy[j]->m_xmOOBB))
				|| ((CTankPlayer*)m_pPlayer)->m_pGun->m_xmOOBB.Intersects(ppBulletsEnemy[j]->m_xmOOBB))
			{
				if (((CTankPlayer*)m_pPlayer)->DecreaseHP(ppBulletsEnemy[j]->m_fBulletDamage) == false)
				{
					//((CTankPlayer*)m_pPlayer)->ResetHP();
					//CExplosiveObject* pExplosiveObject = m_ppAITanks[i];
					//pExplosiveObject->m_bBlowingUp = true;
					ppBulletsEnemy[j]->Reset();
				}
			}
		}
	}
}

void CScene::Animate(float fElapsedTime)
{
	m_pFloorObject->Animate(fElapsedTime);
	m_pFloorObject->ComputeWorldTransform(NULL);

	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Animate(fElapsedTime);
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->ComputeWorldTransform(NULL);

	for (int i = 0; i < m_nHealObject; i++) ((CRotatingObject*)m_ppHealObject[i])->Animate(fElapsedTime);
	for (int i = 0; i < m_nHealObject; i++) m_ppHealObject[i]->ComputeWorldTransform(NULL);

	for (int i = 0; i < m_nAITanks; i++) ((CTankAI*)m_ppAITanks[i])->Animate(fElapsedTime, m_pPlayer);
	for (int i = 0; i < m_nAITanks; i++) m_ppAITanks[i]->ComputeWorldTransform(NULL);

	CheckObjectByObjectCollisions();

	CheckObjectByBulletCollisions();
}

void CScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);
	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);

	m_pFloorObject->Render(hDCFrameBuffer, pCamera);
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);

	for (int i = 0; i < m_nHealObject; i++) m_ppHealObject[i]->Render(hDCFrameBuffer, pCamera);

	if (m_pPlayer) m_pPlayer->Render(hDCFrameBuffer, pCamera);

	for (int i = 0; i < m_nAITanks; i++) m_ppAITanks[i]->Render(hDCFrameBuffer, pCamera);

//UI
#ifdef _WITH_DRAW_AXIS
	CGraphicsPipeline::SetViewOrthographicProjectTransform(&pCamera->m_xmf4x4ViewOrthographicProject);
	m_pWorldAxis->SetRotationTransform(&m_pPlayer->m_xmf4x4World);
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif
}
