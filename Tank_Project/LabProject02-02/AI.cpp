#include "stdafx.h"
#include "AI.h"
#include "Player.h"

CAI::CAI()
{

}

CAI::~CAI()
{
}

void CAI::SetPosition(float x, float y, float z)
{
	m_xmf3Position = XMFLOAT3(x, y, z);

	CGameObject::SetPosition(x, y, z);
}

void CAI::MoveForward(float distance)
{
	CGameObject::MoveForward(distance);
}

void CAI::Rotate(float fPitch, float fYaw, float fRoll)
{
	if (fPitch != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
	}
	if (fYaw != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}
	if (fRoll != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	XMFLOAT3 crossProduct1 = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look);
	XMFLOAT3 crossProduct2 = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right);
	m_xmf3Right = Vector3::Normalize(crossProduct1);
	m_xmf3Up = Vector3::Normalize(crossProduct2);
}

void CAI::LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(m_xmf3Position, xmf3LookAt, xmf3Up);
	XMFLOAT3 m_xmRight = XMFLOAT3(xmf4x4View._11, xmf4x4View._21, xmf4x4View._31);
	XMFLOAT3 m_xmUp = XMFLOAT3(xmf4x4View._12, xmf4x4View._22, xmf4x4View._32);
	XMFLOAT3 m_xmLook = XMFLOAT3(xmf4x4View._13, xmf4x4View._23, xmf4x4View._33);
	m_xmf3Right = Vector3::Normalize(m_xmRight);
	m_xmf3Up = Vector3::Normalize(m_xmUp);
	m_xmf3Look = Vector3::Normalize(m_xmLook);
}

void CAI::Update(float fTimeElapsed)
{
	Move(m_xmf3Velocity, false);

	XMFLOAT3 scalarProduct = Vector3::ScalarProduct(m_xmf3Velocity, -1.0f);
	XMFLOAT3 xmf3Deceleration = Vector3::Normalize(scalarProduct);
	float fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = m_fFriction * fTimeElapsed;
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Deceleration, fDeceleration);
}

void CAI::Animate(float fElapsedTime)
{
	OnUpdateTransform();

	CExplosiveObject::Animate(fElapsedTime);
}

void CAI::OnUpdateTransform()
{
	m_xmf4x4Transform._11 = m_xmf3Right.x; m_xmf4x4Transform._12 = m_xmf3Right.y; m_xmf4x4Transform._13 = m_xmf3Right.z;
	m_xmf4x4Transform._21 = m_xmf3Up.x; m_xmf4x4Transform._22 = m_xmf3Up.y; m_xmf4x4Transform._23 = m_xmf3Up.z;
	m_xmf4x4Transform._31 = m_xmf3Look.x; m_xmf4x4Transform._32 = m_xmf3Look.y; m_xmf4x4Transform._33 = m_xmf3Look.z;
	m_xmf4x4Transform._41 = m_xmf3Position.x; m_xmf4x4Transform._42 = m_xmf3Position.y; m_xmf4x4Transform._43 = m_xmf3Position.z;
}

void CAI::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CExplosiveObject::Render(hDCFrameBuffer, pCamera);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CTankAI::CTankAI()
{
	CCubeMesh* pBodyMesh = new CCubeMesh(6.0f, 2.0f, 6.0f);
	SetMesh(pBodyMesh);
	SetColor(RGB(255, 0, 0));

	m_pTurret = new CGameObject();
	CCubeMesh* pTurretMesh = new CCubeMesh(4.0f, 1.5f, 4.0f);
	m_pTurret->SetMesh(pTurretMesh);
	m_pTurret->SetColor(RGB(255, 0, 0));
	m_pTurret->SetPosition(0.0f, 1.75f, 0.0f);
	SetChild(m_pTurret);

	m_pGun = new CGameObject();
	CCubeMesh* pGunMesh = new CCubeMesh(1.0f, 1.0f, 10.0f);
	m_pGun->SetMesh(pGunMesh);
	m_pGun->SetColor(RGB(255, 0, 0));
	m_pGun->SetPosition(0.0f, 1.25f, 4.0f);
	m_pTurret->SetChild(m_pGun);
	
	m_pHP = new CGameObject();
	m_fHPLength = 6.0f;
	m_fMAXHPLength = 6.0f;
	CCubeMesh* pHP = new CCubeMesh(m_fHPLength, 1.0f, 0.0f);
	m_pHP->SetMesh(pHP);
	m_pHP->SetColor(RGB(255, 0, 0));
	m_pHP->SetPosition(0.0f, 5.0f, 0.0f);
	SetChild(m_pHP);

	CCubeMesh* pBulletMesh = new CCubeMesh(1.0f, 1.0f, 4.0f);
	for (int i = 0; i < BULLETS; i++)
	{
		XMFLOAT3 rotationAxis = XMFLOAT3(0.0f, 0.0f, 1.0f);
		m_ppBullets[i] = new CBulletObject(m_fBulletEffectiveRange);
		m_ppBullets[i]->SetMesh(pBulletMesh);
		m_ppBullets[i]->SetRotationAxis(rotationAxis);
		m_ppBullets[i]->SetRotationSpeed(360.0f);
		m_ppBullets[i]->SetMovingSpeed(120.0f);
		m_ppBullets[i]->SetActive(false);
	}
}

