#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Player.h"
#include "AI.h"

class CScene
{
public:
	CScene(CPlayer* pPlayer, int nAITanks, CAI** ppAITanks);
	virtual ~CScene();

private:

	int							m_nHealObject = 0;
	CHealObject					**m_ppHealObject = NULL;

	CFloorObject*				m_pFloorObject = NULL;

	int							m_nAITanks = 0;
	CAI							**m_ppAITanks = NULL;

	CPlayer*					m_pPlayer = NULL;

#ifdef _WITH_DRAW_AXIS
	CGameObject*				m_pWorldAxis = NULL;
#endif

public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	void CheckObjectByObjectCollisions();
	void CheckPlayerByHealObjectCollisions();
	void CheckObjectByBulletCollisions();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

