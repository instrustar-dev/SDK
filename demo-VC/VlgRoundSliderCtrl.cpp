#include "StdAfx.h"
#include "VlgRoundSliderCtrl.h"


CVlgRoundSliderCtrl::CVlgRoundSliderCtrl(void)
{
}


CVlgRoundSliderCtrl::~CVlgRoundSliderCtrl(void)
{
}

CString CVlgRoundSliderCtrl::OnFormatText()
{
	CString strFormattedText;
	int pos=GetPos();

	if(pos==1)
		strFormattedText.Format("%d mV", MvAxis[pos-1]);
	else if(pos==2)
		strFormattedText.Format("%d mV", MvAxis[pos-1]);
	else if(pos==3)
		strFormattedText.Format("%d mV", MvAxis[pos-1]);
	else if(pos==4)
		strFormattedText.Format("%d V", MvAxis[pos-1]/1000);
	else if(pos==5)
		strFormattedText.Format("%d V", MvAxis[pos-1]/1000);
	else if(pos==6)
		strFormattedText.Format("%d V", MvAxis[pos-1]/1000);

	return strFormattedText;
}
