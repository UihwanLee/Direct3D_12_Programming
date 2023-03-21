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
      if (j != 0) Draw2DLine(vtxPrevious.x, vtxPrevious.y, v.x, v.y);
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

<details>
  <summary><span style="border-bottom:0.05em solid"><strong>② 평행이동(Translation) 변환</strong></span></summary>
<br>
     직교 좌표계에서 평행이동 변환은 점을 좌표축에 평행하게 이동하는 변환이다. 3차원 직
교 좌표계의 점 (x, y, z)를 x축으로 a 만큼, y축으로 b 만큼, z축으로 c 만큼 평행이동한 
점은 (x+a, y+b, z+c) 가 된다. 3차원 직교 좌표계의 평행이동 변환 T는 다음과 같이 표현
할 수 있다.
<pre>
<code>
T: (x, y, z) → (x+a, y+b, z+c)
</code>
</pre>
<br>

  모델 좌표계의 원점과 월드 좌표계의 원점이 같고 두 좌표계의 좌표축의 방향이 같다고 
가정하자(실제로 두 원점이 같지 않아도 상관없다). 월드 좌표계는 게임 객체의 위치와 방
향을 표현하기 위하여 사용하는데 게임 객체의 회전이 없다고 가정하면, 월드 좌표는 게임 
객체의 위치만을 표현하는 것이고, 모델 좌표계의 점 (x, y, z)는 월드 좌표계에서도 (x, y, 
z)가 된다. 게임 객체의 위치는 게임 객체(모델)의 중심이 월드 좌표계의 어떤 점에 대응되
는 가를 표현하는 것이다. 특별한 이유가 없다면 모델의 중심 즉, 모델 좌표계의 원점은 
(0, 0, 0)이라고 가정할 수 있다. 월드 좌표계의 원점도 (0, 0, 0)이라고 가정하자.
<br>
  월드 좌표계에서 게임 객체의 위치가 (a, b, c)이라고 가정하자. 점 (a, b, c)는 월드 좌
표계의 원점 (0, 0, 0)을 x축으로 a 만큼, y축으로 b 만큼, z축으로 c 만큼 평행이동한 결
과와 같다. 그러면 모델 좌표계의 원점 (0, 0, 0)도 같은 평행이동을 하게 된다. 모델의 모
든 정점들에 대하여 같은 평행이동을 적용하면 월드 좌표계로 표현된 정점들을 구할 수 있
다. 
  <br>
  정리하면, 게임 객체의 위치가 (a, b, c)일 때 모델의 모든 정점들에 대하여 (a, b, c) 만
큼의 평행이동을 적용하여 월드 좌표계의 모델 좌표를 얻을 수 있다는 것이다. 즉, 월드 
좌표계로 표현된 게임 객체의 위치가 모델 좌표계의 원점이 이동한 위치가 되며, 그리고 
게임 객체의 위치가 모델의 각 정점들에 대한 평행이동의 양을 의미한다. 
<br>
  
다음 그림은 게임 객체의 위치가 (1, 3, 3)일 때 평행이동 변환에 의해 모델 좌표계의 원
점이 월드 좌표계 (1, 3, 3)이 되고, 모델 좌표계의 (2, -2, -2)가 월드 좌표계 (3, 1, 1)이 
되는 것을 보이고 있다.
<br>
   <img src="https://user-images.githubusercontent.com/36596037/226646900-e773c262-8160-4207-9866-ee9696c8acbe.png">
<br>
</details>
<br>
  
<details>
  <summary><span style="border-bottom:0.05em solid"><strong>③ 인스턴싱(Instancing)</strong></span></summary>
<br>
    직여러 게임 객체들이 같은 외관(겉모양)을 가지면 각 게임 객체가 모델을 따로 가질 필요
가 없이 하나의 모델을 서로 공유하면 된다. 모델의 정보는 렌더링 과정에서 바뀌지 않음
에 유의하라. 이렇게 같은 외관을 갖는 게임 객체들이 하나의 모델을 공유하게 표현하고 
렌더링하는 것을 인스턴싱이라고 한다.
<br>

  평행이동 변환만을 고려할 때 인스턴싱을 위해 게임 객체를 C++ 프로그래밍 언어로 다
