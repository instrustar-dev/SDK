#include "StdAfx.h"
#include "SampleRoundSliderCtrl.h"


CSampleRoundSliderCtrl::CSampleRoundSliderCtrl(void)
{
}


CSampleRoundSliderCtrl::~CSampleRoundSliderCtrl(void)
{
}

CString CSampleRoundSliderCtrl::OnFormatText()
{
	CString strFormattedText;
	strFormattedText.Format("%d", SampleValue[GetPos()-1]);
	return strFormattedText;
}