//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::BuildDefaultLightsAndMaterials()
{
	m_nLights = 4;
	m_pLights = new LIGHT[m_nLights];
	::ZeroMemory(m_pLights, sizeof(LIGHT) * m_nLights);

	m_xmf4GlobalAmbient = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);

	m_pLights[0].m_bEnable = true;
	m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights[0].m_fRange = 1000.0f;
	m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	m_pLights[0].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[0].m_xmf3Position = XMFLOAT3(30.0f, 30.0f, 30.0f);
	m_pLights[0].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pLights[0].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);
	m_pLights[1].m_bEnable = true;
	m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights[1].m_fRange = 500.0f;
	m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[1].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[1].m_fFalloff = 8.0f;
	m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
	m_pLights[2].m_bEnable = true;
	m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights[2].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[2].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights[2].m_xmf4Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.0f);
	m_pLights[2].m_xmf3Direction = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_pLights[3].m_bEnable = true;
	m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights[3].m_fRange = 600.0f;
	m_pLights[3].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[3].m_xmf4Diffuse = XMFLOAT4(0.3f, 0.7f, 0.0f, 1.0f);
	m_pLights[3].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[3].m_xmf3Position = XMFLOAT3(50.0f, 30.0f, 30.0f);
	m_pLights[3].m_xmf3Direction = XMFLOAT3(0.0f, 1.0f, 1.0f);
	m_pLights[3].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[3].m_fFalloff = 8.0f;
	m_pLights[3].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
	m_pLights[3].m_fTheta = (float)cos(XMConvertToRadians(30.0f));
}

void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	BuildDefaultLightsAndMaterials();

	m_nGameObjects = 4;
	m_ppGameObjects = new CGameObject*[m_nGameObjects];

	CGameObject *pApacheModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Apache.bin");
	CApacheObject* pApacheObject = NULL;

	//pApacheObject = new CApacheObject();
	//pApacheObject->SetChild(pApacheModel, true);
	//pApacheObject->OnInitialize();
	//pApacheObject->SetPosition(2.0, 0.0f, 0.0f);
	//pApacheObject->Rotate(0.0f, 90.0f, 0.0f);
	//m_ppGameObjects[0] = pApacheObject;

	/*pApacheObject = new CApacheObject();
	pApacheObject->SetChild(pApacheModel, true);
	pApacheObject->OnInitialize();
	pApacheObject->SetPosition(0.0, 0.0f, 10.0f);
	pApacheObject->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[0] = pApacheObject;*/

	CGameObject *pGunshipModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Gunship.bin");
	CGunshipObject* pGunshipObject = NULL;

	pGunshipObject = new CGunshipObject();
	pGunshipObject->SetChild(pGunshipModel, true);
	pGunshipObject->OnInitialize();
	pGunshipObject->SetPosition(0.0, 0.0f, 20.0f);
	pGunshipObject->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[0] = pGunshipObject;

	CGameObject *pSuperCobraModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/SuperCobra.bin");
	CSuperCobraObject* pSuperCobraObject = NULL;

	pSuperCobraObject = new CSuperCobraObject();
	pSuperCobraObject->SetChild(pSuperCobraModel, true);
	pSuperCobraObject->OnInitialize();
	pSuperCobraObject->SetPosition(0.0, 0.0f, 30.0f);
	pSuperCobraObject->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[1] = pSuperCobraObject;

	CGameObject *pMi24Model = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Mi24.bin");
	CMi24Object* pMi24Object = new CMi24Object();
	pMi24Object->SetChild(pMi24Model, true);
	pMi24Object->OnInitialize();
	pMi24Object->SetPosition(0.0, 0.0f, 40.0f);
	pMi24Object->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[2] = pMi24Object;

	CGameObject* pHummerModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Hummer.bin");
	CHummerObject* pHummerObject = new CHummerObject();
	pHummerObject->SetChild(pHummerModel);
	pHummerObject->OnInitialize();
	pHummerObject->SetPosition(0.0, 0.0f, 50.0f);
	pHummerObject->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[3] = pHummerObject;

	////CGameObject* pFieldModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/BattleField.bin");
	//CBattleField* pFieldObject = new CBattleField(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	////pFieldObject->SetChild(pFieldModel, true);
	//pFieldObject->SetPosition(0.0f, -10.0f, 0.0f);
	//m_ppGameObjects[4] = pFieldObject;

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

	if (m_ppGameObjects)
	{
		for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Release();
		delete[] m_ppGameObjects;
	}

	ReleaseShaderVariables();

	if (m_pLights) delete[] m_pLights;
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_PARAMETER pd3dRootParameters[3];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	::memcpy(m_pcbMappedLights->m_pLights, m_pLights, sizeof(LIGHT) * m_nLights);
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &m_nLights, sizeof(int));
}

