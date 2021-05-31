
// DLLTESTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DLLTEST.h"
#include "DLLTESTDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define USBNOTICE_ADD_MSG  (WM_USER+1544)
#define USBNOTICE_REMOVE_MSG  (WM_USER+1545)
#define DATASUPDATE_MSG  (WM_USER+1554)
#define IO_STATE_MSG (WM_USER+1555)

typedef enum {
   errorSample = 0x00000000,
   int8Sample  = 0x00010001,
   uint8Sample  = 0x00010002,
   int16Sample = 0x00020001,
   uint16Sample = 0x00020002,
   int24Sample = 0x00030001,
   int32Sample = 0x0004000A,
   uint32Sample = 0x0004000B,
   floatSample = 0x0004000F,
   doubleSample = 0x0008000F
} sampleFormat;

enum ColorConstants 
	{
	coBlack 		= 0,
	coRed			= 255,
	coGreen 		= 65280,
	coBlue			= 16711680,
	coMagenta		= 16711935,
	coCyan			= 16776960,
	coWhite 		= 16777215,
	coLightGray		= 12632256,
	coDarkGray		= 4210752,
	coGray			= 8421504,
	coPaleYellow	= 13697023,
	coLightYellow	= 8454143,
	coYellow		= 65535,
	coLimeGreen		= 12639424,
	coTeal			= 8421440,
	coDarkGreen		= 32768,
	coMaroon		= 128,
	coPurple		= 8388736,
	coOrange		= 33023,
	coKhaki 		= 7051175,
	coOlive 		= 32896,
	coBrown 		= 4210816,
	coNavy			= 8404992
	};

#ifdef AFX_TARG_CHS
static const char* CHUfA_MODE_NAME[] = 
{
    "自动",
	"连续"
};
#else
static const char* CHUfA_MODE_NAME[] = 
{
    "Auto",
	"Continuous"
};
#endif

#ifdef AFX_TARG_CHS
static const char* CHUfA_STYLE_NAME[] = 
{
    "不触发",
	"上升沿",
	"下降沿",
	"边沿"
};
#else
static const char* CHUfA_STYLE_NAME[] = 
{
    "not trigger",
	"Rising edge",
	"Trailing edge",
	"Edge"
};
#endif

//----------------------------------------------------DDS---------------------------------------------------
static const char* BOXING_STYLE_NAME[] = 
{
	"Sine", 
	"Square", 
	"Triangular", 
	"Up Sawtooth", 
    "Down Sawtooth"
};


#ifdef AFX_TARG_CHS
static char* const CHUfA_SOURCE_NAME[] = 
{
	"左通道",
	"右通道"
};
#else
static char* const CHUfA_SOURCE_NAME[] = 
{
	"CH1",
	"CH2"
};
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

static char* const CH_NAME[] = {"CH1","CH2"};
static COLORREF CLR[]={RGB(255,0,0), RGB(255,255,0)};
static char lr_yaxis[]={-1,1};

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	CMFCLinkCtrl m_url_btn;
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_URL, m_url_btn);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDLLTESTDlg 对话框

CDLLTESTDlg::CDLLTESTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLLTESTDlg::IDD, pParent)
	//, iddes(_T(""))
	, m_trigger_level(0)
	, m_slider_left_move_index(0)
	, m_slider_right_move_index(0)
	, m_wave_duty_cycle(50)
	, ddssupport(false)
	, m_wave_freq(1000)
	, m_capture_length(0)
	, samples(NULL)
	, sample_num(0)
	, m_left_range_min(0)
	, m_left_range_max(0)
	, m_right_range_min(0)
	, m_right_range_max(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//初始化Dll
	InitDll();
	SetDevNoticeCallBack(this, UsbDevNoticeAddCallBack, UsbDevNoticeRemoveCallBack);
	SetDataReadyCallBack(this, DataReadyCallBack);
	SetIOReadStateCallBack(this, IOReadStateCallBack);
}


CDLLTESTDlg::~CDLLTESTDlg(void)
{
	if(samples!=NULL)
	{	
		delete samples;
		samples=NULL;
	}
	//销毁Dll
	FinishDll();
}

void CDLLTESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PLOTCTRL1, m_plot);
	//DDX_Control(pDX, IDC_BTN_PAUSE, m_btn_pause);
	DDX_Control(pDX, IDC_SLIDER_TIME, m_slider_time);
	DDX_Control(pDX, IDC_SLIDER_LEFT_Y, m_slider_left_y);
	DDX_Control(pDX, IDC_SLIDER_RIGHT_Y, m_slider_right_y);
	DDX_Control(pDX, IDC_TRIGGER_MODE, m_trigger_mode);
	DDX_Control(pDX, IDC_TRIGGER_STYLE, m_trigger_style);
	DDX_Control(pDX, IDC_TRIGGER_SOURCE, m_trigger_source);
	DDX_Text(pDX, IDC_TRIGGER_LEVEL, m_trigger_level);
	DDX_Slider(pDX, IDC_SLIDER_LEFT, m_slider_left_move_index);
	DDX_Slider(pDX, IDC_SLIDER_RIGHT, m_slider_right_move_index);
	//DDX_Control(pDX, IDC_BUTTON_LEFT, m_left_channel_ctrl);
	//DDX_Control(pDX, IDC_BUTTON_RIGHT, m_right_channel_ctrl);
	DDX_Text(pDX, IDC_WAVE_DUTY_CYCLE, m_wave_duty_cycle);
	DDV_MinMaxInt(pDX, m_wave_duty_cycle, 1, 99);
	DDX_Control(pDX, IDC_CHECK_WAVE_CTRL, m_output_ctrl);
	DDX_Control(pDX, IDC_COMBO_WAVE_BOXING, m_boxing_style_ctrl);
	DDX_Control(pDX, IDC_EDIT_FREQ, m_wave_freq_ctrl);
	DDX_Control(pDX, IDC_WAVE_DUTY_CYCLE, m_wave_duty_cycle_ctrl);
	DDX_Text(pDX, IDC_EDIT_FREQ, m_wave_freq);
	DDV_MinMaxInt(pDX, m_wave_freq, 1, 2000000);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_capture_length);
	DDX_Control(pDX, IDC_COMBO_SAMPLES, m_samples_combox);
	DDX_Control(pDX, IDC_CHECK_AC_1, m_ch1_ac);
	DDX_Control(pDX, IDC_CHECK_AC_2, m_ch2_ac);
	DDX_Control(pDX, IDC_CHECK_CAPTURE, m_check_capture_btn);
	DDX_Control(pDX, IDC_COMBO_IO_NUM, m_io_num_ctrl);
	DDX_Control(pDX, IDC_COMBO_IO_INOUT, m_io_inout_ctrl);
	DDX_Control(pDX, IDC_CHECK_IO_STATE, m_io_state_ctrl);
	DDX_Control(pDX, IDC_BUTTON_SET, m_io_set_ctrl);
	DDX_Control(pDX, IDC_BUTTON_READ, m_io_set_read);
}

BEGIN_MESSAGE_MAP(CDLLTESTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(USBNOTICE_ADD_MSG, OnUsbNoticeAddMsg)  
	ON_MESSAGE(USBNOTICE_REMOVE_MSG, OnUsbNoticeRemoveMsg)  
	ON_MESSAGE(DATASUPDATE_MSG, OnDataUpdateMsg)  	
	ON_MESSAGE(IO_STATE_MSG, OnIoStateMsg)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_TRIGGER_MODE, &CDLLTESTDlg::OnCbnSelchangeTriggerMode)
	ON_CBN_SELCHANGE(IDC_TRIGGER_STYLE, &CDLLTESTDlg::OnCbnSelchangeTriggerStyle)
	ON_CBN_SELCHANGE(IDC_TRIGGER_SOURCE, &CDLLTESTDlg::OnCbnSelchangeTriggerSource)
	ON_EN_CHANGE(IDC_TRIGGER_LEVEL, &CDLLTESTDlg::OnEnChangeTriggerLevel)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_FREQ, OnEnChangeEditFreq)
	ON_BN_CLICKED(IDC_CHECK_WAVE_CTRL, OnBnClickedCheckWaveCtrl)
	ON_CBN_SELCHANGE(IDC_COMBO_WAVE_BOXING, OnCbnSelchangeComboBoxing)
	ON_EN_CHANGE(IDC_WAVE_DUTY_CYCLE, OnEnChangeWaveDutyCycle)
	ON_CBN_SELCHANGE(IDC_COMBO_SAMPLES, &CDLLTESTDlg::OnCbnSelchangeComboSamples)
	ON_BN_CLICKED(IDC_CHECK_AC_1, &CDLLTESTDlg::OnBnClickedCheckAc1)
	ON_BN_CLICKED(IDC_CHECK_AC_2, &CDLLTESTDlg::OnBnClickedCheckAc2)
	ON_BN_CLICKED(IDC_DEV_RESET_BTN, &CDLLTESTDlg::OnBnClickedDevResetBtn)
	ON_BN_CLICKED(IDC_DLL_RECONNECT_BTN, &CDLLTESTDlg::OnBnClickedDllReconnectBtn)
	ON_BN_CLICKED(IDC_CHECK_CAPTURE, &CDLLTESTDlg::OnBnClickedCheckCapture)
	ON_CBN_SELCHANGE(IDC_COMBO_IO_INOUT, &CDLLTESTDlg::OnCbnSelchangeComboIoInout)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CDLLTESTDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CDLLTESTDlg::OnBnClickedButtonRead)
	ON_CBN_SELCHANGE(IDC_COMBO_IO_NUM, &CDLLTESTDlg::OnCbnSelchangeComboIoNum)
