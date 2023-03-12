#pragma once

/* 
	define.h는 코드를 좀 더 보기 편하게 간략하게 조정하기 위한 헤더파일이다.
	또한 반복적으로 쓰이는 함수나 문법들을 정의한다.	
*/

// Singleton 정의
#define SINGLETON(type) private:\
static type* thisP;\
type();\
~type();\
public:\
	static type* GetInst()\
	{\
		if (thisP == nullptr) { thisP = new type(); }\
		return thisP;\
	}


#define FRAMEBUFFER_WIDTH		640
#define FRAMEBUFFER_HEIGHT		480

#define RANDOM_COLOR			(0xFF000000 | ((rand() * 0xFFFFFF) / RAND_MAX))

#define DegreeToRadian(x)		float((x)*3.141592654f/180.0f)

#define EPSILON					1.0e-2f