void CScene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}
}

void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->ReleaseUploadBuffers();
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	/*switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'W': m_ppGameObjects[0]->MoveForward(+1.0f); break;
		case 'S': m_ppGameObjects[0]->MoveForward(-1.0f); break;
		case 'A': m_ppGameObjects[0]->MoveStrafe(-1.0f); break;
		case 'D': m_ppGameObjects[0]->MoveStrafe(+1.0f); break;
		case 'Q': m_ppGameObjects[0]->MoveUp(+1.0f); break;
		case 'R': m_ppGameObjects[0]->MoveUp(-1.0f); break;
		default:
			break;
		}
		break;
	default:
		break;
	}*/
	return(false);
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;

	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->Animate(fTimeElapsed, NULL);

	if (m_pLights)
	{
		if(m_pPlayer) m_pLights[1].m_bEnable = m_pPlayer->m_bLight;
		m_pLights[1].m_xmf3Position = m_pPlayer->GetPosition();
		m_pLights[1].m_xmf3Direction = m_pPlayer->GetLookVector();
	}
}


void CScene::CheckCollisionByBullet()
{
	CBulletObject** ppBullets = ((CTankPlayer*)m_pPlayer)->m_ppBullets;
	CBulletObject** ppBulletsEnemy = NULL;

	for (int i = 0; i < m_nAITanks; i++)
	{
		ppBulletsEnemy = ((CTankAI*)m_ppAITanks[i])->m_ppBullets;

		// 적이 플레이어의 대포에 맞았는지 체크
		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBullets[j]->m_bFire)
			{
				if (CScene::CheckObjectByObjectCollisions(m_ppAITanks[i], ppBullets[j]))
				{
					// TO DO 
					ppBullets[j]->Reset();
					if (((CTankAI*)m_ppAITanks[i])->DecreaseHP(ppBullets[j]->m_fBulletDamage) == false)
					{
						m_ppAITanks[i]->SetActive(false);
					}
				}
			}
		}

		// 플레이어가 적의 대포에 맞았는지 체크
		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBulletsEnemy[j]->m_bFire)
			{
				if (CScene::CheckObjectByObjectCollisions(m_pPlayer, ppBulletsEnemy[j]))
				{
					// TO DO
					ppBulletsEnemy[j]->Reset();
					if (((CTankPlayer*)m_pPlayer)->DecreaseHP(ppBullets[j]->m_fBulletDamage) == false)
					{
						((CTankPlayer*)m_pPlayer)->ResetHP();
						m_pPlayer->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
					}
				}
			}
		}
	}
}

bool CScene::CheckCollisionByBuildings(CPlayer *pPlayer)
{
	// 플레이어가 빌딩에 닿았는지 체크
	for (int i = 0; i < BUILDINGS; i++)
	{
		CGameObject** ppBuildings = m_pBattleField->m_ppBuildingObjects;
		XMFLOAT3 offset = XMFLOAT3(2.0f, 5.0f, 2.0f);
		
		for (int i = 0; i < BUILDINGS; i++)
		{
			if (ppBuildings[i])
			{
				if (CheckObjectByObjectCollisions(pPlayer, ppBuildings[i]))
				{
					XMFLOAT3 pos = ppBuildings[i]->m_xmBoundingBox.Center;
					ppBuildings[i]->GetPosition();
					return true;
				}
			}
		}
	}

	return false;
}