END_MESSAGE_MAP()


// CDLLTESTDlg 消息处理程序

BOOL CDLLTESTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_check_capture_btn.EnableWindow(FALSE);

	//
	m_slider_time.SetRange(1, 21, FALSE);
	m_slider_time.SetPos(5);
	m_slider_time.SetZero(270);
	
	m_slider_time.SetFontName("Tahoma");
	m_slider_time.SetFontItalic();
	m_slider_time.SetFontBold();
	m_slider_time.SetFontSize(14);
	m_slider_time.SetKnobColor(coYellow);
	m_slider_time.SetTextColor(coCyan);
	m_slider_time.SetDialColor(coTeal);

	//
	m_slider_left_y.SetRange(1, 5, FALSE);
	m_slider_left_y.SetPos(4);
	m_slider_left_y.SetZero(270);
	m_slider_left_y.SetFontName("Tahoma");
	m_slider_left_y.SetFontItalic();
	m_slider_left_y.SetFontBold();
	m_slider_left_y.SetFontSize(14);
	m_slider_left_y.SetKnobColor(coYellow);
	m_slider_left_y.SetTextColor(coCyan);
	m_slider_left_y.SetDialColor(coTeal);

	CSliderCtrl *pSlidCtrl=(CSliderCtrl*)GetDlgItem(IDC_SLIDER_LEFT);
	pSlidCtrl->SetRange(-5,5,TRUE);
	m_slider_left_move_index=0;

	m_slider_right_y.SetRange(1, 5, FALSE);
	m_slider_right_y.SetPos(4);
	m_slider_right_y.SetZero(270);
	m_slider_right_y.SetFontName("Tahoma");
	m_slider_right_y.SetFontItalic();
	m_slider_right_y.SetFontBold();
	m_slider_right_y.SetFontSize(14);
	m_slider_right_y.SetKnobColor(coYellow);
	m_slider_right_y.SetTextColor(coCyan);
	m_slider_right_y.SetDialColor(coTeal);

	pSlidCtrl=(CSliderCtrl*)GetDlgItem(IDC_SLIDER_RIGHT);
	pSlidCtrl->SetRange(-5,5,TRUE);
	m_slider_right_move_index=0;

	//////////////////////////DDS?/////////////////////////
	m_output_ctrl.EnableWindow(FALSE);
	m_boxing_style_ctrl.EnableWindow(FALSE);
	m_wave_freq_ctrl.EnableWindow(FALSE);
	m_wave_duty_cycle_ctrl.EnableWindow(FALSE);

	//检测现在已经插入的USB设备
//	DetectUsb();
	//
	m_plot.SubclassDlgItem(IDC_PLOT, this);
	m_plot.SetGrid(GRID,RGB(64,96,64),PS_SOLID);  //设置网格
	m_plot.SetbgColor(RGB(0,0,0));         //设置背景黑色
	//m_plot.SetCursor(IDC_CUROPEN, IDC_CURCLOSE);
	m_plot.SetTopTitle(_T("波形"));
	m_plot.SetYLeftDanwei(_T("100mV/格"));  //设置Y轴的"单位"显示
    m_plot.SetYRightDanwei(_T("100mV/格"));
	m_plot.SetYAxisDanweiFontHeight(20);//设置Y轴字体的最大大小 
	m_plot.SetTimeTitleFontHeight(20);
	m_plot.SetYLeftRange(-5000,5000,false);
	m_plot.SetYRightRange(-5000,5000,false);
	m_plot.SetTimeAxisRange(0,1000000000,false,false); //100ms

	//io
	m_io_num_ctrl.SetCurSel(0);
	m_io_inout_ctrl.SetCurSel(1);
	//m_io_state_ctrl.SetImage(IDB_OFF);
	//m_io_state_ctrl.SetCheckedImage(IDB_ON);
	m_io_set_read.EnableWindow(FALSE);
	//m_io_state_ctrl.EnableWindow(FALSE);

	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDLLTESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDLLTESTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDLLTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//*********************************************USB****************************************
