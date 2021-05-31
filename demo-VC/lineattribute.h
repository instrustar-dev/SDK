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

	// 线名
	std::string pctName;
	// 线色
	COLORREF clr;
	// 线风格
	int nLineStyle;  // 实线、虚线、点线、点划线、点点划线（SOLID, DASH, DOT, DASHDOT, DASHDOTDOT）
	// 线宽
	int nWidth;
	//线使用那边的坐标左边或右边的   小于0代表左边  等于0代表不画    大于0代表右边
	char  lr_lengend;
	//
	double *buf;
	int length;
	//时间
	double timestart;
	double timeend;
private:
	lineattribute& operator=(const lineattribute& copy){};
};

