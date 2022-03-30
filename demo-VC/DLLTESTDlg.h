
// DLLTESTDlg.h : 头文件
//

#pragma once
#include "VdsoLib.h"
#include "afxwin.h"
#include "AdvPlot.h"
#include "afxcmn.h"
#include "VlgRoundSliderCtrl.h"
#include "TimeRoundSliderCtrl.h"

// CDLLTESTDlg 对话框
class CDLLTESTDlg : public CDialogEx
{
// 构造
public:
	CDLLTESTDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CDLLTESTDlg(void);

	static void WINAPI UsbDevNoticeAddCallBack(void* ppara);
	static void WINAPI UsbDevNoticeRemoveCallBack(void* ppara);
	static void WINAPI DataReadyCallBack(void* ppara);
	static void WINAPI IOReadStateCallBack(void* ppara, unsigned char channel, char state);

// 对话框数据
	enum { IDD = IDD_DLLTEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	LRESULT OnUsbNoticeAddMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnUsbNoticeRemoveMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnDataUpdateMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnIoStateMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangeTriggerMode();
	afx_msg void OnCbnSelchangeTriggerStyle();
	afx_msg void OnCbnSelchangeTriggerSource();
	afx_msg void OnEnChangeTriggerLevel();

	afx_msg void OnEnChangeEditFreq();
	afx_msg void OnBnClickedCheckWaveCtrl();
	afx_msg void OnCbnSelchangeComboBoxing();
	afx_msg void OnEnChangeWaveDutyCycle();
private:
	void NextCapture();
	void LeftDisplayZoomCtrl(bool start = false);
	void RightDisplayZoomCtrl(bool start = false);
	unsigned int DisplayZoomCtrl(double y_min, double y_max);
	
	CString SampleStr(double frequence);
	CString ValueStr(double maxv, double minv);

	//CString iddes;
	AdvPlot m_plot;
	//CButton m_btn_pause;

	CTimeRoundSliderCtrl m_slider_time;
	CVlgRoundSliderCtrl m_slider_left_y;
	CVlgRoundSliderCtrl m_slider_right_y;
	int m_slider_left_move_index;
	int m_slider_right_move_index;
	double m_left_range_min, m_left_range_max;
	double m_right_range_min, m_right_range_max;

	unsigned int* samples;
	int sample_num;
	CComboBox m_samples_combox;
	unsigned int m_sample;

	CComboBox m_trigger_mode;
	CComboBox m_trigger_source;
	CComboBox m_trigger_style;	
	int m_trigger_level;

	int m_capture_length;
	unsigned int m_real_length;
	CButton m_check_capture_btn;
	//CButton m_capture_btn;
	//CButton m_left_channel_ctrl;
	//CButton m_right_channel_ctrl;

	//
	bool ddssupport;
	int m_wave_duty_cycle;
	CButton m_output_ctrl;
	CComboBox m_boxing_style_ctrl;
	CEdit m_wave_freq_ctrl;
	int m_wave_freq;
	CEdit m_wave_duty_cycle_ctrl;

	CButton m_ch1_ac;
	CButton m_ch2_ac;

	CComboBox m_io_num_ctrl;
	CComboBox m_io_inout_ctrl;
	CButton m_io_set_ctrl;
	CButton m_io_set_read;
	CButton m_io_state_ctrl;
public:
	afx_msg void OnCbnSelchangeComboSamples();
	
	afx_msg void OnBnClickedCheckAc1();
	afx_msg void OnBnClickedCheckAc2();
	afx_msg void OnBnClickedDevResetBtn();
	afx_msg void OnBnClickedDllReconnectBtn();
	afx_msg void OnBnClickedCheckCapture();
	afx_msg void OnCbnSelchangeComboIoInout();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnCbnSelchangeComboIoNum();
};