//此处采用发送消息给窗口，这样避免C++的线程，访问窗口信息

void WINAPI CDLLTESTDlg::UsbDevNoticeAddCallBack(void* ppara)
{
	CDLLTESTDlg *pthis=(CDLLTESTDlg*)ppara;
	pthis->SendMessage(USBNOTICE_ADD_MSG, (WPARAM)(1), (LPARAM)(NULL)); 
}

LRESULT CDLLTESTDlg::OnUsbNoticeAddMsg(WPARAM wParam, LPARAM lParam)
{
	TRACE("插入设备 Only Id %08X %08X\n", GetOnlyId0(), GetOnlyId1());

	//////////////////////////////////////////////////////////////////	
	long long int value=m_slider_left_y.GetRealValue();
	m_plot.SetYLeftRange(-value*5,value*5,TRUE);
	LeftDisplayZoomCtrl(true);

	value=m_slider_right_y.GetRealValue();
	m_plot.SetYRightRange(-value*5,value*5,TRUE);
	RightDisplayZoomCtrl(true);

	////////////////////////////////////////////////////////////////
	m_plot.SetTimeAxisRange(0,m_slider_time.GetRealValue()*10,FALSE,TRUE);

	sample_num=GetOscSupportSampleNum();
	if(samples!=NULL)
		delete samples;
	samples=new unsigned int[sample_num];
	GetOscSupportSamples(samples,sample_num);
	m_sample = GetOscSample();

	m_samples_combox.ResetContent();
	CString temp;
	for(int i=0;i<sample_num;i++)
	{
		temp.Format("%d",samples[i]);
		m_samples_combox.AddString(temp);
		if(m_sample==samples[i])
			m_samples_combox.SetCurSel(i);
	}

	m_capture_length=GetMemoryLength();

	//////////////////////////////////////////////////////////////////
	if(IsSupportHardTrigger())
		TRACE("支持硬件触发\n");
	else
		TRACE("不支持硬件触发\n");

	if (IsSupportTriggerForce())
		TRACE("支持强制触发\n");
	else
		TRACE("不支持强制触发\n");

	m_trigger_mode.ResetContent();
	m_trigger_mode.AddString(CHUfA_MODE_NAME[0]);
	m_trigger_mode.AddString(CHUfA_MODE_NAME[1]);
	unsigned int chufa_mode=GetTriggerMode();
	m_trigger_mode.SetCurSel(0);

	m_trigger_style.ResetContent();
	m_trigger_style.AddString(CHUfA_STYLE_NAME[0]);
	m_trigger_style.AddString(CHUfA_STYLE_NAME[1]);
	m_trigger_style.AddString(CHUfA_STYLE_NAME[2]);
	m_trigger_style.AddString(CHUfA_STYLE_NAME[3]);
	unsigned int chufa_style=GetTriggerStyle();
	if(chufa_style==TRIGGER_STYLE_NONE)
		m_trigger_style.SetCurSel(0);
	else if(chufa_style==TRIGGER_STYLE_RISE_EDGE)
		m_trigger_style.SetCurSel(1);
	else if(chufa_style==TRIGGER_STYLE_FALL_EDGE)
		m_trigger_style.SetCurSel(2);
	else if(chufa_style==TRIGGER_STYLE_EDGE)
		m_trigger_style.SetCurSel(3);

	m_trigger_source.ResetContent();
	m_trigger_source.AddString(CHUfA_SOURCE_NAME[0]);
	m_trigger_source.AddString(CHUfA_SOURCE_NAME[1]);
	unsigned int chufa_source=GetTriggerSource();
	m_trigger_source.SetCurSel(chufa_source);

	m_trigger_level=GetTriggerLevel();

	//SetPreTriggerPercent(5);
	//////////////////////////////////////////////////////////////////
	m_ch1_ac.SetCheck(FALSE);
	m_ch2_ac.SetCheck(FALSE);
	if(IsSupportAcDc())
	{
		m_ch1_ac.EnableWindow(TRUE);
		m_ch2_ac.EnableWindow(TRUE);
		TRACE("支持AC/DC\n");
	}
	else
	{
		m_ch1_ac.EnableWindow(FALSE);
		m_ch2_ac.EnableWindow(FALSE);
		TRACE("不支持AC/DC\n");
	}

	////////////////////////////////////////////////////////////////////
#ifdef AFX_TARG_CHS
	CString str("多功能虚拟信号分析仪(简化版)-"); str+="已连接";
#else
	CString str("Multi VirAnalyzer(Simplified Version)-"); str+="Connected";
#endif
	SetWindowText(str);

	///
	m_slider_left_move_index=0;
	m_slider_right_move_index=0;

	m_check_capture_btn.EnableWindow(TRUE);

	/////////////////////DDS///////////////////////////
	ddssupport = IsSupportDDSDevice();
	if(ddssupport)
	{
		int boxing_style_num = GetDDSSupportBoxingStyle(NULL);
		int *style=new int[boxing_style_num];
		GetDDSSupportBoxingStyle(style);

		m_boxing_style_ctrl.ResetContent();
		for(int i=0;i<boxing_style_num;i++)
			m_boxing_style_ctrl.AddString(BOXING_STYLE_NAME[style[i]]);
		m_boxing_style_ctrl.SetCurSel(0);
			
		SetDDSPinlv(m_wave_freq);

		m_output_ctrl.EnableWindow(TRUE);
		m_output_ctrl.SetCheck(FALSE);
		m_boxing_style_ctrl.EnableWindow(TRUE);
		m_wave_freq_ctrl.EnableWindow(TRUE);
		m_wave_duty_cycle_ctrl.EnableWindow(TRUE);

		delete []style;
	}
	else
	{
		m_output_ctrl.EnableWindow(FALSE);
		m_boxing_style_ctrl.EnableWindow(FALSE);
		m_wave_freq_ctrl.EnableWindow(FALSE);
		m_wave_duty_cycle_ctrl.EnableWindow(FALSE);
	}

	//IO
	if (IsSupportIODevice())
	{
		m_io_num_ctrl.EnableWindow(TRUE);
		m_io_inout_ctrl.EnableWindow(TRUE);
		m_io_set_ctrl.EnableWindow(TRUE);
		m_io_set_read.EnableWindow(FALSE);
		m_io_state_ctrl.EnableWindow(TRUE);

		SetIOInOut(m_io_num_ctrl.GetCurSel(), m_io_inout_ctrl.GetCurSel());
	}
	else
	{
		m_io_num_ctrl.EnableWindow(FALSE);
		m_io_inout_ctrl.EnableWindow(FALSE);
		m_io_set_ctrl.EnableWindow(FALSE);
		m_io_set_read.EnableWindow(FALSE);
		m_io_state_ctrl.EnableWindow(FALSE);
	}

	UpdateData(FALSE);
	return 0;
}

