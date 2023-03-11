#include "stdafx.h"
#include "GameFramework.h"

void CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	// 렌더링 화면 생성
	BuildFrameBuffer();

	// 플레이어와 게임 세계(씬)을 생성
	BuildObjects();
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
	if (m_hDCFrameBuffer) ::DeleteDC(m_hDCFrameBuffer);
}

void CGameFramework::BuildFrameBuffer()
{
	/* 
		화면 깜박임을 줄이기 위하여 윈도우의 클라이언트 영역의 디바이스 컨텍스트를 사용하여 직접 클라이언트 표면에 그리지 않는다. 
		BuildFrameBuffer() 함수는 클라이언트 표면을 대신할 비트맵 생성하고 이 비트맵을 메모리 디바이스 컨텍스트로 생성한다. 
		ClearFrameBuffer() 함수는 렌더링을 시작하기 전에 비트맵 표면을 원하는 색상으로 지운다. 
		씬을 화면(클라이언트 영역)으로 렌더링하기 위하여 먼저 씬의 게임 객체들을 비트맵 표면으로 렌더링한다. 
		PresentFrameBuffer() 함수는 비트맵 표면으로 렌더링된 비트맵을 클라이언트 영역으로 옮긴다
	*/

	::GetClientRect(m_hWnd, &m_rcClient);

	HDC hDC = ::GetDC(m_hWnd);
	m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC
							, m_rcClient.right - m_rcClient.left
							, m_rcClient.bottom - m_rcClient.top);

	::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);
	::ReleaseDC(m_hWnd, hDC);
	::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void CGameFramework::ClearFrameBuffer(DWORD dwColor)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 0, dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(m_hDCFrameBuffer, hPen);
	HBRUSH hBrush = ::CreateSolidBrush(dwColor);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDCFrameBuffer, hBrush);

	::Rectangle(m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom);
	::SelectObject(m_hDCFrameBuffer, hOldBrush);
	::SelectObject(m_hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
}

void CGameFramework::PresentFrameBuffer()
{
	HDC hDC = ::GetDC(m_hWnd);
	::BitBlt(hDC
		, m_rcClient.left
		, m_rcClient.top
		, m_rcClient.right - m_rcClient.left
		, m_rcClient.bottom - m_rcClient.top
		, m_hDCFrameBuffer
		, m_rcClient.left
		, m_rcClient.top
		, SRCCOPY);
	::ReleaseDC(m_hWnd, hDC);
}

void CGameFramework::BuildObjects()
{
	/*
		BuildObjects() 함수는 카메라를 생성하여 설정하고, 플레이어 객체를 생성하고, 
		그리고 씬(게임 세계)을 생성한다. ReleaseObjects() 함수는 BuildObjects() 함수에서 생성한 객체들을 소멸한다.
	*/

	// 카메라를 생성하고 뷰포트와 시야각(FOV)을 설정한다.
	CCamera* pCamera = new CCamera();
	pCamera->SetViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	pCamera->SetFOVAngle(60.0f);

	// 플레이어 게임 객체를 생성하고 카메라와 위치를 설정한다.
	m_pPlayer = new CPlayer();
	m_pPlayer->SetCamera(pCamera);
	m_pPlayer->SetPosition(0.0f, 3.0f, -40.0f);

	// 빈 객체를 생성하고 게임 객체들을 생성한다.
	m_pScene = new CScene();
	m_pScene->BuildObjects();
}

void CGameFramework::ReleaseObjects()
{
	// 씬 객체의 게임 객체들을 소멸하고, 씬 객체와 플레이어 객체를 소멸한다.
	if (m_pScene) m_pScene->ReleaseObjects();
	if (m_pScene) delete m_pScene;
	if (m_pPlayer) delete m_pPlayer;
}

void CGameFramework::ProcessInput()
{
	// ProcessInput() 함수는 키보드 입력을 처리하여 플레이어 객체를 이동

	static UCHAR pKeyBuffer[256];
	if (::GetKeyboardState(pKeyBuffer))
	{
		float cxKeyDelta = 0.0f, cyKeyDelta = 0.0f, czKeyDelta = 0.0f;
		if (pKeyBuffer[VK_UP] & 0xF0) czKeyDelta =+ 0.125f;
		if (pKeyBuffer[VK_DOWN] & 0xF0) czKeyDelta =- 0.125f;
		if (pKeyBuffer[VK_LEFT] & 0xF0) cxKeyDelta =- 0.125f;
		if (pKeyBuffer[VK_RIGHT] & 0xF0) cxKeyDelta =+ 0.125f;
		if (pKeyBuffer[VK_PRIOR] & 0xF0) cyKeyDelta =+ 0.125f;
		if (pKeyBuffer[VK_NEXT] & 0xF0) cyKeyDelta =- 0.125f;

		m_pPlayer->Move(cxKeyDelta, cyKeyDelta, czKeyDelta);
	}
}

void CGameFramework::AnimateObjects()
{
	// AnimateObjects() 함수는 씬의 게임 객체들을 애니메이션한다.

	if (m_pScene) m_pScene->Animate(1.0f / 60.f);
}

void CGameFramework::FrameAdvance()
{
	/*
		FrameAdvance() 함수는 윈도우 메시지 루프에서 반복적으로 호출된다. 
		즉, 응용프로그램이 실행되는 동안 이 함수가 반복적으로 계속 실행된다. 
		이 함수는 사용자 입력을 받아 플레이어 또는 게임 세계의 게임 객체들을 움직이고 그 결과에 따라 게임 세계를 화면으로 렌더링한다.
	*/

	// 사용자 입력을 처리한다.
	ProcessInput();

	// 게임 세계를 애니메이션(움직이게)한다.
	AnimateObjects();

	// 렌더링을 할 대상 화면(비트맵)을 지운다.
	ClearFrameBuffer(RGB(90, 103, 224));

	// 씬을 렌더링한다.
	CCamera* pCamera = m_pPlayer->GetCamera();
	if (m_pScene) m_pScene->Render(m_hDCFrameBuffer, pCamera);

	// 렌더링을 한 화면(비트맵)을 클라이언트 영역으로 복사한다.
	PresentFrameBuffer();
}
