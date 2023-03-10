#include "stdafx.h"
#include "Mesh.h"
#include "GraphicsPipeline.h"

CPolygon::CPolygon(int nVertices)
{
	m_nVertices = nVertices;
	m_pVertices = new CVertex[nVertices];
}

CPolygon::~CPolygon()
{
	// Vertex가 존재하면 삭제
	if (m_pVertices) delete[] m_pVertices;
}

void CPolygon::SetVertex(int nIndex, CVertex vertex)
{
	// 인덱스가 Vertex 개수보다 작고 Vertex 배열이 존재한다면 들어온 Vertex를 해당 인덱스에 세팅한다.
	if ((0 <= nIndex) && (nIndex < m_nVertices) && m_pVertices)
	{
		m_pVertices[nIndex] = vertex;
	}
}

CMesh::CMesh(int nPolygons)
{
	// Polygon 개수만큼 정점들의 정보를 가지고 있는 폴리곤 배열 생성
	m_nPolygons = nPolygons;
	m_ppPolygons = new CPolygon * [nPolygons];
}

CMesh::~CMesh()
{
	// Polygon 배열이 존재하면 폴리곤 개수만큼 존재하는 정점들을 삭제
	if (m_ppPolygons)
	{
		for (int i = 0; i < m_nPolygons; ++i)
			if (m_ppPolygons[i]) delete[] m_ppPolygons;
	}
}

void CMesh::SetPolygons(int nIndex, CPolygon* pPolygon)
{
	// 메쉬의 다각형을 설정한다
	if ((0 <= nIndex) && (nIndex < m_nPolygons)) m_ppPolygons[nIndex] = pPolygon;
}

void CMesh::Render(HDC hDCFrameBuffer)
{
	CPoint3D f3InitialProject, f3PreviousProject, f3Intersect;
	bool bPreviousInside = false
		, bInitialInside = false
		, bCurrentInside = false
		, bIntersectInside = false;

	// 메쉬를 구성하는 모든 다각형들을 렌더링한다.
	for (int j = 0; j < m_nPolygons; ++j)
	{
		//int nVertices = m_ppPolygons[j]->m_nVertices;
		//CVertex* pVertices = m_ppPolygons[j]->m_pVertices;

		//// 다각형의 첫 번째 정점을 원근 투영 변환한다.
		//f3PreviousProject = f3InitialProject = CGraphicsPipeline::Project(pVertices[0].m_f3Position);

		//// 변환된 점이 투영 사각형에 포함되는 가를 계산한다.
		//bPreviousInside = bInitialInside
		//	= (-1.0f <= f3InitialProject.x) && (f3InitialProject.x <= 1.0f)
		//	&& (-1.0f <= f3InitialProject.y) && (f3InitialProject.y <= 1.0f);

		//// 다각형을 구성하는 모든 정점들은 원근 투영을 변환하고 선분으로 렌더링한다.
		//for (int i = 1; i < nVertices; ++i)
		//{
		//	CPoint3D f3CurrentProject = CGraphicsPipeline::Project(pVertices[i].m_f3Position);

		//	// 변환된 점이 투영 사각형에 포함되는 가를 계산한다.
		//	bCurrentInside = (-1.0f <= f3CurrentProject.x) &&
		//		(f3CurrentProject.x <= 1.0f) && (-1.0f <= f3CurrentProject.y) &&
		//		(f3CurrentProject.y <= 1.0f);

		//	// 변환된 점이 투영 사각형에 포함되면 이전 점과 현재 점을 선분으로 그린다.
		//	if (((f3PreviousProject.z >= 0.0f) || (f3CurrentProject.z >= 0.0f)) && ((bCurrentInside || bPreviousInside)))
		//	{
		//		::Draw2DLine(hDCFrameBuffer, f3PreviousProject, f3CurrentProject);
		//		f3PreviousProject = f3CurrentProject;
		//		bPreviousInside = bCurrentInside;
		//	}

		//	// 다각형의 마지막 정점과 다각형의 시작점을 선분으로 그린다.
		//	if (((f3PreviousProject.z >= 0.0f) || (f3InitialProject.z >=
		//		0.0f)) && ((bInitialInside || bPreviousInside)))
		//		::Draw2DLine(hDCFrameBuffer, f3PreviousProject, f3InitialProject);
		//}
	}
}

//// 전역함수
//void Draw2DLine(HDC hDCFrameBuffer, CPoint3D& f3PreviousProject, CPoint3D& f3CurrentProject)
//{
//	// 투영좌표계 -> 화면좌표계 변환 과정
//	// 투영좌표계의 2점을 화면 좌표계로 변환하고 변환된 두 점(픽셀)을 선분으로 그린다
//	CPoint3D f3Previous = CGraphicsPipeline::ScreenTransform(f3PreviousProject);
//	CPoint3D f3Current = CGraphicsPipeline::ScreenTransform(f3CurrentProject);
//
//	// MoveToEx 함수는 현재 위치를 지정된 지점으로 업데이트하고 필요에 따라 이전 위치를 반환합니다
//	::MoveToEx(hDCFrameBuffer, (long)f3Previous.x, (long)f3Previous.y, NULL);
//
//	// LineTo 함수는 현재 위치에서 지정된 지점까지 선을 그립니다.
//	::LineTo(hDCFrameBuffer, (long)f3Previous.x, (long)f3Previous.y);
//}