void WINAPI CDLLTESTDlg::UsbDevNoticeRemoveCallBack(void* ppara)
{
	CDLLTESTDlg *pthis=(CDLLTESTDlg*)ppara;
	pthis->SendMessage(USBNOTICE_REMOVE_MSG, (WPARAM)(1), (LPARAM)(NULL)); 
}

LRESULT CDLLTESTDlg::OnUsbNoticeRemoveMsg(WPARAM wParam, LPARAM lParam)
{
	TRACE("拔出设备\n");
	//m_left_channel_ctrl.EnableWindow(FALSE);
	//m_right_channel_ctrl.EnableWindow(FALSE);
	m_trigger_mode.ResetContent();
	m_trigger_source.ResetContent();
	m_trigger_style.ResetContent();
	m_check_capture_btn.SetCheck(0);
	m_check_capture_btn.EnableWindow(FALSE);
	/////////////////////DDS///////////////////////////
	/*if(ddssupport)
		DestoryDdsDevice(iddes);
	m_output_ctrl.EnableWindow(FALSE);
	m_boxing_style_ctrl.EnableWindow(FALSE);
	m_wave_freq_ctrl.EnableWindow(FALSE);
	m_wave_duty_cycle_ctrl.EnableWindow(FALSE);*/

#ifdef AFX_TARG_CHS
	SetWindowText("多功能虚拟信号分析仪(简化版)");
#else
	SetWindowText("Multi VirAnalyzer(Simplified Version)");
#endif
	UpdateData(FALSE);
	return 0;
}

//**************************************************数据******************************************************
CString CDLLTESTDlg::SampleStr(double frequence)
{
	CString temp;
#ifdef AFX_TARG_CHS
	if(frequence>1000000)
		temp.Format("频率 %0.3f MHz",frequence/1000000.0);
	else if(frequence>1000)
		temp.Format("频率 %0.3f KHz",frequence/1000.0);
	else
		temp.Format("频率 %0.3f Hz",frequence);
#else
	if(frequence>1000000)
		temp.Format("Freq %0.3f MHz",frequence/1000000.0);
	else if(frequence>1000)
		temp.Format("Freq %0.3f KHz",frequence/1000.0);
	else
		temp.Format("Freq %0.3f Hz",frequence);
#endif
	return temp;
}

