#pragma once
#include "Object.h"
class AI : public CGameObject
{
protected:
	XMFLOAT3					m_xmf3Position;
	XMFLOAT3					m_xmf3Right;
	XMFLOAT3					m_xmf3Up;
	XMFLOAT3					m_xmf3Look;

	float           			m_fPitch;
	float           			m_fYaw;
	float           			m_fRoll;

	XMFLOAT3					m_xmf3Velocity;
	XMFLOAT3     				m_xmf3Gravity;
	float           			m_fMaxVelocityXZ;
	float           			m_fMaxVelocityY;
	float           			m_fFriction;

	LPVOID						m_pAIUpdatedContext;

	float						m_fHP = 100.0f;

public:
	AI();
	virtual ~AI();

	XMFLOAT3 GetPosition() { return(m_xmf3Position); }
	XMFLOAT3 GetLookVector() { return(m_xmf3Look); }
	XMFLOAT3 GetUpVector() { return(m_xmf3Up); }
	XMFLOAT3 GetRightVector() { return(m_xmf3Right); }

	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(const XMFLOAT3& xmf3Gravity) { m_xmf3Gravity = xmf3Gravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(const XMFLOAT3& xmf3Velocity) { m_xmf3Velocity = xmf3Velocity; }
	void SetPosition(const XMFLOAT3& xmf3Position)
	{
		CGameObject::SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
		Move(XMFLOAT3(xmf3Position.x - m_xmf3Position.x, xmf3Position.y - m_xmf3Position.y, xmf3Position.z - m_xmf3Position.z), false);
	}

	const XMFLOAT3& GetVelocity() const { return(m_xmf3Velocity); }
	float GetYaw() const { return(m_fYaw); }
	float GetPitch() const { return(m_fPitch); }
	float GetRoll() const { return(m_fRoll); }

	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	void Rotate(float x, float y, float z);

	void Update(float fTimeElapsed);
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);

	virtual void OnAIUpdateCallback(float fTimeElapsed) { }
	void SetAIUpdatedContext(LPVOID pContext) { m_pAIUpdatedContext = pContext; }

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);

public:

	void ResetHP();
	bool DecreaseHP(float fDamage);
};

#define BULLETS					50

class CTankAI : public AI
{
public:
	CTankAI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CTankAI();

	CGameObject* m_pBody = NULL;
	CGameObject* m_pWheel = NULL;
	CGameObject* m_pTurret = NULL;
	CGameObject* m_pGun = NULL;

public:
	float						m_fMovingDistance = 0.0f;
	float						m_fMovingSpeed = 0.5f;
	float						m_fMaxMovingDistance = 3.0f;

public:
	float						m_fBulletEffectiveRange = 150.0f;
	CBulletObject				*m_ppBullets[BULLETS];

	float						m_fFireDuration = 2.0f;
	float						m_fFireElapsedTimes = 0.0f;

public:
	void FireBullet();
	void FireBulltetForSeconds(float fElapsedTime);

private:
	virtual void OnInitialize();

public:
	virtual void Animate(CPlayer* pPlayer, float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);
	void ChasePleyerMovement(CPlayer* pPlayer, float fTimeElapsed);
	void SetMaxMovingDistance(float distance) { m_fMaxMovingDistance = distance; }

public:
	void RotateTurret(float fAngle) { m_pTurret->Rotate(0.0f, 0.0f, fAngle); }
	void RotateGun(float fAngle) { m_pGun->Rotate(fAngle, 0.0f, 0.0f); }
	void RotateBullet(float fAngle);

public:
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);

};




