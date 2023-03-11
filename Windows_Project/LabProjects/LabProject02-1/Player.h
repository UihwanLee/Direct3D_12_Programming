#pragma once

#include "GameObject.h"
#include "Camera.h"

// �÷��̾� ��ü�� ���� ��ü�̹Ƿ� CGameObject Ŭ�������� ��ӹ޾� Ŭ������ �����Ѵ�.
class CPlayer : public CGameObject
{
public:
	CPlayer() { }
	virtual ~CPlayer() { if (m_pCamera) delete m_pCamera; }

private:
	// �÷��̾� ��ü�� ���Ե� ī�޶��̴�.
	CCamera*		m_pCamera = NULL;

public:
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void Move(float x, float y, float z);
	void Rotate(float fPitch, float fYaw, float fRoll);

	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	CCamera* GetCamera() { return(m_pCamera); }
};
