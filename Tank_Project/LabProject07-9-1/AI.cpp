#include "stdafx.h"
#include "AI.h"
#include "Player.h"

AI::AI()
{
	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_pAIUpdatedContext = NULL;
}

AI::~AI()
{
	ReleaseShaderVariables();
}

void AI::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);

		Move(xmf3Shift, bUpdateVelocity);
	}
}

void AI::Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else
	{
		m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);
	}
}

void AI::Rotate(float x, float y, float z)
{
	if (x != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
	}
	if (y != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
	}
	if (z != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(z));
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
	}

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);
}

void AI::Update(float fTimeElapsed)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(m_xmf3Velocity, fTimeElapsed, false);
	Move(xmf3Velocity, false);

	if (m_pAIUpdatedContext) OnAIUpdateCallback(fTimeElapsed);

	fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));
}

void AI::Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
	CGameObject::Animate(fTimeElapsed, pxmf4x4Parent);
}

void AI::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void AI::ReleaseShaderVariables()
{
}

void AI::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void AI::OnPrepareRender()
{
	m_xmf4x4Transform._11 = m_xmf3Right.x; m_xmf4x4Transform._12 = m_xmf3Right.y; m_xmf4x4Transform._13 = m_xmf3Right.z;
	m_xmf4x4Transform._21 = m_xmf3Up.x; m_xmf4x4Transform._22 = m_xmf3Up.y; m_xmf4x4Transform._23 = m_xmf3Up.z;
	m_xmf4x4Transform._31 = m_xmf3Look.x; m_xmf4x4Transform._32 = m_xmf3Look.y; m_xmf4x4Transform._33 = m_xmf3Look.z;
	m_xmf4x4Transform._41 = m_xmf3Position.x; m_xmf4x4Transform._42 = m_xmf3Position.y; m_xmf4x4Transform._43 = m_xmf3Position.z;

	UpdateTransform(NULL);
}

void AI::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	CGameObject::Render(pd3dCommandList, pCamera);
}

void AI::ResetHP()
{
	m_fHP = 100.0f;
}

bool AI::DecreaseHP(float fDamage)
{
	if (m_fHP - fDamage > 0.0f)
	{
		m_fHP -= fDamage;
		return true;
	}
	else
	{
		m_fHP = 0;
		return false;
	}
}

CTankAI::CTankAI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CGameObject* pGameObject = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Tank.bin");

	SetChild(pGameObject, true);

	OnInitialize();

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	for (int i = 0; i < BULLETS; i++)
	{
		m_ppBullets[i] = new CBulletObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
		m_ppBullets[i]->SetBoundingBox(m_ppBullets[i]->GetPosition(), XMFLOAT3(0.5f, 0.5f, 0.5f));
		m_ppBullets[i]->SetActive(false);
	}
}

CTankAI::~CTankAI()
{
}

void CTankAI::FireBullet()
{
	CBulletObject* pBulletObject = NULL;
	for (int i = 0; i < BULLETS; i++)
	{
		if (!m_ppBullets[i]->GetActive())
		{
			pBulletObject = m_ppBullets[i];
			break;
		}
	}

	if (pBulletObject)
	{
		XMFLOAT3 xmf3Position = m_pTurret->GetPosition();
		XMFLOAT3 xmf3Direction = m_pTurret->GetRight();
		XMFLOAT3 xmf3FirePosition = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Direction, 1.0f, false));

		pBulletObject->SetFirePosition(xmf3FirePosition, m_pTurret->GetRight());
		pBulletObject->SetActive(true);
		pBulletObject->SetFire(true);
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

void CTankAI::OnInitialize()
{
	SetBoundingBox(GetPosition(), XMFLOAT3(1.0f, 1.0f, 2.0f));

	m_pWheel = FindFrame("Cylinder85");
	m_pTurret = FindFrame("tankk");
	m_pGun = FindFrame("Box03.001");
}

void CTankAI::Animate(CPlayer* pPlayer, float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
	if (GetActive())
	{
		FireBulltetForSeconds(fTimeElapsed);

		ChasePleyerMovement(pPlayer, fTimeElapsed);

		for (int i = 0; i < BULLETS; i++)
		{
			if (m_ppBullets[i])
			{
				if (m_ppBullets[i]->GetActive())
				{
					m_ppBullets[i]->Animate(fTimeElapsed);
					m_ppBullets[i]->UpdateBoundingBox();
				}
			}
		}
	}
}

void CTankAI::ChasePleyerMovement(CPlayer* pPlayer, float fTimeElapsed)
{
	// 반복 이동
	MoveForward(0.1f);

	float fDistance = m_fMovingSpeed * fTimeElapsed;

	m_fMovingDistance += fDistance;

	if (m_fMovingDistance > m_fMaxMovingDistance)
	{
		CGameObject::Rotate(0.0f, 180.0f, 0.0f);
		m_fMovingDistance = 0.0f;
	}
}

void CTankAI::RotateBullet(float fAngle)
{
	for (int i = 0; i < BULLETS; i++)
	{
		if (m_ppBullets[i])
		{
			if (!m_ppBullets[i]->m_bFire) m_ppBullets[i]->Rotate(0.0f, fAngle, 0.0f);
		}
	}
}

void CTankAI::OnPrepareRender()
{
	//AI::OnPrepareRender();
}

void CTankAI::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	AI::Render(pd3dCommandList, pCamera);

	for (int i = 0; i < BULLETS; i++) if (m_ppBullets[i]->GetActive()) m_ppBullets[i]->Render(pd3dCommandList, pCamera);
}