음과 같이 표현할 수 있다. 렌더링을 위하여 게임 객체가 필수적으로 가져야 하는 정보는 
위치(월드 좌표계)와 메쉬(모델)에 대한 포인터이다. xPosition, yPosition, zPosition는 게
임 객체의 위치가 원점에서 x-축, y-축, z-축 방향으로의 평행이동할 양을 나타내고, pMesh는 게임 객체의 외관을 나타내는 메쉬에 대한 포인터이다. 
<pre>
<code>
class CObject
{
  public:
    CMesh *pMesh;
    float xPosition;
    float yPosition;
    float zPosition;
};
</code>
</pre>
<br>
  <img src="https://user-images.githubusercontent.com/36596037/226647461-23725114-605b-473c-bc35-98e44b6f8b25.png">
<br>
  평행이동 변환만을 고려할 때 변환 파이프라인의 함수 WorldTransform()은 다음과 같
이 표현할 수 있다.
  <br>
  <pre>
<code>
CVertex WorldTransform(CVertex vtxModel, CObject *pObject) 
{
  CVertex vtxWorld;
  vtxWorld.x = vtxModel.x + pObject->xPosition;
  vtxWorld.y = vtxModel.y + pObject->yPosition;
  vtxWorld.z = vtxModel.z + pObject->zPosition; 
  return(vtxWorld);
}
</code>
</pre>
<br>
</details>
<br>
  
<details>
  <summary><span style="border-bottom:0.05em solid"><strong>④ 회전(Rotation) 변환</strong></span></summary>
<br>
다음 그림은 2차원 평면에서 원점을 중심으로 점 (x1, y1)을 θ만큼 반시계방향으로 회전
한 결과가 점 (x2, y2)임을 나타내고 있다. 점 (x2, y2)를 (x1, y1)과 로 표현해보자.
<br>
    <img src="https://user-images.githubusercontent.com/36596037/226649013-57f787a1-8f8f-4409-9aa2-3b8275fdfdd4.png">
<br>
  2차원 평면에서 반지름이 인 원주 위의 점 (x1, y1)는 다음과 같이 표현할 수 있다.
 <br>
  <img src="https://user-images.githubusercontent.com/36596037/226649020-e15c4bb5-b9fd-449f-8454-edf986cfd42a.png">
  <br>
점 (x2, y2)를 (x1, y1)과 θ로 표현하면 다음과 같다. 
 <br>
  <img src="https://user-images.githubusercontent.com/36596037/226649020-e15c4bb5-b9fd-449f-8454-edf986cfd42a.png">
<br>
   <img src="https://user-images.githubusercontent.com/36596037/226649021-0686dc1b-984b-4b24-9859-313b0301ef9b.png">
<br>
이것에 대한 증명은 다음 그림을 참고하라. 증명 과정을 이해할 필요는 없다. 또한 그 
결과를 외울 필요도 없다. 단지 “2차원 좌표계에서 어떤 점을 원점을 중심으로 회전을 할 
때 그 회전의 결과를 수학적으로 구할 수 있다”라는 정도만 기억하도록 하자

<br>
<img src="https://user-images.githubusercontent.com/36596037/226651321-b1e186dc-004e-474f-9ec0-224242c1a65a.png">
<br>
  이제 3차원 좌표계(왼손 좌표계)에서 z-축을 중심으로 회전하는 경우를 생각해보자. 위의 
그림에서 파란색 원은 z-축에 해당한다. 그리고 2차원 좌표계의 점 (x1, t1)과 (x2, y2)는 3
차원 좌표계의 점 (x1, y1, 0)과 (x2, y2, 0)이라고 생각할 수 있다. 3차원 좌표계(왼손 좌표계)
에서 z-축을 중심으로 회전하는 것은 2차원 좌표계의 원점을 중심으로 회전하는 것과 같다. 
  그리고 3차원 좌표계에서 임의의 점 (x1, y1, z)를 z-축을 중심으로 회전하면 (x2, y2, z)가 된다. 
  왜냐하면 z-축을 중심으로 회전하면 z-좌표는 변하지 않기 때문이다.
 3차원 좌표계(왼손 좌표계)에서 점 (x1, y1, z)를 z-축을 중심으로 θ만큼 회전한 결과 (x2, y2, z)는 다음과 같이 표현할 수 있다.
