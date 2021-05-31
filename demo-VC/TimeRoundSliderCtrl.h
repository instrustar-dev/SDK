#pragma once
#include "roundsliderctrl.h"

static long long int TimeAxis[]={100,200,500, 
					   1000,2000,5000,10000,20000,50000,100000,200000,500000, 
					   1000000,2000000,5000000,10000000,20000000,50000000,100000000,200000000,500000000, 
					   1000000000,2000000000,5000000000};
					   
class CTimeRoundSliderCtrl :
	public CRoundSliderCtrl
{
public:
	CTimeRoundSliderCtrl(void);
	virtual ~CTimeRoundSliderCtrl(void);

	virtual CString OnFormatText();

	virtual long long int GetRealValue(){return TimeAxis[GetPos()-1];};  //ns
};

