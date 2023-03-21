> ### 변환 파이프라인(Transformation Pipeline)

```
  3D 게임이 2D 게임 화면에 렌더링되는 과정 변환 파이프라인은 거친다.
  변환 파이프라인은 어떻게 진행되며 그 개념에 대해 알아보자.
```

<br>

<details>
  <summary><span style="border-bottom:0.05em solid"><strong>① 변환 파이프라인(Transformation Pipeline)</strong></span></summary>
<br>
     다음 그림과 같은 변환 파이프라인은 3D로 표현된 모델 좌표계의 정점을 입력하면 컴퓨
터 화면에 그릴 수 있도록 2D 화면(픽셀) 좌표로 변환하는 함수(Function)라고 간주할 수 
있다. 
<br>
 <img src="https://user-images.githubusercontent.com/36596037/226642655-d443c756-468a-4856-a6ba-3c70cf480a83.png">
<br>
  변환 파이프라인은 순차적으로 실행되는 4개의 단계(월드 변환(World Transform), 카메
라 변환(Camera Transform), 투영 변환(Projection Transform), 화면 변환(Screen 
Transform))로 구성된다. 4개의 단계 각각을 다음과 같은 함수로 표현할 수 있다.
<pre>
<code>
CVertex WorldTransform(CVertex vtxModel, CObject *pObject);
CVertex CameraTransform(CVertex vtxWorld);
CVertex ProjectionTransform(CVertex vtxCamera);
CVertex ScreenTransform(CVertex vtxProject);
</code>
</pre>
<br>
  함수 WorldTransform()은 모델 좌표계의 정점과 게임 객체를 입력으로 받아 모델 좌표
계의 정점 좌표를 월드 좌표계로 변환한다. 함수 CameraTransform()은 월드 좌표계의 
점을 카메라 좌표계로 변환한다. 카메라 좌표계는 플레이어의 눈(카메라)으로 게임 세상을 
바라볼 때 카메라의 위치와 방향을 기준으로 표현하는 좌표계이다. 현재는 이러한 카메라 
변환을 위하여 가상적인 카메라 객체가 있다고 가정하자. 함수 ProjectionTransform()은 
카메라 좌표계의 점을 투영 좌표계로 변환한다. 투영 좌표계는 카메라의 시야각(FOV: 
Field Of View)과 화면의 종횡비(가로와 세로 길이의 비율)에 따라 보정을 하기 위한 좌표
계이다. 함수 ScreenTransform()은 투영 좌표계의 점을 화면 좌표계로 변환한다. 
  <br>
  간단한 렌더링 과정을 함수로 표현하기 위하여 다음과 같은 자료구조를 사용한다. CVertex 클래스는 정점 또는 3차원 좌표를 표현하고, CPolygon 클래스는 하나의 다각형
을 표현하고, CMesh 클래스는 메쉬(모델)를 표현하고, CObject 클래스는 게임 객체를 표
현한다.
<pre>
<code>
class CVertex
{
 float x; 
 float y; 
 float z;
};
class CPolygon
{
 UINT nVertices;
 CVertex *pVertices;
};
class CMesh
{
 UINT nFaces;
 CPolygon *pFaces;
};
class CObject
{
 CMesh *pMesh;
 ...
}
</code>
</pre>
<br>
  
변환 파이프라인은 다음과 같은 함수 Transform()으로 표현할 수 있다. 이 함수는 렌더
링을 할 게임 객체와 모델 좌표계의 한 정점을 입력으로 받아 화면 좌표계의 픽셀 좌표를 
반환한다

<pre>
<code>
CVertex Transform(CVertex vtxModel, CObject *pObject) 
{
 CVertex vtxWorld = WorldTransform(vtxModel, pObject);
 CVertex vtxCamera = CameraTransform(vtxWorld);
 CVertex vtxProject = ProjectionTransform(vtxCamera);
 CVertex vtxScreen = ScreenTransform(vtxProject);
 return(vtxScreen);
}
</code>
</pre>
<br>

  렌더링은 게임 세상의 모든 게임 객체들을 그리는 것이라고 할 수 있으므로 렌더링을 다
음과 같은 함수 RenderObjects()로 표현할 수 있다. 함수 Draw()는 게임 객체의 메쉬를 
구성하는 모든 다각형에 대하여 다각형의 정점들을 픽셀 좌표로 변환하고 와인딩 순서대로 
선분으로 이어서 그리는 함수이다. Draw2DLine()는 화면의 두 픽셀을 하나의 선분으로 그
리는 함수이며 윈도우 API에서 LineTo(...)와 MoveTo(...)를 사용하여 작성할 수 있다. 
 <pre>
<code>
int gnObjects; //게임 객체의 개수
CObject *gpObjects; //게임 객체들의 배열
CCamera *gpCamera; //카메라 객체
void RenderObjects() 
{ 
 for (int i = 0; i < gnObjects; i++) Draw(&gpObjects[i]);
} 
void Draw(CObject *pObject) 
{ 
 CVertex vtxPrevious;
 for (int i = 0; i < pObject->pMesh->nFaces; i++) 
 { 
 CPolygon *pPolygon = &pObject->pMesh->pFaces[i]; 
 for (int j = 0; j < pPolygon->nVertices; j++) 
 { 
 CVertex v = Transform(pPolygon->pVertices[j], pObject);
 if (j != 0) Draw2DLine(vtxPrevious.x, vtxPrevious.y, v.x, 
v.y);
 vtxPrevious = v;
 }
 }
} 
void Draw2DLine(float x0, float y0, float x, float y)
{
 HDC hDC = GetDC(...);
 ::MoveToEx(hDC, (long)x0, (long)y0, NULL);
 ::LineTo(hDC, (long)x, (long)y);
 ::ReleaseDC(...);
}
</code>
</pre>
<br>
  
</details>
<br>