<br>
  <img src="https://user-images.githubusercontent.com/36596037/226651418-d581646b-a4fe-47ed-9584-3536ff66dd58.png">
<br>
  3차원 좌표계에서 좌표축을 중심으로 회전할 때 회전의 방향은 회전축의 +방향의 점에
서 원점을 바라볼 때를 기준으로 한다. 시계방향의 회전을 양(+)의 회전, 그리고 반시계방
향의 회전을 음(-)의 회전 방향으로 정한다. 그러면 2차원 좌표계에서 원점을 중심으로 반
시계방향으로 회전하는 것을 양(+)의 회전으로 표현하는 것과 회전의 방향이 일치하게 된
다. 
<br>
<br>
  이제 3차원 좌표계(왼손 좌표계)에서 x-축을 중심으로 회전하는 경우를 생각해보자. 위
의 그림에서 파란색 원을 x-축이라고 가정하면 빨간색 x는 y-축에 해당하고, 초록색 y는 
z-축에 해당한다(다음 그림 참조). 3차원 좌표계의 점 (x, y1, z1)과 (x, y2, z2)는 2차원 좌표
계의 점 (y1, z1)과 (y2, z2)에 대응된다.
<br>
  <img src="https://user-images.githubusercontent.com/36596037/226652112-b49ac4d9-9578-45bb-be09-57e680195448.png">
<br>
   3차원 좌표계(왼손 좌표계)에서 점 (x, y1, z1)를 x-축을 중심으로 θ만큼 회전하면 x-좌표
는 변하지 않기 때문에 회전의 결과 (x, y2, z2)는 다음과 같이 표현할 수 있다.
<br>
  <img src="https://user-images.githubusercontent.com/36596037/226652116-9d3e86c3-6248-47dc-a0ee-3e99d5b9d407.png">
<br>
  같은 방법으로 3차원 좌표계(왼손 좌표계)에서 점 (x1, y, z1)를 y-축을 중심으로 θ만큼 
회전하면 y-좌표는 변하지 않기 때문에 회전의 결과 (x2, y, z2)는 다음과 같이 표현할 수 
있다. 
<br>
  <img src="https://user-images.githubusercontent.com/36596037/226652120-cc6c9e98-ac55-40a0-a3c0-d21ff00317f6.png">
<br>
  정리하면, 3차원 좌표계(왼손 좌표계)에서 점(x1, y1, z1)를 어떤 좌표축을 중심으로 θ만
큼 회전하면 좌표축에 해당하는 좌표는 바뀌지 않으며, 나머지 좌표를 2차원 좌표계에서 
원점을 중심으로 회전하는 방법으로 회전의 결과를 구하는 것과 같다는 것이다. 즉, 3차원 
좌표계(왼손 좌표계)에서 좌표축을 중심으로 회전하는 것은 2차원 좌표계에서 원점을 중심
으로 회전하는 것과 같다.
<br>
</details>
<br>
  
  <details>
  <summary><span style="border-bottom:0.05em solid"><strong>⑤ 평행이동 변환과 회전 변환의 적용 순서</strong></span></summary>
<br>
 회전 변환은 기본적으로 좌표계의 원점 또는 원점을 지나는 좌표축을 기준으로 하는 것
