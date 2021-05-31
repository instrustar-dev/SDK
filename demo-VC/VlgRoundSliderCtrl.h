#pragma once
#include "roundsliderctrl.h"

static long long int MvAxis[]={100,200,500,1000,2000,5000};

class CVlgRoundSliderCtrl :
	public CRoundSliderCtrl
{
public:
	CVlgRoundSliderCtrl(void);
	virtual ~CVlgRoundSliderCtrl(void);

	virtual CString OnFormatText();

	virtual long long int GetRealValue(){return MvAxis[GetPos()-1];};
};