CString CDLLTESTDlg::ValueStr(double maxv, double minv)
{
	CString str,temp;

#ifdef AFX_TARG_CHS
	if(maxv>1000)
		temp.Format(_T("最大值 %0.2f V  "), maxv/1000.0);
	else
		temp.Format(_T("最大值 %0.0f mV  "), maxv);
	str+=temp;
	
	if(maxv>1000)
		temp.Format(_T("最小值 %0.2f V  "), minv/1000.0);
	else
		temp.Format(_T("最小值 %0.0f mV  "), minv);
	str+=temp;
	
	if(maxv-minv>1000)
		temp.Format(_T("峰峰值 %0.2f  V "), (maxv-minv)/1000.0);
	else
		temp.Format(_T("峰峰值 %0.0f  mV "), maxv-minv);
	str+=temp;
#else
	if(maxv>1000)
		temp.Format(_T("Vmax %0.2f V  "), maxv/1000.0);
	else
		temp.Format(_T("Vmax %0.0f mV  "), maxv);
	str+=temp;
	
	if(maxv>1000)
		temp.Format(_T("Vmin %0.2f V  "), minv/1000.0);
	else
		temp.Format(_T("Vmin %0.0f mV  "), minv);
	str+=temp;
	
	if(maxv-minv>1000)
		temp.Format(_T("Vpp %0.2f  V "), (maxv-minv)/1000.0);
	else
		temp.Format(_T("Vpp %0.0f  mV "), maxv-minv);
	str+=temp;
#endif	
	return str;
}
//此处采用发送消息给窗口，这样避免C++的线程，访问窗口信息
void WINAPI CDLLTESTDlg::DataReadyCallBack(void* owner)
{
	CDLLTESTDlg *pthis=(CDLLTESTDlg*)owner;
	pthis->SendMessage(DATASUPDATE_MSG, (WPARAM)(0), (LPARAM)(0));
}

LRESULT CDLLTESTDlg::OnDataUpdateMsg(WPARAM wParam, LPARAM lParam)
{
	double* buffer = new double[m_real_length];

	for(int channel=0; channel<2; channel++)
	{
		unsigned int length = ReadVoltageDatas(channel, buffer, m_real_length);
		int outrange = IsVoltageDatasOutRange(channel);
		TRACE("%d is outrange %d\n", channel, outrange);

		bool addline=false;
		if(!m_plot.HaveLine(CH_NAME[channel]))
			addline=true;
		else if(m_plot.GetBufferLength(CH_NAME[channel])!=length)
		{
			m_plot.DestoryLine(CH_NAME[channel]);
			addline=true;
		}

		if(addline)
		{
			lineattribute line;
			line.pctName=CH_NAME[channel];
			line.clr=channel==0? RGB(255,0,0):RGB(255,255,0);
			line.lr_lengend=channel==0? -1:1;
			line.buf=NULL;
			line.length=m_real_length;
			m_plot.CreateLine(&line);
		}

		double timelength=length*1000000000.0/m_sample;  //将采集点数换算成时间ns
		//m_plot.ChangeDatas(CH_NAME[buffer->channel-1], (signed char*)(buffer->buffer+buffer->trigger_pos), buffer->length-buffer->trigger_pos);
		m_plot.ChangeDatas(CH_NAME[channel], buffer, length, 0, timelength);
	}

	delete [] buffer;

	m_plot.Redraw();
	
	NextCapture();

	UpdateData(FALSE);
	return 0;
}