으로 가정한다. 평행이동 변환과 회전 변환이 같이 적용될 때 변환을 하는 순서가 중요하
다. 평행이동 변환을 먼저 하고 회전 변환을 나중에 하는 결과와 회전 변환을 먼저 하고 
평행이동 변환을 나중에 하는 결과는 다르다. 
<br>
<br>
다음 그림은 직육면체를 y-축 방향으로 2만큼 평행이동하는 변환과 z-축을 중심으로 
45° 회전하는 변환을 같이 적용할 때 변환의 적용 순서에 따라 변환의 최종 결과가 다르다
는 것을 보여준다. 왼쪽 그림은 직육면체를 y-축 방향으로 2만큼 평행이동을 먼저하고 그 
결과를 z-축을 중심으로 45° 회전하는 것을 나타낸다. 오른쪽 그림은 직육면체를 먼저 z축을 중심으로 45° 회전하고 그 결과를 y-축 방향으로 2만큼 평행이동하는 것을 나타낸다. 즉, 평행이동 변환과 회전 변환이 같이 적용될 때 변환을 하는 순서가 중요함을 보이고 있
다. 왼쪽 그림에서 직육면체를 z-축을 중심으로 45° 회전할 때 회전의 중심이 객체(메쉬)의 
중심이 아니라 좌표계(월드 좌표계)의 원점을 지나는 z-축이다. 오른쪽 그림에서 직육면체
를 z-축을 중심으로 45° 회전할 때 회전의 중심이 객체(메쉬)의 중심이 좌표계(월드 좌표
계)의 원점을 지나는 z-축과 일치한다. 
<br>
<br>
    왼쪽 그림에서와 같이 평행이동을 먼저하고 회전을 나중에 하면 회전은 좌표계의 원점을 
기준으로 회전(공전)하게 된다. 오른쪽 그림에서와 같이 회전을 먼저하고 평행이동을 나중
에 하면 회전은 객체(메쉬)의 원점을 기준으로 회전(자전)하게 된다.
<br>
 <img src="https://user-images.githubusercontent.com/36596037/226653543-d4697d1f-e8f3-499a-9cca-d0a84c50025a.png">
<br>
일반적으로 특별한 상황이 아니면 모든 객체의 회전은 자전(객체의 중심을 기준으로 회
전)하는 것으로 가정한다.
<br>
 태양계에서 태양, 지구, 달은 자전을 하면서 공전을 한다. 달이 자전을 하면서 지구 주위
를 공전을 하고, 지구는 자전을 하면서 태양 주위를 공전을 한다. 태양, 지구, 달의 움직임
을 회전과 평행이동으로 표현할 수 있는가? 
<br>
</details>
<br>
  
  <details>
  <summary><span style="border-bottom:0.05em solid"><strong>⑥ 회전 변환과 평행이동이 있는 게임 객체의 표현</strong></span></summary>
<br>
 일반적으로 게임 객체는 평행이동과 회전을 모두 할 수 있다. 평행이동을 하면 게임 객
체의 위치가 변하고, 회전을 하면 게임 객체의 방향이 변한다. 게임 객체는 월드 좌표계에
서의 위치와 방향을 표현할 수 있어야 한다. 회전 변환과 평행이동 변환을 모두 할 수 있
는 게임 객체를 C++ 프로그래밍 언어로 다음과 같이 표현할 수 있다
<br>
  <pre>
<code>
class CObject
{
  public:
    CMesh *pMesh; 
    float xPosition;
    float yPosition;
    float zPosition;
    float xRotation;
    float yRotation;
    float zRotation;
};
</code>
</pre>
<br>
    <img src="https://user-images.githubusercontent.com/36596037/226654238-20c6a35c-52c4-4586-b31f-0b566749b3a7.png">
<br>
xRotation, yRotation, zRotation는 x-축, y-축, z-축을 중심으로 회전하는 양을 나타
낸다. x-축, y-축, z-축을 중심으로 회전하는 양을 피치(Pitch), 요(Yaw), 롤(Roll)이라고 
한다.
<br>
<pre>
<code>
CVertex WorldTransform(CVertex vtxModel, CObject *pObject)
{ 
  float fPitch = pObject->xRotation;
  float fYaw = pObject->yRotation;
  float fRoll = pObject->zRotation;
  CVertex vtxWorld = vtxModel, vtxRotated;

  if (fPitch) {
    vtxRotated.y = vtxWorld.y * cos(fPitch) - vtxWorld.z * sin(fPitch);
    vtxRotated.z = vtxWorld.y * sin(fPitch) + vtxWorld.z * cos(fPitch);
    vtxWorld = vtxRotated;
    }
 if (fYaw) {
    vtxRotated.x = vtxWorld.x * cos(fYaw) + vtxWorld.z * sin(fYaw);
    vtxRotated.z = -vtxWorld.x * sin(fYaw) + vtxWorld.z * cos(fYaw);
    vtxWorld = vtxRotated;
    }
 if (fRoll) {
    vtxRotated.x = vtxWorld.x * cos(fRoll) - vtxWorld.y * sin(fRoll);
    vtxRotated.y = vtxWorld.x * sin(fRoll) + vtxWorld.y * cos(fRoll);
    vtxWorld = vtxRotated;
    }
  vtxWorld.x += pObject->xPosition;
  vtxWorld.y += pObject->yPosition;
  vtxWorld.z += pObject->zPosition; 
  return(vtxWorld);
}
</code>
</pre>
<br>
  
