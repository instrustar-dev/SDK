#pragma once
//#include "plotpoints.h"
//#include "points.h"
#include <string>
#include <windows.h>

class lineattribute
{
public:
	lineattribute(void);
	virtual ~lineattribute(void);

	// ����
	std::string pctName;
	// ��ɫ
	COLORREF clr;
	// �߷��
	int nLineStyle;  // ʵ�ߡ����ߡ����ߡ��㻮�ߡ���㻮�ߣ�SOLID, DASH, DOT, DASHDOT, DASHDOTDOT��
	// �߿�
	int nWidth;
	//��ʹ���Ǳߵ�������߻��ұߵ�   С��0�������  ����0������    ����0�����ұ�
	char  lr_lengend;
	//
	double *buf;
	int length;
	//ʱ��
	double timestart;
	double timeend;
private:
	lineattribute& operator=(const lineattribute& copy){};
};

