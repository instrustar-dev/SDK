#pragma once
#include "roundsliderctrl.h"

static long long int SampleValue[]={23809,71428,111111,230764,428571};

class CSampleRoundSliderCtrl :
	public CRoundSliderCtrl
{
public:
	CSampleRoundSliderCtrl(void);
	virtual ~CSampleRoundSliderCtrl(void);

	virtual CString OnFormatText();

	virtual long long int GetRealValue(){return SampleValue[GetPos()-1];};
};

