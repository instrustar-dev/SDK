#include "StdAfx.h"
#include "lineattribute.h"

lineattribute::lineattribute(void)
: pctName("demo"),
  clr(RGB(0, 255, 0)),
  nLineStyle(PS_SOLID),
  nWidth(1),
  lr_lengend(-1),
  buf(NULL),
  length(0),
  timestart(1),
  timeend(1)
{
}

lineattribute::~lineattribute(void)
{
	if(buf!=NULL)
		delete []buf;
}

//lineattribute& lineattribute::operator = (const lineattribute& copy)
//{
//	pctName=copy.pctName;
//	clr=copy.clr;
//	fTransparence=copy.fTransparence;
//	nLineStyle=copy.nLineStyle;  
//	nWidth=copy.nWidth;
//	lr_lengend=copy.lr_lengend;
//	Visiual=copy.Visiual;
//	return *this;
//}