void CScene::CheckCollisionPlayerByRunWay()
{
	XMFLOAT3 pos = m_pPlayer->GetPosition();
	float pos_y = 0.0f;
	float rot = 0.0f;
	float RotateAngle = 11.31f;

	if (m_pPlayer->m_xmBoundingBox.Intersects(m_pBattleField->m_RunWayField))
	{
		float pos_z = pos.z;

		// y = 1/5z - 12.4에 의거해 y값을 구해 위치를 적용한다.
		pos_y = (0.2 * pos_z) - 12;

		// 각도 회전은 닿았을 때 한번만 실행
		if (!m_pPlayer->m_bIsRunWay)
		{
			rot = (pos.z < 150.0f) ? -RotateAngle : RotateAngle;
			m_pPlayer->RotateAngle(rot, 0.0f, 0.0f);
			m_pPlayer->m_bIsRunWay = true;
		}
	}
	else
	{
		pos_y = (pos.z < 150.0f) ? 0.0f : 18.8f;

		// 다시 정상적으로 회전
		if (m_pPlayer->m_bIsRunWay)
		{

			rot = (pos.z < 150.0f) ? -RotateAngle : RotateAngle;
			m_pPlayer->RotateAngle(rot, 0.0f, 0.0f);
			m_pPlayer->m_bIsRunWay = false;
		}
	}


	m_pPlayer->SetPosition(XMFLOAT3(pos.x, pos_y, pos.z));
}

void CScene::CheckCollisionAIByRunWay()
{
	XMFLOAT3 pos;
	float rot;
	float RotateAngle = 11.31f;
	for (int i = 0; i < m_nAITanks; i++)
	{
		pos = m_ppAITanks[i]->GetPosition();
		rot = 0.0f;
		
		if (m_ppAITanks[i]->m_xmBoundingBox.Intersects(m_pBattleField->m_RunWayField))
		{
			// 각도 회전은 닿았을 때 한번만 실행
			if (!m_ppAITanks[i]->m_bIsRunWay)
			{
				rot = (pos.z < 150.0f) ? -RotateAngle : RotateAngle;
				((CGameObject*)m_ppAITanks[i])->Rotate(rot, 0.0f, 0.0f);
				m_ppAITanks[i]->m_bIsRunWay = true;
			}
		}
		else
		{
			// 다시 정상적으로 회전
			if (m_ppAITanks[i]->m_bIsRunWay)
			{
				rot = (pos.z < 150.0f) ? -RotateAngle : RotateAngle;
				((CGameObject*)m_ppAITanks[i])->Rotate(rot, 0.0f, 0.0f);
				m_ppAITanks[i]->m_bIsRunWay = false;
			}
		}
	}
}

bool CScene::CheckObjectByObjectCollisions(CGameObject* pGameObject, CGameObject* pTargetGameObject)
{
	if (pGameObject->m_xmBoundingBox.Intersects(pTargetGameObject->m_xmBoundingBox)) return(true);

	return false;
}

void CScene::SetPlayeBeforeCollidedPosition(XMFLOAT3 beforCollidedPos)
{
	float offset = 0.1f;

	XMFLOAT3 curPos = m_pPlayer->GetPosition();

	float changePosX = (curPos.x - beforCollidedPos.x >= 0) ? beforCollidedPos.x - offset : beforCollidedPos.x + offset;
	float changePosZ = (curPos.z - beforCollidedPos.z >= 0) ? beforCollidedPos.z - offset : beforCollidedPos.z + offset;

	m_pPlayer->SetPosition(XMFLOAT3(changePosX, beforCollidedPos.y, changePosZ));
}


void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights

	for (int i = 0; i < m_nGameObjects; i++)
	{
		if (m_ppGameObjects[i])
		{
			m_ppGameObjects[i]->Animate(m_fElapsedTime, NULL);
			m_ppGameObjects[i]->UpdateTransform(NULL);
			m_ppGameObjects[i]->Render(pd3dCommandList, pCamera);
		}
	}
}

