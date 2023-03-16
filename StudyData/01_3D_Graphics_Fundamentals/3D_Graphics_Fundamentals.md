> ### 3D 그래픽 기초(3D Graphics Fundamentals)

```
  3D 그래픽에 대한 개념을 설명한다.
  2D 게임과 3D 게임의 차이에 알아보고 Direct3D 게임 프로그래밍은 어떻게 이루어지는지
  알아볼 수 있도록 한다.
```

<br>


<details>
  <summary><span style="border-bottom:0.05em solid"><strong>① 컴퓨터 구조(Computer Architecture)</strong></span></summary>
<hr>
     최신 컴퓨터는 3D 그래픽 처리를 할 수 있는 강력한 그래픽 카드(Graphic card, 비디
오 카드(Video card))를 가지고 있다. 우리는 그래픽 카드의 기능을 사용한 3D 게임 프로
그램을 작성하기 위하여 Direct3D를 이해하고 활용하는 것을 배운다. 윈도우 10 운영체제
에서 Direct3D 12 API를 기반으로 하며 프로그래밍 언어는 C++을 사용한다.
<hr>

</details>

<details>
  <summary><span style="border-bottom:0.05em solid"><strong>② 게임 프로그래밍(Game programming)</strong></span></summary>
     실세계의 일반적인 문제를 컴퓨터 프로그램으로 해결할 때의 과정은 문제에 대한 이해와 
분석, 문제의 표현, 알고리즘 작성, 프로그래밍 언어를 사용하여 구현하는 것이다. 게임을 
컴퓨터 프로그램으로 개발하는 과정도 유사하게 게임의 설계, 게임의 표현, 알고리즘 작성, 프로그래밍 언어를 사용하여 구현하는 것이다. 컴퓨터 프로그램의 작성에서 중요한 것은 
문제의 표현, 알고리즘 작성, 구현 과정이 모두 수학적인 방법(궁극적으로 숫자로)으로 이
루어져야 한다는 것이다.
<hr>

</details>

<details>
  <summary><span style="border-bottom:0.05em solid"><strong>③ 2D 게임과 3D 게임의 구분</strong></span></summary>
<hr>
  <ul>
     <li>표현의 차이가 있다.</li>
     <li>3D 게임의 오브젝트는 (x, y, z)좌표를 가진다.</li>
     <li>3D 게임의 렌더링은 2D좌표 공간으로 표현하는 것이다.</li>
  </ul>
<hr>
</details>

<details>
  <summary><span style="border-bottom:0.05em solid"><strong>④ Direct3D 게임 프로그래밍</strong></span></summary>
<hr>
     Direct3D 게임 프로그래밍은 2학기 과정으로 구성된다. 1학기 과정에서는 다음과 같은 
기초적인 내용을 다룬다. 2학기 과정에서는 Direct3D의 고급 내용과 애니메이션 등을 다
룬다.
  <hr>
  <ul>
    <li> 3D 그래픽 파이프라인(Graphics Pipeline)</li>
    <li> 3D 그래픽을 위한 수학</li>
    <li> 게임 프레임워크(Game Framework)</li>
    <li> Direct3D 파이프라인(Pipeline)</li>
    <li> 쉐이더 프로그래밍(Shader Programming)</li>
  </ul>
<hr>

<details>
  <summary><span style="border-bottom:0.05em solid"><strong>④ Direct3D 게임 프로그래밍</strong></span></summary>
<hr>
     3D 게임을 개발하기 위하여 다음과 같은 처리를 하는 프로그램을 작성해야 한다.
  <hr>
  <ul>
    <li> 사용자 입력(User Input)</li>
    <li> 자원 관리(Resource Management)</li>
    <li> 그래픽 로딩과 렌더링(Loading and Rendering Graphics)</li>
    <li> 음향 처리(Playing Sound Effects)</li>
    <li> 인공지능(Artificial Intelligence)</li>
  </ul>
  <hr>
  일반적으로 3D 그래픽 처리를 담당하는 프로그램의 요소를 렌더링 엔진(Rendering Engine)이라고 한다. 렌더링 엔진은 여러 가지 게임 엔진의 핵심 모듈(Module)이며 렌더러(Renderer)라고 부르기도 한다. 렌더러는 가상적인 게임 세계의 3차원 표현을 모니터 화면(스크린, Screen)에 
2차원 영상으로 그려내는 역할을 한다. 이러한 처리를 하기 위하여 수학적인 3차원 표현과 
수학적인 처리 방법이 필요하다. 렌더링(Rendering)이란 수학적으로 표현된 3차원 세상을 
수학적인 처리 방법을 통하여 2차원 영상(Image)으로 그리는 것을 말한다.


