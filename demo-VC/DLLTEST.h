
// DLLTEST.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDLLTESTApp:
// �йش����ʵ�֣������ DLLTEST.cpp
//

class CDLLTESTApp : public CWinApp
{
public:
	CDLLTESTApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDLLTESTApp theApp;