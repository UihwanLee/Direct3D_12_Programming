#pragma once

/* 
	define.h�� �ڵ带 �� �� ���� ���ϰ� �����ϰ� �����ϱ� ���� ��������̴�.
	���� �ݺ������� ���̴� �Լ��� �������� �����Ѵ�.	
*/

// Singleton ����
#define SINGLETON(_type) public:\
						static _type* GetInst()\
						{\
							static _type mgr;\
							return &mgr;\
						}\
						private:\
							_type();\
							~_type();

// 

#define FRAMEBUFFER_WIDTH		640
#define FRAMEBUFFER_HEIGHT		480

#define RANDOM_COLOR			(0xFF000000 | ((rand() * 0xFFFFFF) / RAND_MAX))

#define DegreeToRadian(x)		float((x)*3.141592654f/180.0f)

#define EPSILON					1.0e-2f