</details>
<br>

  <details>
  <summary><span style="border-bottom:0.05em solid"><strong>⑦ 카메라 변환(Camera Transformation, Viewing Transformation)</strong></span></summary>
<br>
 ■ 가상 카메라(Virtual Camera)
<br>
  게임 세계를 렌더링하기 위해서는 게임 세계를 보기 위한 가상의 카메라(객체)가 필
요하다. 실세계에서 사람이 카메라 또는 눈을 통해 세상의 일부를 볼 수 있는 것처럼, 게임 플레이어는 이 가상 카메라를 통해 게임 세계의 일부를 볼 수 있다. 3차원 공간의 
게임 객체들은 가상 카메라의 2차원 평면으로 투영되고 이 투영된 2차원 평면의 장면
이 화면에 그려져야 한다. 그러므로 게임 플레이어가 현재 보고 있는 장면은 가상 카메
라에 나타나는 장면이다. 일반적으로 이 가상 카메라는 플레이어 캐릭터에 부착되어 있
다. 1인칭 게임의 경우 플레이어 캐릭터의 눈이 카메라에 해당한다. 카메라에 보이는 
게임 객체들만 화면에 최종적으로 그려지게 된다(앞으로 가상 카메라를 카메라로 표기
함). 
<br>
  카메라가 가져야 하는 일반적인 정보는 다음과 같다. 
  <ul>
    <li>카메라의 위치(Position) : 월드 좌표계에서 카메라가 어디에 있는 가를 표현</li>
    <li>카메라의 방향(Viewing Direction) : 월드 좌표계에서 카메라가 바라보는 방향(카메라의 회전)을 표현</li>
    <li>카메라의 화각(FOV(Field Of View)) : 카메라가 바라보는 시야 각도(범위)를 표현</li>
  </ul>
  
<img src="(https://user-images.githubusercontent.com/36596037/226655969-5f3023fd-fb30-4c4c-a13c-10c033e536dc.png">
<br>
<br>
 ■ 카메라 좌표계(View Space, Camera Space)
 <br>
3차원 게임에서 카메라가 이동을 하면 위치가 변하게 된다. 게임 세계에서 카메라가 
현재 어느 위치에 있는 가는 월드 좌표계로 표현되어야 한다. 카메라가 회전을 하면 카
메라가 바라보는 방향이 달라진다. 카메라가 바라보는 방향은 왼손 좌표계에서 z-축 방
향이다. 일반적으로 카메라의 회전은 카메라의 중심을 기준으로 이루어져야 한다(자전). 
카메라 좌표계는 카메라의 중심 위치(월드 좌표계)를 원점으로 하며 회전에 따라 달라
진 방향을 좌표축으로 표현되는 좌표계이다. 게임 세계를 카메라를 중심으로 한 상대적
인 좌표계로 표현하는 변환이 카메라 변환이다.
<br>
<img src="https://user-images.githubusercontent.com/36596037/226656793-368f9803-6164-4475-900f-94cb252d5def.png">
<br>
다음은 간단한 카메라를 표현하고 있다. (xPosition, yPosition, zPosition)는 월드 좌표
계에서 가상 카메라의 위치, 즉, 가상 카메라가 게임 세계의 어디에 위치하는 가를 나타낸
다. (xRotation, yRotation, zRotation)는 가상 카메라의 방향을 나타낸다.
<br>
<pre>
<code>
class CCamera
{
  public:
    float xPosition; //카메라의 위치(월드 좌표계) float yPosition;
    float zPosition;
    float xRotation; //카메라의 방향(회전 각도)
    float yRotation; 
    float zRotation; 
    float fovAngle; //카메라의 화각
}
</code>
</pre>
<br>
<br>
<br>
  
</details>
<br>
  
