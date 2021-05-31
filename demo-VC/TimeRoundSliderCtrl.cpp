#include "StdAfx.h"
#include "TimeRoundSliderCtrl.h"


CTimeRoundSliderCtrl::CTimeRoundSliderCtrl(void)
{
}


CTimeRoundSliderCtrl::~CTimeRoundSliderCtrl(void)
{
}


CString CTimeRoundSliderCtrl::OnFormatText()
{
	CString strFormattedText;
	int pos=GetPos();

	if((pos>=1)&&(pos<=3))
		strFormattedText.Format("%d ns", TimeAxis[pos-1]);
	else if((pos>=4)&&(pos<=12))
		strFormattedText.Format("%d us", TimeAxis[pos-1]/1000);
	else if((pos>=13)&&(pos<=21))
		strFormattedText.Format("%d ms", TimeAxis[pos-1]/1000000);
	else 
		strFormattedText.Format("%d s", TimeAxis[pos-1]/1000000000);

	return strFormattedText;
}
