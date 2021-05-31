// RoundSliderCtrl.h : Header file
//
// Copyright (c) 1999 Daniel Frey
// See RoundSliderCtrl.cpp for details.

#pragma once

class CRoundSliderCtrl : public CSliderCtrl
{
protected:
	CPoint	m_ptCenter;
	int		m_nRadius;
	CRgn	m_rgn;
                  
	// Color and font stuff from CLabel static control class
	LOGFONT  m_lf;
	CFont	 m_font;
	COLORREF m_crText;
	HBRUSH	 m_hKnobBrush;
	HBRUSH	 m_hDialBrush;
	HBRUSH	 m_hwndBrush;

	int		m_nZero;
	bool	m_bInverted;

	CString m_strText;
	int		m_nKnobRadius;    //指示器半径
    //HCURSOR     m_hcCross;    //鼠标形状
	bool	m_bDragging;     //鼠标左键是否已经按下
	bool	m_bDragChanged;  //指示器是否改变标志
    bool	m_bDrawRadioButton; 

public:
	DECLARE_DYNAMIC(CRoundSliderCtrl)

	CRoundSliderCtrl();
	virtual ~CRoundSliderCtrl();

	virtual long long int GetRealValue(){return GetPos();};

	// You may use '%ld' to show the slider value. Default: "%ld?"
	virtual void SetText(const CString& strNewText);
	virtual CString GetText() const;

	// Override this function for customized output
	virtual CString OnFormatText();

	// Default: 7, Minimum: 5
	virtual void SetKnobRadius(const int nNewKnobRadius);
	virtual int GetKnobRadius() const;

	// 0-359, 0 = Top, 90 = Right, 180 = Bottom, 270 = Left
	virtual void SetZero(const int nZero);
	virtual int GetZero() const;

	virtual void SetInverted(const bool bNewInverted = true);
	virtual bool GetInverted();
    void SetDrawRadioButton(const bool nDrawRadioButton);    //选择按钮的形式

	// Font stuff ripped off from CLabel class
	void ReconstructFont();
	CRoundSliderCtrl& SetFontBold(BOOL bBold = TRUE);
	CRoundSliderCtrl& SetFontUnderline(BOOL bSet= TRUE);
	CRoundSliderCtrl& SetFontItalic(BOOL bSet = TRUE);
	CRoundSliderCtrl& SetFontSize(int nSize);
	CRoundSliderCtrl& SetFontName(const CString& strFont);
	CRoundSliderCtrl& SetTextColor(COLORREF crText);
	CRoundSliderCtrl& SetKnobColor(COLORREF crBkgnd);
	CRoundSliderCtrl& SetDialColor(COLORREF crBkgnd);


	//{{AFX_VIRTUAL(CRoundSliderCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	virtual void Init();
	virtual bool SetKnob(const CPoint& pt);
	virtual void PostMessageToParent(const int nTBCode) const;

	//{{AFX_MSG(CRoundSliderCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

#ifdef _DEBUG
public:
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif // _DEBUG
	
};