<details>
<summary><span style="border-bottom:0.05em solid"><strong>⑧ 카메라 변환(Camera Transformation)</strong></span></summary>
<br>
 ■ 가상 카메라(Virtual Camera)
<br>
  카메라를 이동하고 회전할 때 주의할 점은 카메라의 움직임과 화면상의 게임 객체의 움
직임은 방향이 서로 반대라는 것이다. 예를 들어, 카메라를 앞으로 움직이면, 화면상에서 
게임 세계가 전체적으로 카메라의 이동 방향과 반대로(카메라에 가까워지는 방향으로) 이동
하는 것처럼 보이게 된다. 이것은 카메라를 움직이지 않고 게임 세계의 게임 객체들을 카
메라 이동 방향과 반대 방향으로 이동하는 것과 화면상의 결과와 같다. 또한, 카메라를 왼
쪽으로 이동하면 화면상에서 게임 세계는 오른쪽으로 이동하게 된다. 카메라를 회전하는 
경우도 화면상에서 게임 세계는 반대 방향으로 회전을 한다
<br>
<br>
  카메라 변환은 월드 좌표계로 표현된 점을 카메라 좌표계로 변환하는 것이다. 카메라를 
회전하고 이동하기 전에, 카메라가 월드 좌표계의 원점에 위치하고, 카메라(객체) 지역 좌
표계의 x-축, y-축, z-축의 방향이 월드 좌표계의 x-축, y-축, z-축과 각각 일치한다고 가
정하자. 카메라를 y-축을 중심으로 시계 방향으로 90° 회전(자전)하였고, 평행이동에 의해 
카메라의 위치가 (4, 0, 2)가 되었다. 그리고 (8, 0, 3)의 위치에 게임 객체(직육면체)가 있
다. 다음 그림은 이 상황을 나타내고 있다. 이때 카메라는 직육면체의 오른쪽 부분을 보게
된다.
<br>
   <img src="https://user-images.githubusercontent.com/36596037/226656800-9c342423-cc8b-4558-944d-9932a9cd5c1a.png">
<br>
 위의 그림과 같은 위치와 방향을 가진 카메라에 대한 카메라 변환 과정을 생각해보자. 카메라 변환은 월드 좌표계로 표현된 점들의 좌표를 카메라의 중심과 방향을 사용하여 상
대적으로 표현하는 과정이다. 다르게 말하면 게임 세계가 월드 좌표계의 기준(월드 좌표계
의 원점과 축 방향)이 아니라 카메라를 기준으로 표현되어야 한다는 것이다(카메라가 게임 
세계를 표현하는 중심이다 또는 카메라가 세상의 중심이다). 월드 좌표계의 점을 카메라의 
위치와 방향에 따라 직접 변환하지 않고, 카메라가 원래 세상의 기준(월드 좌표계의 원점
과 축 방향)이 되도록 변환하도록 하자. “게임 객체 또는 카메라가 이동하고 회전을 하더
라도 월드 좌표계의 기준은 불변이다”라는 것에 주의하라. 
<br>
<br>
다음 그림은 카메라를 월드 좌표계의 원점으로 이동하는 변환을 월드 좌표계로 표현된 
정점에 대해서도 같게 적용한 것을 나타낸다. 카메라의 위치가 (4, 0, 2)이므로 카메라를 
월드 좌표계의 원점으로 이동하려면 x-축으로 –4, z-축으로 –2 만큼 평행이동을 한다. 게
임 객체(직육면체)의 위치 (8, 0, 3)에 같은 평행이동을 적용하면 위치가 (4, 0, 1)이 된다. (4, 0, 1)은 카메라의 위치를 기준으로 직육면체가 상대적으로 떨어져 있는 위치가 된다. 이제 카메라는 변하지 않는 월드 좌표계의 기준(원점)에 있게 되었다. 
<br>
<img src="https://user-images.githubusercontent.com/36596037/226656801-c87418bf-b235-4e85-8001-8bb7f9cf8d40.png">
<br>
카메라를 y-축을 중심으로 회전을 했기 때문에 위의 그림에서 카메라가 바라보는 방향
(z-축)은 월드 좌표계의 x-축이다. 카메라의 방향(z-축)을 변하지 않는 월드 좌표계의 z-축
과 일치시키도록 하자
<br>
<br>
  다음 그림은 카메라를 y-축을 중심으로 반시계 방향으로 90° 회전하고, 이 회전 변환을 
