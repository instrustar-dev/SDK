#pragma once
#include "AdvPlotBasic.h"
#include <math.h>
#include <assert.h>
#include <list>
#include "lineattribute.h"
// AdvPlot

class AdvPlot : public CWnd
{
	DECLARE_DYNAMIC(AdvPlot)

public:
	AdvPlot();
	virtual ~AdvPlot();
	//注册窗口类
	static BOOL RegisterWndClass(HINSTANCE hInstance);

	//-------------------------------------------------Line------------------------------------------------
	void CreateLine(lineattribute* line);
	bool HaveLine(std::string name);
	void DestoryLine(std::string name);
	void ChangeDatas(std::string name, double *buf, int length, double timestart, double timeend);
	void DestoryAllLine();
	int GetBufferLength(std::string name);
	//设置进入缩放和移动区鼠标形状
	bool SetCursor(int hCurID=-1, int enhCurID=-1);
    //--------------------------------------------------创建------------------------------------------------//
	// 创建静态控件
	bool Create(const RECT & rect, CWnd * pParentWnd, DWORD dwStyle = NULL , UINT nID = 0);
	// 创建静态控件
	bool Create(const int nTop, const int nLeft, const int nRight, const int nBottom, CWnd * pParentWnd, DWORD dwStyled = NULL , UINT nID = 0);
    //-------------------------------------------------网格背景------------------------------------------------//
	// 背景色包括背景网格色
	void SetbgColor(COLORREF clrBg = RGB(0, 0, 80), bool bRepaint = true);
	// 获取背景色
	void GetbgColor(COLORREF &clr);
	// 背景网格设置
	void SetGrid(BK_GRID bDrawgGrid = GRID, COLORREF clrGrid = RGB(64, 96, 64), int lineStyle = PS_SOLID, int nTransparence = 100, bool bRepaint = true);
	// 获取网格参数
	void GetGrid(bool &bfShow, COLORREF &clr, int &nStyle, int &nTransparence);
    //---------------------------------------------------Y轴----------------------------------------------//
	//设置Y轴的"单位"显示
	void SetYLeftDanwei(CString Danwei, bool bRepaint = true);
    void SetYRightDanwei(CString Danwei, bool bRepaint = true);
	//设置Y轴的"单位"显示字体的最大大小
	void SetYAxisDanweiFontHeight(int fheight,bool bRepaint = true);
	//设置Y轴的"单位"显示的字体
	void SetYAxisDanweiFontStyle(CString style, bool bRepaint = true);
	//设置Y轴的"单位"显示和刻度的颜色（当有线时将随线的颜色而变化）
    void SetYAxisDanwei_TextClr(COLORREF clr, bool bRepaint = true);
	//设置Y轴左边的范围
	void SetYLeftRange(double min, double max, bool bRepaint = true);
	void GetYLeftRange(double *min, double *max);
	 //设置Y轴右边的范围
	void SetYRightRange(double min, double max, bool bRepaint = true);
	void GetYRightRange(double *min, double *max);
	//---------------------------------------------------Top----------------------------------------------//
    //更改toptitle的显示
	void SetTopTitle(CString Title, bool bRepaint = true);
    //设置toptitle轴字体的大小
	void SetTopTitleFontHeight(int fonth,bool bRepaint = true);
	//设置Top显示的字体
	void SetTopTitleFontStyle(CString style, bool bRepaint = true);
    //设置Top显示的字体颜色
	void SetTopTitleFontClr(COLORREF clr, bool bRepaint = true);
	//---------------------------------------------------Time----------------------------------------------//
	//是否启动Time轴缩放工具功能
	void ActiveTimeBar(bool acv, bool bRepaint=true);
    //设置time轴的title
	void SetTimeTitle(CString Title, bool bRepaint = true);
	//设置time轴title字体的大小
	void SetTimeTitleFontHeight(int fonth, bool bRepaint = true);
	//设置time轴title和显示计数时间的显示的字体
	void SetTimeTitleFontStyle(CString style, bool bRepaint = true);
	//设置time轴的"刻度"显示的字体
	void SetTimeAxisKeduFontStyle(CString style, bool bRepaint = true);
    //设置time轴的"刻度"显示的字体的最大大小
	void SetTimeAxisKeduFontHeight(int fheight, bool bRepaint = true);
    //设置time轴的Title显示和刻度的颜色
    void SetTimeAxisTitle_TextClr(COLORREF clr, bool bRepaint = true);
    //设置time轴的"刻度线"显示的颜色
	void SetTimeAxisKeduLineClr(COLORREF clr, bool bRepaint = true);
	//设置time轴的范围    一ns为单位计数
	virtual void SetTimeAxisRange(double min, double max, bool large, bool bRepaint = true);
	virtual void GetTimeAxisRange(double *min, double *max);
	virtual void MoveTimeAxis(double time);
	//-------------------------------------------------控制移动------------------------------------------------//
	virtual void MoveLeft(int cnt, bool bRepaint = true);
	virtual void MoveRight(int cnt, bool bRepaint = true);
	void MoveVerTop(bool left_right, bool bRepaint = true);
	void MoveVerBottom(bool left_right, bool bRepaint = true);
	//-------------------------------------------------缩放------------------------------------------------------//
	virtual void ZoomIn(bool bRepaint = true);
	virtual void ZoomInPos(bool bRepaint = true);
	virtual void ZoomInCenter(bool bRepaint = true);
	virtual void ZoomOut(bool bRepaint = true);
	virtual void ZoomOutPos(bool bRepaint = true);
	virtual void ZoomOutCenter(bool bRepaint = true);
	virtual void ZoomVerIn(bool left_right,bool bRepaint = true);
	virtual void ZoomVerInCenter(bool left_right,bool bRepaint = true);
	virtual void ZoomVerOut(bool left_right,bool bRepaint = true);
	virtual void ZoomVerOutCenter(bool left_right,bool bRepaint = true);
	//----------------------------------------------设定顶端的显示字符串-------------------------------------------------//
	void SetLeftTopText(CString text, bool bRepaint = true);
    void SetLeftTopText2(CString text, bool bRepaint = true);
	void SetRightTopText(CString text, bool bRepaint = true);
	void SetRightTopText2(CString text, bool bRepaint = true);
	void SetCenterTopText(CString text, bool bRepaint = true);
	void SetCenterTopText2(CString text, bool bRepaint = true);
	void SetRightBottomText(CString text, bool bRepaint = true);
	void SetRightBottomText2(CString text, bool bRepaint = true);
	//设置toptitle轴字体的大小
	void SetTopTextFontHeight(int fonth,bool bRepaint = true);
    //设置Top显示的字体颜色
	void SetTopTextFontClr(COLORREF clr, bool bRepaint = true);
	//---------------------------------------------------------重绘窗口--------------------------------------------------
	inline void Redraw();// 重绘窗口

protected:
	DECLARE_MESSAGE_MAP()
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
private:
	//分配区域
	void ComputeRects();  
	void SaveBmp(CBitmap& memBitmap, CDC *pMemDC);
	//
	void SetRange(double *dstmin, double *dstmax, int *dstproperity, int axis_count, double min, double max);

	virtual bool DrawGrid(CDC *pMemDC);
	virtual bool DrawAxis(CDC *pMemDC);
    virtual bool DrawTopAxis(CDC *pMemDC);
	virtual bool DrawTimeAxis(CDC *pMemDC);
	bool DrawTopText(CDC *pMemDC);  //顶端的文字显示
	bool DrawBottomText(CDC *pMemDC);  //低端的文字显示
	virtual bool UpdateLine(CDC *pMemDC, CDC *pDC, const CRect &rect);
protected:
	CRITICAL_SECTION g_Time_CS;

	//上边和左右两边鼠标形状
	HCURSOR m_handCur, m_enhandCur;

	advplotarea m_plotarea;
	advaxis		m_leftaxis;			// left axis
	advaxis		m_rightaxis;		// right axis
	advtopaxis  m_topaxis;
    advtimeaxis m_timeaxis;

	CRect		m_clientRect;

	//顶端的文本显示
	CString lefttoptext,lefttoptext2,righttoptext,righttoptext2,centertoptext,centertoptext2;
	CString rightbottomtext,rightbottomtext2;
    COLORREF textclr;
	char     textheight;

	//
	std::list<lineattribute*> linelist;
};


