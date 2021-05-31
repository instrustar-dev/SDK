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
	//ע�ᴰ����
	static BOOL RegisterWndClass(HINSTANCE hInstance);

	//-------------------------------------------------Line------------------------------------------------
	void CreateLine(lineattribute* line);
	bool HaveLine(std::string name);
	void DestoryLine(std::string name);
	void ChangeDatas(std::string name, double *buf, int length, double timestart, double timeend);
	void DestoryAllLine();
	int GetBufferLength(std::string name);
	//���ý������ź��ƶ��������״
	bool SetCursor(int hCurID=-1, int enhCurID=-1);
    //--------------------------------------------------����------------------------------------------------//
	// ������̬�ؼ�
	bool Create(const RECT & rect, CWnd * pParentWnd, DWORD dwStyle = NULL , UINT nID = 0);
	// ������̬�ؼ�
	bool Create(const int nTop, const int nLeft, const int nRight, const int nBottom, CWnd * pParentWnd, DWORD dwStyled = NULL , UINT nID = 0);
    //-------------------------------------------------���񱳾�------------------------------------------------//
	// ����ɫ������������ɫ
	void SetbgColor(COLORREF clrBg = RGB(0, 0, 80), bool bRepaint = true);
	// ��ȡ����ɫ
	void GetbgColor(COLORREF &clr);
	// ������������
	void SetGrid(BK_GRID bDrawgGrid = GRID, COLORREF clrGrid = RGB(64, 96, 64), int lineStyle = PS_SOLID, int nTransparence = 100, bool bRepaint = true);
	// ��ȡ�������
	void GetGrid(bool &bfShow, COLORREF &clr, int &nStyle, int &nTransparence);
    //---------------------------------------------------Y��----------------------------------------------//
	//����Y���"��λ"��ʾ
	void SetYLeftDanwei(CString Danwei, bool bRepaint = true);
    void SetYRightDanwei(CString Danwei, bool bRepaint = true);
	//����Y���"��λ"��ʾ���������С
	void SetYAxisDanweiFontHeight(int fheight,bool bRepaint = true);
	//����Y���"��λ"��ʾ������
	void SetYAxisDanweiFontStyle(CString style, bool bRepaint = true);
	//����Y���"��λ"��ʾ�Ϳ̶ȵ���ɫ��������ʱ�����ߵ���ɫ���仯��
    void SetYAxisDanwei_TextClr(COLORREF clr, bool bRepaint = true);
	//����Y����ߵķ�Χ
	void SetYLeftRange(double min, double max, bool bRepaint = true);
	void GetYLeftRange(double *min, double *max);
	 //����Y���ұߵķ�Χ
	void SetYRightRange(double min, double max, bool bRepaint = true);
	void GetYRightRange(double *min, double *max);
	//---------------------------------------------------Top----------------------------------------------//
    //����toptitle����ʾ
	void SetTopTitle(CString Title, bool bRepaint = true);
    //����toptitle������Ĵ�С
	void SetTopTitleFontHeight(int fonth,bool bRepaint = true);
	//����Top��ʾ������
	void SetTopTitleFontStyle(CString style, bool bRepaint = true);
    //����Top��ʾ��������ɫ
	void SetTopTitleFontClr(COLORREF clr, bool bRepaint = true);
	//---------------------------------------------------Time----------------------------------------------//
	//�Ƿ�����Time�����Ź��߹���
	void ActiveTimeBar(bool acv, bool bRepaint=true);
    //����time���title
	void SetTimeTitle(CString Title, bool bRepaint = true);
	//����time��title����Ĵ�С
	void SetTimeTitleFontHeight(int fonth, bool bRepaint = true);
	//����time��title����ʾ����ʱ�����ʾ������
	void SetTimeTitleFontStyle(CString style, bool bRepaint = true);
	//����time���"�̶�"��ʾ������
	void SetTimeAxisKeduFontStyle(CString style, bool bRepaint = true);
    //����time���"�̶�"��ʾ�����������С
	void SetTimeAxisKeduFontHeight(int fheight, bool bRepaint = true);
    //����time���Title��ʾ�Ϳ̶ȵ���ɫ
    void SetTimeAxisTitle_TextClr(COLORREF clr, bool bRepaint = true);
    //����time���"�̶���"��ʾ����ɫ
	void SetTimeAxisKeduLineClr(COLORREF clr, bool bRepaint = true);
	//����time��ķ�Χ    һnsΪ��λ����
	virtual void SetTimeAxisRange(double min, double max, bool large, bool bRepaint = true);
	virtual void GetTimeAxisRange(double *min, double *max);
	virtual void MoveTimeAxis(double time);
	//-------------------------------------------------�����ƶ�------------------------------------------------//
	virtual void MoveLeft(int cnt, bool bRepaint = true);
	virtual void MoveRight(int cnt, bool bRepaint = true);
	void MoveVerTop(bool left_right, bool bRepaint = true);
	void MoveVerBottom(bool left_right, bool bRepaint = true);
	//-------------------------------------------------����------------------------------------------------------//
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
	//----------------------------------------------�趨���˵���ʾ�ַ���-------------------------------------------------//
	void SetLeftTopText(CString text, bool bRepaint = true);
    void SetLeftTopText2(CString text, bool bRepaint = true);
	void SetRightTopText(CString text, bool bRepaint = true);
	void SetRightTopText2(CString text, bool bRepaint = true);
	void SetCenterTopText(CString text, bool bRepaint = true);
	void SetCenterTopText2(CString text, bool bRepaint = true);
	void SetRightBottomText(CString text, bool bRepaint = true);
	void SetRightBottomText2(CString text, bool bRepaint = true);
	//����toptitle������Ĵ�С
	void SetTopTextFontHeight(int fonth,bool bRepaint = true);
    //����Top��ʾ��������ɫ
	void SetTopTextFontClr(COLORREF clr, bool bRepaint = true);
	//---------------------------------------------------------�ػ洰��--------------------------------------------------
	inline void Redraw();// �ػ洰��

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
	//��������
	void ComputeRects();  
	void SaveBmp(CBitmap& memBitmap, CDC *pMemDC);
	//
	void SetRange(double *dstmin, double *dstmax, int *dstproperity, int axis_count, double min, double max);

	virtual bool DrawGrid(CDC *pMemDC);
	virtual bool DrawAxis(CDC *pMemDC);
    virtual bool DrawTopAxis(CDC *pMemDC);
	virtual bool DrawTimeAxis(CDC *pMemDC);
	bool DrawTopText(CDC *pMemDC);  //���˵�������ʾ
	bool DrawBottomText(CDC *pMemDC);  //�Ͷ˵�������ʾ
	virtual bool UpdateLine(CDC *pMemDC, CDC *pDC, const CRect &rect);
protected:
	CRITICAL_SECTION g_Time_CS;

	//�ϱߺ��������������״
	HCURSOR m_handCur, m_enhandCur;

	advplotarea m_plotarea;
	advaxis		m_leftaxis;			// left axis
	advaxis		m_rightaxis;		// right axis
	advtopaxis  m_topaxis;
    advtimeaxis m_timeaxis;

	CRect		m_clientRect;

	//���˵��ı���ʾ
	CString lefttoptext,lefttoptext2,righttoptext,righttoptext2,centertoptext,centertoptext2;
	CString rightbottomtext,rightbottomtext2;
    COLORREF textclr;
	char     textheight;

	//
	std::list<lineattribute*> linelist;
};


