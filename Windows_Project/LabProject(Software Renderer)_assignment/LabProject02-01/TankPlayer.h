#pragma once
#include "Player.h"

#define BULLETS					50

class CTankPlayer : public CPlayer
{
public:
	CTankPlayer();
	virtual ~CTankPlayer();

	float						m_fBulletEffectiveRange = 150.0f;
	CBulletObject* m_ppBullets[BULLETS];

	void FireBullet(CGameObject* pLockedObject);

	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
};