//----------------------------------------------------------------------------------------
void CDLLTESTDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);
	if(pScrollBar->GetDlgCtrlID()==IDC_SLIDER_LEFT_Y)
	{
		long long int value=((CRoundSliderCtrl*)pScrollBar)->GetRealValue();
		m_plot.SetYLeftRange(-value*5+value*m_slider_left_move_index,value*5+value*m_slider_left_move_index,TRUE);
		LeftDisplayZoomCtrl();
	}
	else if(pScrollBar->GetDlgCtrlID()==IDC_SLIDER_RIGHT_Y)
	{
		long long int value=((CRoundSliderCtrl*)pScrollBar)->GetRealValue();
		m_plot.SetYRightRange(-value*5+value*m_slider_right_move_index,value*5+value*m_slider_right_move_index,TRUE);
		RightDisplayZoomCtrl();
	}
	else if(pScrollBar->GetDlgCtrlID()==IDC_SLIDER_TIME)
	{
		m_plot.SetTimeAxisRange(0,((CRoundSliderCtrl*)pScrollBar)->GetRealValue()*10,FALSE,TRUE);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDLLTESTDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);
	if(pScrollBar->GetDlgCtrlID()==IDC_SLIDER_LEFT)
	{
		long long int value=m_slider_left_y.GetRealValue();
		m_plot.SetYLeftRange(-value*5+value*m_slider_left_move_index,value*5+value*m_slider_left_move_index,TRUE);
		LeftDisplayZoomCtrl();
	}
	else if(pScrollBar->GetDlgCtrlID()==IDC_SLIDER_RIGHT)
	{
		long long int value=m_slider_right_y.GetRealValue();
		m_plot.SetYRightRange(-value*5+value*m_slider_right_move_index,value*5+value*m_slider_right_move_index,TRUE);
		RightDisplayZoomCtrl();
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void CDLLTESTDlg::LeftDisplayZoomCtrl(bool start)
{
	double y_min, y_max;
	m_plot.GetYLeftRange(&y_min,&y_max);
	if (start||((y_min != m_left_range_min) || (y_max != m_left_range_max)))
	{
		SetOscChannelRange(0, y_min, y_max);
		m_left_range_min = y_min;
		m_left_range_max = y_max;
	}
	
}

void CDLLTESTDlg::RightDisplayZoomCtrl(bool start)
{
	double y_min, y_max;
	m_plot.GetYRightRange(&y_min,&y_max);
	if (start||((m_right_range_min != y_min) || (m_right_range_max != y_max)))
	{
		SetOscChannelRange(1, y_min, y_max);
		m_right_range_min = y_min;
		m_right_range_max = y_max;
	}
}


void CDLLTESTDlg::OnCbnSelchangeComboSamples()
{
	if(samples!=NULL)
	{
		m_sample=samples[m_samples_combox.GetCurSel()];
		SetOscSample(m_sample); 
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
void CDLLTESTDlg::OnCbnSelchangeTriggerMode()
{
	CString str;
	m_trigger_mode.GetWindowText(str);
	if(str==CHUfA_MODE_NAME[0])
		SetTriggerMode(TRIGGER_MODE_AUTO);
	else if(str==CHUfA_MODE_NAME[1])
		SetTriggerMode(TRIGGER_MODE_LIANXU);	
}

void CDLLTESTDlg::OnCbnSelchangeTriggerStyle()
{
	CString str;
	m_trigger_style.GetWindowText(str);
	if(str==CHUfA_STYLE_NAME[0])
		SetTriggerStyle(TRIGGER_STYLE_NONE);
	else if(str==CHUfA_STYLE_NAME[1])
		SetTriggerStyle(TRIGGER_STYLE_RISE_EDGE);	
	else if(str==CHUfA_STYLE_NAME[2])
		SetTriggerStyle(TRIGGER_STYLE_FALL_EDGE);	
	else if(str==CHUfA_STYLE_NAME[3])
		SetTriggerStyle(TRIGGER_STYLE_EDGE);
}

void CDLLTESTDlg::OnCbnSelchangeTriggerSource()
{
	CString str;
	m_trigger_source.GetWindowText(str);
	if(str==CHUfA_SOURCE_NAME[0])
		SetTriggerSource(0);
	else if(str==CHUfA_SOURCE_NAME[1])
		SetTriggerSource(1);	
}

void CDLLTESTDlg::OnEnChangeTriggerLevel()
{
	UpdateData(TRUE);
	SetTriggerLevel(m_trigger_level);
}

void CDLLTESTDlg::OnBnClickedCheckCapture()
{
	if (m_check_capture_btn.GetCheck())
	{
		UpdateData(TRUE);
		m_real_length = Capture(m_capture_length, 0);
		m_real_length *= 1024;  //转换成长度
	}
}

void CDLLTESTDlg::NextCapture()
{
	if (m_check_capture_btn.GetCheck())
	{
		LeftDisplayZoomCtrl();
		RightDisplayZoomCtrl();

		m_real_length = Capture(m_capture_length, 0);
		m_real_length *= 1024;  //转换成长度
	}
}

void CDLLTESTDlg::OnBnClickedCheckAc1()
{
	SetAcDc(0,m_ch1_ac.GetCheck());
}


void CDLLTESTDlg::OnBnClickedCheckAc2()
{
	SetAcDc(1,m_ch2_ac.GetCheck());
}


//*******************************************************DDS************************************************
void CDLLTESTDlg::OnCbnSelchangeComboBoxing()
{
	if(ddssupport)
	{
		CString BoxingName;
		m_boxing_style_ctrl.GetWindowText(BoxingName);

		if(BoxingName==BOXING_STYLE_NAME[0])
			SetDDSBoxingStyle(BX_SINE);
		else if(BoxingName==BOXING_STYLE_NAME[1])
			SetDDSBoxingStyle(BX_SQUARE);
		else if(BoxingName==BOXING_STYLE_NAME[2])
			SetDDSBoxingStyle(BX_TRIANGULAR);
		else if(BoxingName==BOXING_STYLE_NAME[3])
			SetDDSBoxingStyle(BX_UP_SAWTOOTH);
		else if(BoxingName==BOXING_STYLE_NAME[4])
			SetDDSBoxingStyle(BX_DOWN_SAWTOOTH);
	}
}

void CDLLTESTDlg::OnEnChangeEditFreq()
{
	UpdateData(TRUE);
	if(ddssupport)
	{
		if((m_wave_freq<=0)||(m_wave_freq>2000000))  //暂时最大支持2M
		{
			CString str;
#ifdef AFX_TARG_CHS
			str.Format("频率必须在[0,%d]之间!",2000000);
			AfxMessageBox(str);
#else
			str.Format("Frequency Must Between the [0,%d]!",2000000);
			AfxMessageBox(str);
#endif
			m_wave_freq=2000000;
			UpdateData(FALSE);
		}
		SetDDSPinlv(m_wave_freq);
	}
}

void CDLLTESTDlg::OnEnChangeWaveDutyCycle()
{
	UpdateData(TRUE);
	if(ddssupport)
		SetDDSDutyCycle(m_wave_duty_cycle);
}

void CDLLTESTDlg::OnBnClickedCheckWaveCtrl()
{
	if(ddssupport)
		DDSOutputEnable(!IsDDSOutputEnable());
}


//-------------------------------------------------------------------------
void CDLLTESTDlg::OnBnClickedDevResetBtn()
{
	ResetDevice();
}


void CDLLTESTDlg::OnBnClickedDllReconnectBtn()
{
	//销毁Dll
	FinishDll();

	Sleep(1000);

	//初始化Dll
	InitDll();
	SetDevNoticeCallBack(this, UsbDevNoticeAddCallBack, UsbDevNoticeRemoveCallBack);
	SetDataReadyCallBack(this, DataReadyCallBack);
}

void CDLLTESTDlg::OnCbnSelchangeComboIoNum()
{
	SetIOInOut(m_io_num_ctrl.GetCurSel(), m_io_inout_ctrl.GetCurSel());
}


void CDLLTESTDlg::OnCbnSelchangeComboIoInout()
{
	if (m_io_inout_ctrl.GetCurSel() == 1)
	{
		m_io_set_ctrl.EnableWindow(TRUE);
		m_io_set_read.EnableWindow(FALSE);
		m_io_state_ctrl.EnableWindow(TRUE);
	}
	else //输入
	{
		m_io_set_ctrl.EnableWindow(FALSE);
		m_io_set_read.EnableWindow(TRUE);
		m_io_state_ctrl.EnableWindow(FALSE);
	}
	SetIOInOut(m_io_num_ctrl.GetCurSel(), m_io_inout_ctrl.GetCurSel());
}


void CDLLTESTDlg::OnBnClickedButtonSet()
{
	SetIOState(m_io_num_ctrl.GetCurSel(), m_io_state_ctrl.GetCheck());
}


void CDLLTESTDlg::OnBnClickedButtonRead()
{
	ReadIOState(m_io_num_ctrl.GetCurSel());
}

void CDLLTESTDlg::IOReadStateCallBack(void* ppara, unsigned char channel, char state)
{
	TRACE("IOReadStateCallBack %d %d\n", channel, state);
	CDLLTESTDlg* pthis = (CDLLTESTDlg*)ppara;
	pthis->SendMessage(IO_STATE_MSG, (WPARAM)(channel), (LPARAM)(state));
}

LRESULT CDLLTESTDlg::OnIoStateMsg(WPARAM wParam, LPARAM lParam)
{
	m_io_state_ctrl.SetCheck(char(lParam));
	UpdateData(FALSE);
	return 0;
}