게임 객체(직육면체)의 위치 (4, 0, 1)에도 적용한 것을 나타내고 있다. 3차원 좌표계(왼손 
좌표계)에서 점 (4, 0, 1)을 y-축을 중심으로 -90° 회전한 결과는 다음과 같이 표현할 수 
있다(sin(-90) = -1, cos(-90) = 0)
<br>
  <img src="https://user-images.githubusercontent.com/36596037/226656806-eced6f64-875b-44e5-bf83-d09edcb16570.png">
<br>
  이제 월드 좌표계의 원점과 카메라 좌표계의 원점이 일치하고 좌표축의 방향도 서로 같
아졌다. 월드 좌표계에서 중심 좌표가 (8, 0, 3)인 게임 객체(직육면체)가 카메라 좌표계로 
변환되었고 그 중심 좌표는 (-1, 0, 4)가 된다는 것을 보여주고 있다. 이때 카메라는 직육
면체의 오른쪽 부분을 보게 되며, 카메라가 보는 이미지는 카메라 변환을 수행하기 전의 
카메라 위치에서 보는 이미지와 완벽히 같게 된다. 그러나 월드 좌표계로 표현된 점이 카
메라 좌표계로 표현되었다.
<br>
<br>
  카메라 변환은 다음과 같이 정리할 수 있다.
  <ol>
    <li>카메라를 월드 좌표계의 원점으로 옮기는 평행이동 변환을 월드 좌표계로 표현된 점들
에 적용한다. 카메라를 월드 좌표계의 원점으로 옮기는 평행이동 변환은 카메라를 카메
라의 위치로 이동한 평행이동 변환의 반대 방향으로 이동하는 것이다(월드 좌표계의 점
에서 카메라의 위치를 빼면 된다).</li>
    <li>카메라 좌표계의 축이 월드 좌표계의 축과 일치하도록 카메라를 회전하는 변환을 월드 
좌표계로 표현된 점들에 적용한다. 카메라 좌표계의 축이 월드 좌표계의 축과 일치하도
록 회전하는 변환은 카메라를 회전한 방향과 반대 방향으로 회전하는 것이다(회전 방향
이 반대인 회전은 회전 각도의 부호를 반대로 회전하는 것이다). </li>
  </ol>
<br>
<br>
다음은 카메라 변환을 하는 함수 CameraTransform()를 구현한 예이다.
<br>
<pre>
<code>
CVertex CameraTransform(CVertex vtxWorld)
{ 
  CVertex vtxCamera, vtxRotated;
  vtxCamera.x = vtxWorld.x - gpCamera->xPosition;
  vtxCamera.y = vtxWorld.y - gpCamera->yPosition;
  vtxCamera.z = vtxWorld.z – gpCamera->zPosition; 
 
  float fPitch = DegreeToRadian(-gpCamera->xRotation);
  float fYaw = DegreeToRadian(-gpCamera->yRotation);
  float fRoll = DegreeToRadian(-gpCamera->zRotation); 
  if (fPitch) {
    vtxRotated.y = vtxCamera.y * cos(fPitch) - vtxCamera.z * sin(fPitch);
    vtxRotated.z = vtxCamera.y * sin(fPitch) + vtxCamera.z * cos(fPitch);
    vtxCamera = vtxRotated;
    }
  if (fYaw) {
    vtxRotated.x = vtxCamera.x * cos(fYaw) + vtxCamera.z * sin(fYaw);
    vtxRotated.z = -vtxCamera.x * sin(fYaw) + vtxCamera.z * cos(fYaw);
    vtxCamera = vtxRotated;
    }
   if (fRoll) {
     vtxRotated.x = vtxCamera.x * cos(fRoll) - vtxCamera.y * sin(fRoll);
    vtxRotated.y = vtxCamera.x * sin(fRoll) + vtxCamera.y * cos(fRoll);
    vtxCamera = vtxRotated;
    }
  return(vtxCamera);
}
</code>
</pre>
<br>

</details>
<br>
 

