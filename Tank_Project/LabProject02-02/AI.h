#pragma once
#include "GameObject.h"
class CAI : public CExplosiveObject
{
public:
	CAI();
	~CAI();

public:
	XMFLOAT3					m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	XMFLOAT3					m_xmf3CameraOffset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	float						m_fFriction = 125.0f;

	float           			m_fPitch = 0.0f;
	float           			m_fYaw = 0.0f;
	float           			m_fRoll = 0.0f;

	float						m_HP = 100;

public:
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);

	void MoveForward(float distance);

	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);

	void Update(float fTimeElapsed = 0.016f);

	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
};

#define BULLETS					50

class CTankAI : public CAI
{
public:
	CTankAI();
	virtual ~CTankAI();

	CGameObject* m_pTurret = NULL;
	CGameObject* m_pGun = NULL;

	float						m_fBulletEffectiveRange = 150.0f;
	CBulletObject* m_ppBullets[BULLETS];

	void RotateTurret(float fAngle) { m_pTurret->Rotate(0.0f, fAngle, 0.0f); }
	void RotateGun(float fAngle) { m_pGun->Rotate(fAngle, 0.0f, 0.0f); }

	void FireBullet();

	void ChasePlayerMovement(CPlayer* player);

	virtual void Animate(float fElapsedTime, CPlayer* pPlayer);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
};