CTankAI::~CTankAI()
{
	for (int i = 0; i < BULLETS; i++) if (m_ppBullets[i]) delete m_ppBullets[i];
}

void CTankAI::Animate(float fElapsedTime, CPlayer* pPlayer)
{
	CAI::Animate(fElapsedTime);

	ChasePlayerMovement(pPlayer);

	FireBulltetForSeconds(fElapsedTime);

	ComputeWorldTransform(NULL);

	for (int i = 0; i < BULLETS; i++)
	{
		if (m_ppBullets[i]->m_bActive)
		{
			m_ppBullets[i]->Animate(fElapsedTime);
			m_ppBullets[i]->ComputeWorldTransform(NULL);
		}
	}
}

void CTankAI::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	if (m_bActive == false) return;

	CAI::Render(hDCFrameBuffer, pCamera);

	for (int i = 0; i < BULLETS; i++)
	{
		if (m_ppBullets[i]->m_bActive)
		{
			m_ppBullets[i]->Render(hDCFrameBuffer, pCamera);
		}
	}
}

void CTankAI::FireBullet()
{
	CBulletObject* pBulletObject = NULL;
	for (int i = 0; i < BULLETS; i++)
	{
		if (!m_ppBullets[i]->m_bActive)
		{
			pBulletObject = m_ppBullets[i];
			break;
		}
	}

	if (pBulletObject)
	{
		XMFLOAT3 xmf3Position = m_pGun->GetPosition();
		XMFLOAT3 xmf3Direction = m_pGun->GetLook();
		XMFLOAT3 xmf3FirePosition = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Direction, 6.0f, false));

		pBulletObject->m_xmf4x4Transform = m_pGun->m_xmf4x4World;

		pBulletObject->SetFirePosition(xmf3FirePosition);
		pBulletObject->SetMovingDirection(xmf3Direction);
		pBulletObject->SetColor(RGB(255, 0, 0));
		pBulletObject->SetActive(true);
	}
}

void CTankAI::FireBulltetForSeconds(float fElapsedTime)
{
	m_fFireElapsedTimes += fElapsedTime;
	if (m_fFireElapsedTimes > m_fFireDuration)
	{
		FireBullet();
		m_fFireElapsedTimes = 0.0f;
	}
}

void CTankAI::ChasePlayerMovement(CPlayer* pPlayer)
{
	XMFLOAT3 xmf3Target = Vector3::Subtract(m_xmf3Position, pPlayer->m_xmf3Position);
	float fDistance = (m_xmf3Position.z >= pPlayer->m_xmf3Position.z) ? m_xmf3Position.z - pPlayer->m_xmf3Position.z : pPlayer->m_xmf3Position.z - m_xmf3Position.z;

	XMFLOAT3 xmf3LookAt = m_xmf3Look;
	XMFLOAT3 xmf3NormalizeTarget = Vector3::Normalize(xmf3Target);

	XMFLOAT3 xmf3CrossProduct = Vector3::CrossProduct(xmf3LookAt, pPlayer->m_xmf3Look, true);
	float fDotProduct = Vector3::DotProduct(xmf3LookAt, pPlayer->m_xmf3Look);

	if (fDistance > 10.0f)
	{
		if (fDotProduct != 0)
		{
			float fAngle = 0.5f;
			fAngle *= (xmf3CrossProduct.y > 0.0f) ? -1.0f : 1.0f;
			Rotate(m_fPitch, m_fYaw + fAngle, m_fRoll);
		}

		CGameObject::MoveForward(0.1f);
	}
	else
	{
		Rotate(m_fPitch, m_fYaw, m_fRoll);
		CGameObject::MoveForward(0.0f);
		fDistance = (m_xmf3Position.z >= pPlayer->m_xmf3Position.z) ? m_xmf3Position.z - pPlayer->m_xmf3Position.z : pPlayer->m_xmf3Position.z - m_xmf3Position.z;
	}

}

void CTankAI::ResetHP()
{
	m_fHP = 100.0f;
	m_fHPLength = m_fMAXHPLength;
	CCubeMesh* pHP = new CCubeMesh(m_fHPLength, 1.0f, 0.0f);
	m_pHP->SetMesh(pHP);
}

bool CTankAI::IncreaseHP(float fHeal)
{
	if (m_fHPLength + (m_fHPLength / fHeal) <= m_fMAXHPLength)
	{
		m_fHPLength += (m_fHPLength / fHeal);
		CCubeMesh* pHP = new CCubeMesh(m_fHPLength, 1.0f, 0.0f);
		m_pHP->SetMesh(pHP);
	}

	if (m_fHP + fHeal < 100.0f) { m_fHP += fHeal; return true; }
	else { m_fHP = 100.0f; return false; }
}

bool CTankAI::DecreaseHP(float fDamage)
{
	if (m_fHPLength - (m_fHPLength / fDamage) >= 0.0f)
	{
		m_fHPLength -= (m_fHPLength / fDamage);
		CCubeMesh* pHP = new CCubeMesh(m_fHPLength, 1.0f, 0.0f);
		m_pHP->SetMesh(pHP);
	}

	if (m_fHP - fDamage > 0.0f) { m_fHP -= fDamage; return true; }
	else { m_fHP = 0; return false; }
}
