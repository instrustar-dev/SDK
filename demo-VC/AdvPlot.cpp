// AdvPlot.cpp : implementation file
//
#include "stdafx.h"
#include "AdvPlot.h"
#include "lineattribute.h"
// AdvPlot
#define DRAG_COUNT 10
#define TRACK_COUNT 5

#define Y_AXIS_PROPERITY_MIN 15  //0.00001
#define Y_AXIS_PROPERITY_MAX 57  //39  //100

IMPLEMENT_DYNAMIC(AdvPlot, CWnd)

AdvPlot::AdvPlot()
: m_handCur(NULL)
, m_enhandCur(NULL)
, lefttoptext(_T(""))
, righttoptext(_T(""))
, lefttoptext2(_T(""))
, righttoptext2(_T(""))
, rightbottomtext(_T(""))
, rightbottomtext2(_T(""))
, centertoptext(_T(""))
, centertoptext2(_T(""))
, textclr(RGB(255,255,0))
, textheight(15)
{
	AdvPlot::RegisterWndClass(AfxGetInstanceHandle());
	InitializeCriticalSection(&g_Time_CS);
}

AdvPlot::~AdvPlot()
{
	DestoryAllLine();
	DeleteCriticalSection(&g_Time_CS);
}

BOOL AdvPlot::RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.lpszClassName = "AdvPlot"; // matches class name in client
	wc.hInstance = hInstance;
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = 0;
	wc.lpszMenuName = NULL;
	wc.hbrBackground = (HBRUSH) ::GetStockObject(LTGRAY_BRUSH);
	wc.style = CS_GLOBALCLASS; // To be modified
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	return (::RegisterClass(&wc) != 0);
}

BEGIN_MESSAGE_MAP(AdvPlot, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

inline void AdvPlot::Redraw()
{
	Invalidate();
}

void AdvPlot::ComputeRects()
{  
	//plotarea
	if(m_clientRect.Height()/7>=80)
		m_plotarea.m_plotRect.top=80;
	else
	 	m_plotarea.m_plotRect.top = m_clientRect.top  + m_clientRect.Height()/7;
    if(m_clientRect.Height()/6>=70)
		m_plotarea.m_plotRect.bottom = m_clientRect.bottom - 70;
	else
        m_plotarea.m_plotRect.bottom = m_clientRect.bottom - m_clientRect.Height()/6;
	if(m_clientRect.Width()/7>=110)
	{
		m_plotarea.m_plotRect.left=110;
		m_plotarea.m_plotRect.right = m_clientRect.right-110;
	}
	else
	{
	    m_plotarea.m_plotRect.left = m_clientRect.left + m_clientRect.Width()/7;
	    m_plotarea.m_plotRect.right = m_clientRect.right - m_clientRect.Width()/7;
	}

    m_plotarea.grid_resolution_x = (double)m_plotarea.m_plotRect.Width()/(double)m_plotarea.grid_count_x;
    m_plotarea.grid_resolution_y = (double)m_plotarea.m_plotRect.Height()/(double)m_plotarea.grid_count_y;
	
	//计算m_leftaxis.m_axisRect
	m_leftaxis.m_axisRect.left = m_clientRect.left;
    m_leftaxis.m_axisRect.right = m_plotarea.m_plotRect.left-1;
    m_leftaxis.m_axisRect.top = m_plotarea.m_plotRect.top;
    m_leftaxis.m_axisRect.bottom = m_plotarea.m_plotRect.bottom;
	m_leftaxis.axis_resolution_y = m_plotarea.grid_resolution_y;
	m_leftaxis.axis_count_y = m_plotarea.grid_count_y;

	//计算m_rightaxis.m_axisRect
	m_rightaxis.m_axisRect.left = m_plotarea.m_plotRect.right+1;
    m_rightaxis.m_axisRect.right = m_clientRect.right;
	m_rightaxis.m_axisRect.top = m_plotarea.m_plotRect.top;
	m_rightaxis.m_axisRect.bottom = m_plotarea.m_plotRect.bottom;
	m_rightaxis.axis_resolution_y = m_plotarea.grid_resolution_y;
	m_rightaxis.axis_count_y = m_plotarea.grid_count_y;

	//m_topaxis.m_axisRect
	m_topaxis.m_axisRect.top = m_clientRect.top;
	m_topaxis.m_axisRect.left = m_clientRect.left;
	m_topaxis.m_axisRect.right = m_clientRect.right;
	m_topaxis.m_axisRect.bottom = m_plotarea.m_plotRect.top;

	//m_timeaxis.m_axisRect
	m_timeaxis.m_axisRect.left = m_plotarea.m_plotRect.left;
    m_timeaxis.m_axisRect.right = m_plotarea.m_plotRect.right;
	m_timeaxis.m_axisRect.top = m_plotarea.m_plotRect.bottom;
	m_timeaxis.m_axisRect.bottom = m_clientRect.bottom;
	m_timeaxis.axis_resolution_x=m_plotarea.grid_resolution_x;
    m_timeaxis.axis_count_x=m_plotarea.grid_count_x;

	//time的拖放和缩放工具
	m_timeaxis.m_BarRect.left = m_timeaxis.m_axisRect.left;
	m_timeaxis.m_BarRect.right = m_timeaxis.m_axisRect.right;
	m_timeaxis.m_BarRect.top = m_timeaxis.m_axisRect.top+WIDTH_LARGRAXIS;
	m_timeaxis.m_BarRect.bottom = m_timeaxis.m_BarRect.top + m_timeaxis.m_axisRect.Height()*2/3;
} 
// AdvPlot message handlers

void AdvPlot::OnPaint() 
{
//TRACE("AdvPlot::OnPaint\n");
	CPaintDC dc(this); // device context for painting

	EnterCriticalSection(&g_Time_CS);

	if (IsWindowVisible()) 
	{
			CDC * pMemDC = new CDC;
			ASSERT(pMemDC);
			CDC * pDC = &dc;
			ASSERT(pDC);

			// 内存绘图
			CBitmap memBitmap;
			CBitmap * pOldBmp = NULL;

			// 创建内存绘图设备
			if (!pMemDC->CreateCompatibleDC(pDC)) goto END_DRAW0;
			//TRACE("DrawItem:pMemDC->CreateCompatibleDC(pDC)\n");
			if (!memBitmap.CreateCompatibleBitmap(pDC, m_clientRect.Width(), m_clientRect.Height())) goto END_DRAW0;
			//TRACE("DrawItem:pMemDC->CreateCompatibleBitmap\n");
			pOldBmp = pMemDC->SelectObject(&memBitmap);
			if (!pMemDC->BitBlt(m_clientRect.left, m_clientRect.top, m_clientRect.Width(), m_clientRect.Height(), pDC, 0, 0, SRCCOPY)) goto END_DRAW;
            //网格背景
        	if (!DrawGrid(pMemDC)) goto END_DRAW;
			//绘y轴
			if (!DrawAxis(pMemDC)) goto END_DRAW;
			//绘X time轴
			if (!DrawTimeAxis(pMemDC)) goto END_DRAW;
			//绘 top
			if (!DrawTopAxis(pMemDC)) goto END_DRAW;
			// 绘曲线
			if (!UpdateLine(pMemDC, pDC, m_plotarea.m_plotRect)) goto END_DRAW;
            if (!DrawTopText(pMemDC)) goto END_DRAW;
			if (!DrawBottomText(pMemDC)) goto END_DRAW;
			// 把内存绘图拷贝到屏幕
			pDC->BitBlt(m_clientRect.left, m_clientRect.top,  m_clientRect.Width(), m_clientRect.Height(), pMemDC, 0, 0, SRCCOPY);

END_DRAW:// 清理
			//TRACE("DrawItem:END_DRAW\n");
			pMemDC->SelectObject(pOldBmp);
			memBitmap.DeleteObject();
END_DRAW0:
			//TRACE("DrawItem:END_DRAW0\n");
			pMemDC->DeleteDC();
			ReleaseDC(pDC);

			if (pMemDC) delete pMemDC;
		}
	LeaveCriticalSection(&g_Time_CS);
//TRACE("Leave AdvPlot::OnPaint\n");
}

bool AdvPlot::DrawGrid(CDC *pMemDC)
{
		if (!pMemDC->GetSafeHdc()) return false;

		//int nBkMode = pMemDC->SetBkMode(TRANSPARENT);
		// 填充背景（圆角矩形）
		CBrush brush;
		brush.CreateSolidBrush(m_plotarea.bgClr);
		CRgn m_rgn;
		if (!m_rgn.CreateRoundRectRgn(m_clientRect.left, m_clientRect.top, m_clientRect.right, m_clientRect.bottom, BOUNDARY_ROUND_RADIUS, BOUNDARY_ROUND_RADIUS)) return false;
		if (!pMemDC->FillRgn(&m_rgn, &brush)) return false;

		if(GetFocus() == this)
			CMFCVisualManager::GetInstance()->OnDrawPopupWindowBorder(pMemDC, m_clientRect);

		// 画网格   
		if (m_plotarea.bDrawGrid) 
		{
            // 根据透明度合成网格颜色
			BYTE bR = (BYTE)(m_plotarea.fGridTransparent * GetRValue(m_plotarea.gridClr) + (1.0 - m_plotarea.fGridTransparent) * GetRValue(m_plotarea.bgClr));
			BYTE bG = (BYTE)(m_plotarea.fGridTransparent * GetGValue(m_plotarea.gridClr) + (1.0 - m_plotarea.fGridTransparent) * GetGValue(m_plotarea.bgClr));
			BYTE bB = (BYTE)(m_plotarea.fGridTransparent * GetBValue(m_plotarea.gridClr) + (1.0 - m_plotarea.fGridTransparent) * GetBValue(m_plotarea.bgClr));
			// 画网格
			CPen pen;
			if (!pen.CreatePen(m_plotarea.nGridStyle, 2, RGB(bR, bG, bB))) return false;
			CPen * pOldPen = pMemDC->SelectObject(&pen);
			
			//画边框
            pMemDC->MoveTo(m_plotarea.m_plotRect.left, m_plotarea.m_plotRect.top);
			pMemDC->LineTo(m_plotarea.m_plotRect.left, m_plotarea.m_plotRect.bottom);
            pMemDC->MoveTo(m_plotarea.m_plotRect.left, m_plotarea.m_plotRect.bottom);
			pMemDC->LineTo(m_plotarea.m_plotRect.right, m_plotarea.m_plotRect.bottom);
			pMemDC->MoveTo(m_plotarea.m_plotRect.right, m_plotarea.m_plotRect.bottom);
			pMemDC->LineTo(m_plotarea.m_plotRect.right, m_plotarea.m_plotRect.top);
            pMemDC->MoveTo(m_plotarea.m_plotRect.right, m_plotarea.m_plotRect.top);
			pMemDC->LineTo(m_plotarea.m_plotRect.left, m_plotarea.m_plotRect.top);

			pMemDC->SelectObject(pOldPen);

			// 网格竖线

			CPen pen1;
			if (!pen1.CreatePen(m_plotarea.nGridStyle, 1, RGB(bR, bG, bB))) return false;
			CPen * pOldPen1 = pMemDC->SelectObject(&pen1);

			for(int i=0; i<=m_plotarea.grid_count_x; i++)
			{
				int k = (int)(m_plotarea.m_plotRect.left+m_plotarea.grid_begin_x+i*m_plotarea.grid_resolution_x);
				if(k>m_plotarea.m_plotRect.right+1)
					continue;
				pMemDC->MoveTo(k, m_plotarea.m_plotRect.top);
				pMemDC->LineTo(k, m_plotarea.m_plotRect.bottom);
			}
			// 网格横线
			for(int i=0;i<=m_plotarea.grid_count_y; i++)
			{
				int k = (int)(m_plotarea.m_plotRect.top+i*m_plotarea.grid_resolution_y);
				if(k>m_plotarea.m_plotRect.bottom+1)
					continue;
				pMemDC->MoveTo(m_plotarea.m_plotRect.left, k );
				pMemDC->LineTo(m_plotarea.m_plotRect.right, k);
			}
			// 恢复画笔
			pMemDC->SelectObject(pOldPen1);
		}		// 绘网格完毕
		//pMemDC->SetBkMode(nBkMode);
		return true;
}

bool AdvPlot::DrawAxis(CDC *pMemDC)
{   //画左边Y轴
	{
		CPen pen;
		if (!pen.CreatePen(PS_SOLID, 2, m_leftaxis.axisclr)) return false;
		CPen * pOldPen = pMemDC->SelectObject(&pen);
		//画小步进线用到和确定字体的大小
		double smallaxis = m_leftaxis.axis_resolution_y/5.0;
	    //画竖线
        pMemDC->MoveTo(m_leftaxis.m_axisRect.right, m_leftaxis.m_axisRect.bottom);
		pMemDC->LineTo(m_leftaxis.m_axisRect.right, m_leftaxis.m_axisRect.top);
		//画大步进线
		if( m_leftaxis.bDrawLargeaxisY)
		{
			for(int i=0;i<=m_leftaxis.axis_count_y;i++)
			{
				double k=m_leftaxis.m_axisRect.top+i*m_leftaxis.axis_resolution_y;

				pMemDC->MoveTo(m_leftaxis.m_axisRect.right, (int)k);
		        pMemDC->LineTo(m_leftaxis.m_axisRect.right-WIDTH_LARGRAXIS , (int)k);
				 //画小步进线
				if( m_leftaxis.bDrawSmallaxisY && k<m_leftaxis.m_axisRect.bottom)
				{
					if(smallaxis<=0) break;  //防止因图太小出现死循环
					for(int j=1; j<5; j++)
					{
                        pMemDC->MoveTo(m_leftaxis.m_axisRect.right, (int)(k+smallaxis*j));
		                pMemDC->LineTo(m_leftaxis.m_axisRect.right-WIDTH_SMALLAXIS ,(int)(k+smallaxis*j));
					}
				}
			}
		}
		pMemDC->SelectObject(pOldPen);

		// 报存原始文本参数
		if( m_leftaxis.bDrawLargeaxisY || m_leftaxis.bDrawSmallaxisY)
		{
		     COLORREF clrText = pMemDC->GetTextColor();
		     int nBkMode = pMemDC->GetBkMode();
			//  COLORREF  clr = Getlineclr(m_leftaxis.szTitle);
			// if(clr!=0)
			//     pMemDC->SetTextColor(clr); //有线的设定为与使用轴坐标线一样的颜色
			// else
			     pMemDC->SetTextColor(m_leftaxis.textclr);
		     pMemDC->SetBkMode(TRANSPARENT);
			 CSize fsize, tsize, tsize1;
             //输出大步进线对应的标签
			 if( m_leftaxis.bDrawLargeaxisY || m_leftaxis.bDrawSmallaxisY)
			 {
				  CFont TextFont;
				  TextFont.CreateFont(m_leftaxis.TextFHeight, 0, 0, 0, FW_THIN, false, false, false, DEFAULT_CHARSET,
			                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH,m_leftaxis.FStyle);
		          CFont * pOldFont = pMemDC->SelectObject(&TextFont);
				  //获得dc中的当前字体的文本的高度与宽度
				  fsize = pMemDC->GetTextExtent(_T("1"),1);

				  //assert((m_leftaxis.m_properity>=Y_AXIS_PROPERITY_MIN)&&(m_leftaxis.m_properity<=Y_AXIS_PROPERITY_MAX));
				  CString b;
				  double yvale;
				  double min;

					if(m_leftaxis.m_properity<30)
					{
							yvale=YAxisInterval[m_leftaxis.m_properity];
							m_leftaxis.danwei.Format(_T("%0.2f mV/%s"), yvale, GRID_DISPLAY_NAME);
							min=m_leftaxis.minrange;			
							m_leftaxis.nr=2;
					}
					else if(m_leftaxis.m_properity<39)
					{
							yvale=YAxisInterval[m_leftaxis.m_properity];
							m_leftaxis.danwei.Format(_T("%0.0f mV/%s"), yvale, GRID_DISPLAY_NAME);
							min=m_leftaxis.minrange;
							m_leftaxis.nr=0;
					}
					else if(m_leftaxis.m_properity<=Y_AXIS_PROPERITY_MAX)
					{
							yvale=YAxisInterval[m_leftaxis.m_properity]/1000;
							m_leftaxis.danwei.Format(_T("%0.0f V/%s"),yvale, GRID_DISPLAY_NAME);
							min=m_leftaxis.minrange/1000;
							m_leftaxis.nr=0;
					}

				   for(int i=0; i<=m_leftaxis.axis_count_y; i++)
		           {	
						    b.Format(_T("%*.*f"),m_leftaxis.nl, m_leftaxis.nr,  (double)(yvale*i + min));

	                        pMemDC->DrawText(b, CRect(m_leftaxis.m_axisRect.left, m_leftaxis.m_axisRect.bottom-(int)(m_leftaxis.axis_resolution_y*i)-fsize.cy/2, 
				     			  m_leftaxis.m_axisRect.right-WIDTH_LARGRAXIS, m_leftaxis.m_axisRect.bottom-(int)(m_leftaxis.axis_resolution_y*i)+fsize.cy/2), DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);
				   }

				  //画一条零点的对应线
				  //if(min<=0 && yvale*m_leftaxis.axis_count_y+min>=0)
				  // if(min<=YLeftCenterLine && yvale*m_leftaxis.axis_count_y+min>=YLeftCenterLine)
				  //{
				  //    CPen pen;
		    //          if (!pen.CreatePen(PS_DOT, 1, RGB(255,255,255))) return false;
	     //       	      CPen * pOldPen = pMemDC->SelectObject(&pen);
      //                 //计算零点位置
					 //  int zeropos=(int)(m_plotarea.m_plotRect.bottom-
						//   ((YLeftCenterLine-min)*(m_plotarea.m_plotRect.bottom-m_plotarea.m_plotRect.top)/(yvale*m_leftaxis.axis_count_y)));
      //                 pMemDC->MoveTo(m_plotarea.m_plotRect.left+1, zeropos);
		    //           pMemDC->LineTo(m_plotarea.m_plotRect.right-1, zeropos);

      //                 pMemDC->SelectObject(pOldPen);
				  //}
				   pMemDC->SelectObject(pOldFont);
			 }

			  //显示单位
		   if(m_leftaxis.danwei!=_T(""))
		   {
			      int fontheight=m_leftaxis.m_axisRect.Width()/4<m_leftaxis.danweiHeight? m_leftaxis.m_axisRect.Width()/4:m_leftaxis.danweiHeight;
			      CFont TitleFont;
				  TitleFont.CreateFont(fontheight, 0, 900, 0, FW_SEMIBOLD, false, false, false, DEFAULT_CHARSET,
			                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, m_leftaxis.danweiFontStyle);
		          CFont * pOldFont = pMemDC->SelectObject(&TitleFont);
				  tsize = pMemDC->GetTextExtent(m_leftaxis.danwei);
				  UINT lastalign = pMemDC->SetTextAlign(TA_LEFT);   
				  int left = m_leftaxis.m_axisRect.left+m_leftaxis.m_axisRect.Width()/3-tsize.cy/2;
               	  pMemDC->DrawText(m_leftaxis.danwei, CRect(left, m_leftaxis.m_axisRect.top-m_leftaxis.m_axisRect.Height()/2, left+tsize.cx, 
		                   m_leftaxis.m_axisRect.top+m_leftaxis.m_axisRect.Height()/2+tsize.cx/2+tsize.cy), DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
				  pMemDC->SetTextAlign(lastalign); 
				  pMemDC->SelectObject(pOldFont);
		   }
		    // 恢复原始文本参数
		     pMemDC->SetTextColor(clrText);
		     pMemDC->SetBkMode(nBkMode);
		}
		
	}

    //画右边Y轴
	{
		CPen pen;
		if (!pen.CreatePen(PS_SOLID, 2, m_rightaxis.axisclr)) return false;
		CPen * pOldPen = pMemDC->SelectObject(&pen);
		//画小步进线用到
		double smallaxis = m_rightaxis.axis_resolution_y/5.0;
	    //画竖线
        pMemDC->MoveTo(m_rightaxis.m_axisRect.left, m_rightaxis.m_axisRect.bottom);
		pMemDC->LineTo(m_rightaxis.m_axisRect.left, m_rightaxis.m_axisRect.top);
		//画大步进线
		if( m_rightaxis.bDrawLargeaxisY)
		{
			for(int i=0;i<=m_rightaxis.axis_count_y;i++)
			{
				double k=m_rightaxis.m_axisRect.top+i*m_rightaxis.axis_resolution_y;

				pMemDC->MoveTo(m_rightaxis.m_axisRect.left, (int)k);
		        pMemDC->LineTo(m_rightaxis.m_axisRect.left+WIDTH_LARGRAXIS , (int)k);
				 //画小步进线
				if( m_rightaxis.bDrawSmallaxisY && k<m_rightaxis.m_axisRect.bottom)
				{
					if(smallaxis<=0) break;  //防止因图太小出现死循环
					for(int j=1; j<5; j++)
					{
                        pMemDC->MoveTo(m_rightaxis.m_axisRect.left, (int)(k+smallaxis*j));
		                pMemDC->LineTo(m_rightaxis.m_axisRect.left+WIDTH_SMALLAXIS ,(int)(k+smallaxis*j));
					}
				}
			}
		}
		pMemDC->SelectObject(pOldPen);
	
		if( m_rightaxis.bDrawLargeaxisY || m_rightaxis.bDrawSmallaxisY)
		{
			// 报存原始文本参数
		     COLORREF clrText = pMemDC->GetTextColor();
		     int nBkMode = pMemDC->GetBkMode();
		     //COLORREF clr=Getlineclr(m_rightaxis.szTitle);
			// if(clr!=0)
			 //    pMemDC->SetTextColor(clr); //有线的设定为与使用轴坐标线一样的颜色
			// else
			     pMemDC->SetTextColor(m_rightaxis.textclr);
		     pMemDC->SetBkMode(TRANSPARENT);
			 CSize fsize, tsize, tsize1;
             //输出大步进线对应的标签
			 if( m_rightaxis.bDrawLargeaxisY || m_rightaxis.bDrawSmallaxisY)
			 {
				 CFont TextFont;
				  TextFont.CreateFont(m_rightaxis.TextFHeight, 0, 0, 0, FW_THIN, false, false, false, DEFAULT_CHARSET,
			                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH,m_rightaxis.FStyle);
		          CFont * pOldFont = pMemDC->SelectObject(&TextFont);
				  fsize = pMemDC->GetTextExtent(_T("1"),1);
                 
				  CString b;
				  double yvale=YAxisInterval[m_rightaxis.m_properity];
				  double min;

					if(m_rightaxis.m_properity<30)
					{
							yvale=YAxisInterval[m_rightaxis.m_properity];
							m_rightaxis.danwei.Format(_T("%0.2f mV/%s"), yvale, GRID_DISPLAY_NAME);
							min=m_rightaxis.minrange;			
							m_rightaxis.nr=2;
					}
					else if(m_rightaxis.m_properity<39)
					{
							yvale=YAxisInterval[m_rightaxis.m_properity];
							m_rightaxis.danwei.Format(_T("%0.0f mV/%s"), yvale, GRID_DISPLAY_NAME);
							min=m_rightaxis.minrange;
							m_rightaxis.nr=0;
					}
					else if(m_rightaxis.m_properity<=Y_AXIS_PROPERITY_MAX)
					{
							yvale=YAxisInterval[m_rightaxis.m_properity]/1000;
							m_rightaxis.danwei.Format(_T("%0.0f V/%s"),yvale, GRID_DISPLAY_NAME);
							min=m_rightaxis.minrange/1000;
							m_rightaxis.nr=0;
					}

				   for(int i=0; i<=m_rightaxis.axis_count_y; i++)
		           {	
						    b.Format(_T("%*.*f"),m_rightaxis.nl, m_rightaxis.nr,  (double)(yvale*i + min));

	                        pMemDC->DrawText(b, CRect(m_rightaxis.m_axisRect.left+3+WIDTH_LARGRAXIS, m_rightaxis.m_axisRect.bottom-(int)(m_rightaxis.axis_resolution_y*i)-fsize.cy/2, 
						               m_rightaxis.m_axisRect.right, m_rightaxis.m_axisRect.bottom-(int)(m_rightaxis.axis_resolution_y*i)+fsize.cy/2), DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
				   }

				   //画一条零点的对应线
				  //if(min<=0 && yvale*m_plotarea.grid_count_y+min>=0)
				  //{
				  //    CPen pen;
		    //          if (!pen.CreatePen(PS_DOT, 1, RGB(255,255,255))) return false;
	     //       	      CPen * pOldPen = pMemDC->SelectObject(&pen);
      //                 //计算零点位置
					 //  int zeropos=(int)(m_plotarea.m_plotRect.bottom-
						//   ((0-min)*(m_plotarea.m_plotRect.bottom-m_plotarea.m_plotRect.top)/(yvale*m_plotarea.grid_count_y)));
      //                 pMemDC->MoveTo(m_plotarea.m_plotRect.left+1, zeropos);
		    //           pMemDC->LineTo(m_plotarea.m_plotRect.right-1, zeropos);

      //                 pMemDC->SelectObject(pOldPen);
				  //}
				   pMemDC->SelectObject(pOldFont);
			 }
		   //显示单位
		   if(m_rightaxis.danwei!=_T(""))
		   {
			   int fontheight=m_rightaxis.m_axisRect.Width()/4<m_rightaxis.danweiHeight? m_rightaxis.m_axisRect.Width()/4:m_rightaxis.danweiHeight;
			      CFont TitleFont;
				  TitleFont.CreateFont(fontheight, 0, 900, 0, FW_SEMIBOLD, false, false, false, DEFAULT_CHARSET,
			                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH,  m_rightaxis.danweiFontStyle);
		          CFont * pOldFont = pMemDC->SelectObject(&TitleFont);
				   tsize = pMemDC->GetTextExtent(m_rightaxis.danwei);
				  UINT lastalign = pMemDC->SetTextAlign(TA_LEFT);   
				  int left = m_rightaxis.m_axisRect.left+m_rightaxis.m_axisRect.Width()*2/3-tsize.cy/2;
               	  pMemDC->DrawText(m_rightaxis.danwei, CRect(left, m_rightaxis.m_axisRect.top-m_rightaxis.m_axisRect.Height()/2, left+tsize.cx, 
		                   m_rightaxis.m_axisRect.top+m_rightaxis.m_axisRect.Height()/2+tsize.cx/2+tsize.cy), DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
				  pMemDC->SetTextAlign(lastalign); 
				  pMemDC->SelectObject(pOldFont);
		    }
		    // 恢复原始文本参数
		     pMemDC->SetTextColor(clrText);
		     pMemDC->SetBkMode(nBkMode);
		}
	}
	return true;
}

bool AdvPlot::DrawTimeAxis(CDC *pMemDC)
{
	if(m_timeaxis.axis_enable == true)
	{
		CPen pen;
		if (!pen.CreatePen(PS_SOLID, 2, m_timeaxis.axisclr)) return false;
		CPen * pOldPen = pMemDC->SelectObject(&pen);
		//画横线
		pMemDC->MoveTo(m_timeaxis.m_axisRect.left, m_timeaxis.m_axisRect.top);
		pMemDC->LineTo(m_timeaxis.m_axisRect.right, m_timeaxis.m_axisRect.top);

         //画小步进线用到
		double smallaxis = m_timeaxis.axis_resolution_x/5;
		//画大步进线
		if(m_timeaxis.bDrawLargeaxisX)
		{
			for(int i=0; i<=m_timeaxis.axis_count_x; i++) //为了消除启动网格移动后最后一个刻度超出界限
			{
				double k = m_timeaxis.m_axisRect.left+i*m_timeaxis.axis_resolution_x;
				if(k>=m_timeaxis.m_axisRect.left)
				{
                     pMemDC->MoveTo((int)k, m_timeaxis.m_axisRect.top);
				     pMemDC->LineTo((int)k, m_timeaxis.m_axisRect.top+WIDTH_LARGRAXIS);
				}
				 //画小步进线
				 if(m_timeaxis.bDrawSmallaxisX && i!=m_timeaxis.axis_count_x+1)
				 {
					if(smallaxis<=0) break;  //防止因图太小出现死循环
					for(int j=0; j<5; j++)
					{
						if( k+smallaxis*j < m_timeaxis.m_axisRect.left) continue;
						if( k+smallaxis*j > m_timeaxis.m_axisRect.right) break;
						pMemDC->MoveTo( (int)(k+smallaxis*j), m_timeaxis.m_axisRect.top);
		                pMemDC->LineTo( (int)(k+smallaxis*j), m_timeaxis.m_axisRect.top+WIDTH_SMALLAXIS);
					}
				}
			}
		}
        pMemDC->SelectObject(pOldPen);

		if( m_timeaxis.bDrawLargeaxisX || m_timeaxis.bDrawSmallaxisX || m_timeaxis.bDrawTitle)
		{
		     COLORREF clrText = pMemDC->GetTextColor();
		     int nBkMode = pMemDC->GetBkMode();
		     pMemDC->SetTextColor(m_timeaxis.textclr);
		     pMemDC->SetBkMode(TRANSPARENT);
			 CSize fsize;
             //输出大步进线对应的标签
			 if( m_timeaxis.bDrawLargeaxisX || m_timeaxis.bDrawSmallaxisX)
			 {
				  CFont  TextFont;
                  TextFont.CreateFont(m_timeaxis.TextFHeight, 0, 0, 0, FW_THIN, false, false, false, DEFAULT_CHARSET,
			                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, m_timeaxis.TextFStyle);
		          CFont * pOldFont = pMemDC->SelectObject(&TextFont);
                  fsize =  pMemDC->GetTextExtent(_T("1"),1);
  
				  CString b;
				  double timevale,min;

				  if(m_timeaxis.m_properity<AXIS_CENTER)
				  {
					    assert(false);
				  }
				  else if(m_timeaxis.m_properity>=AXIS_CENTER && m_timeaxis.m_properity<=8+AXIS_CENTER)
				  {
					    timevale=YAxisInterval[m_timeaxis.m_properity];
						min=m_timeaxis.m_mintime;
					    b.Format(_T("%0.0f ns/%s"), timevale, GRID_DISPLAY_NAME);
					    m_timeaxis.m_szTitle=b;
				   }
				   else if(m_timeaxis.m_properity>=9+AXIS_CENTER && m_timeaxis.m_properity<=17+AXIS_CENTER)
				   {
					    timevale=YAxisInterval[m_timeaxis.m_properity]/1000 ;
						min=m_timeaxis.m_mintime/1000;
					    b.Format(_T("%0.0f us/%s"),timevale, GRID_DISPLAY_NAME);
                        m_timeaxis.m_szTitle=b;
				   }
				   else if(m_timeaxis.m_properity>=18+AXIS_CENTER && m_timeaxis.m_properity<=26+AXIS_CENTER)
				   {
					   timevale=YAxisInterval[m_timeaxis.m_properity]/1000000;
					   min=m_timeaxis.m_mintime/1000000;
					   b.Format(_T("%0.0f ms/%s"),timevale, GRID_DISPLAY_NAME);
                       m_timeaxis.m_szTitle=b;
				   }
				   else if(m_timeaxis.m_properity>=27+AXIS_CENTER)
				   {
					   timevale= YAxisInterval[m_timeaxis.m_properity]/1000000000;
					   min=m_timeaxis.m_mintime/1000000000;
					   b.Format(_T("%0.0f s/%s"),timevale, GRID_DISPLAY_NAME);
					   m_timeaxis.m_szTitle=b;
				   }
				   else
				   {
					    assert(false);
				   }

				   for(int i=0; i<=m_timeaxis.axis_count_x; i++)
		           {	
						 b.Format(_T("%*.*f"),m_timeaxis.nl, m_timeaxis.nr,  (double)(timevale*i + min));

	                     pMemDC->DrawText(b, CRect((int)(m_timeaxis.m_axisRect.left+i*m_timeaxis.axis_resolution_x-m_timeaxis.axis_resolution_x/2),
								             m_timeaxis.m_axisRect.top+WIDTH_SMALLAXIS, 
											 (int)(m_timeaxis.m_axisRect.left+i*m_timeaxis.axis_resolution_x+m_timeaxis.axis_resolution_x/2),
											 m_timeaxis.m_axisRect.bottom), DT_CENTER|DT_SINGLELINE);
				  }
		          pMemDC->SelectObject(pOldFont);
			 }
			
			 if( m_timeaxis.bDrawTitle)
			 {
				 int height=m_timeaxis.m_axisRect.Height()/3<m_timeaxis.TitleFHeight? m_timeaxis.m_axisRect.Height()/3:m_timeaxis.TitleFHeight;
				  CFont  TitleFont;
				  TitleFont.CreateFont(height, 0, 0, 0, FW_SEMIBOLD, false, false, false, DEFAULT_CHARSET,
			                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, m_timeaxis.TitleFStyle);
				  CFont * pOldFont = pMemDC->SelectObject(&TitleFont);
				  fsize =  pMemDC->GetTextExtent(_T("1"),1);
                  pMemDC->DrawText(m_timeaxis.m_szTitle, CRect(m_timeaxis.m_axisRect.left, m_timeaxis.m_axisRect.top+m_timeaxis.m_axisRect.Height()*2/3+WIDTH_SMALLAXIS-fsize.cy/2, 
					              m_timeaxis.m_axisRect.right, m_timeaxis.m_axisRect.top+m_timeaxis.m_axisRect.Height()*2/3+WIDTH_SMALLAXIS+fsize.cy/2), DT_CENTER|DT_SINGLELINE);
                  pMemDC->SelectObject(pOldFont);
			 }
		    // 恢复原始文本参数
		     pMemDC->SetTextColor(clrText);
		     pMemDC->SetBkMode(nBkMode);
		}
	}
	return true;
}

bool AdvPlot::DrawTopAxis(CDC *pMemDC)
{
	{
		int fonth=m_topaxis.m_axisRect.Height()/2>m_topaxis.TitleFHeight? m_topaxis.TitleFHeight:m_topaxis.m_axisRect.Height()/2;
        // 报存原始文本参数
		COLORREF clrText = pMemDC->GetTextColor();
		int nBkMode = pMemDC->GetBkMode();
		pMemDC->SetTextColor(m_topaxis.titleclr);
		pMemDC->SetBkMode(TRANSPARENT);
		CFont   TitleFont;
        TitleFont.CreateFont(fonth, 0, 0, 0, FW_THIN, false, false, false, DEFAULT_CHARSET,
			                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, m_topaxis.TitleFStyle);
	    CFont * pOldFont = pMemDC->SelectObject(&TitleFont);
        CSize fsize = pMemDC->GetTextExtent(m_topaxis.m_szTitle);
        pMemDC->DrawText(m_topaxis.m_szTitle, CRect(m_topaxis.m_axisRect.left, m_topaxis.m_axisRect.top+m_topaxis.m_axisRect.Height()/2-fsize.cy/2, 
			             m_topaxis.m_axisRect.right, m_topaxis.m_axisRect.bottom-m_topaxis.m_axisRect.Height()/2+fsize.cy/2), DT_CENTER|DT_SINGLELINE);

		pMemDC->SelectObject(pOldFont);
		// 恢复原始文本参数
		pMemDC->SetTextColor(clrText);
		pMemDC->SetBkMode(nBkMode);
	}
	return true;
}

bool AdvPlot::DrawTopText(CDC *pMemDC)  //顶端的文字显示
{
	if(lefttoptext!=_T("") || lefttoptext2!=_T("") || righttoptext!=_T("") || righttoptext2!=_T("") ||centertoptext!=_T(""))
	{
		CFont   Font;
        Font.CreateFont(textheight, 0, 0, 0, FW_THIN, false, false, false, DEFAULT_CHARSET,
			                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, _T("Times New Roman"));
	    CFont * pOldFont = pMemDC->SelectObject(&Font);
		int nBkMode = pMemDC->GetBkMode();
		pMemDC->SetBkMode(TRANSPARENT);
        COLORREF OldClr = pMemDC->SetTextColor(textclr);
		CSize tsize = pMemDC->GetTextExtent(_T("Y"),1);
		if(lefttoptext!=_T(""))
	       pMemDC->DrawText(lefttoptext, CRect(m_clientRect.left+2, m_clientRect.top+2, m_clientRect.right,  m_clientRect.top+2+tsize.cy), DT_LEFT);
		if(lefttoptext2!=_T(""))
			pMemDC->DrawText(lefttoptext2, CRect(m_clientRect.left+2, m_clientRect.top+2+tsize.cy, m_clientRect.right,  m_clientRect.top+2+tsize.cy*2), DT_LEFT);
		if(righttoptext!=_T(""))
           pMemDC->DrawText(righttoptext, CRect(m_clientRect.left, m_clientRect.top+2, m_clientRect.right-2,  m_clientRect.top+2+tsize.cy), DT_RIGHT);
		if(righttoptext2!=_T(""))
           pMemDC->DrawText(righttoptext2, CRect(m_clientRect.left+2, m_clientRect.top+2+tsize.cy, m_clientRect.right,  m_clientRect.top+2+tsize.cy*2), DT_RIGHT);
		if(centertoptext!=_T(""))
			pMemDC->DrawText(centertoptext, CRect(m_clientRect.left, m_clientRect.top+2, m_clientRect.right,  m_clientRect.top+2+tsize.cy), DT_CENTER);
		if(centertoptext2!=_T(""))
			pMemDC->DrawText(centertoptext2, CRect(m_clientRect.left+2, m_clientRect.top+2+tsize.cy, m_clientRect.right,  m_clientRect.top+2+tsize.cy*2), DT_CENTER);
		pMemDC->SetBkMode(nBkMode);
		pMemDC->SetTextColor(OldClr);
		pMemDC->SelectObject(pOldFont);
	}
	return true;
}

bool AdvPlot::DrawBottomText(CDC *pMemDC)
{
	if(rightbottomtext!=_T("") || rightbottomtext2!=_T(""))
	{
		CFont   Font;
        Font.CreateFont(textheight, 0, 0, 0, FW_THIN, false, false, false, DEFAULT_CHARSET,
			                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, _T("Times New Roman"));
	    CFont * pOldFont = pMemDC->SelectObject(&Font);
		int nBkMode = pMemDC->GetBkMode();
		pMemDC->SetBkMode(TRANSPARENT);
        COLORREF OldClr = pMemDC->SetTextColor(textclr);
		CSize tsize = pMemDC->GetTextExtent(_T("Y"),1);
        if(rightbottomtext!=_T(""))
	       pMemDC->DrawText(rightbottomtext, CRect(m_clientRect.left, m_clientRect.bottom-2-tsize.cy, m_clientRect.right-2, m_clientRect.bottom-2), DT_RIGHT);
	    if(rightbottomtext2!=_T(""))
	       pMemDC->DrawText(rightbottomtext, CRect(m_clientRect.left, m_clientRect.bottom-2-2*tsize.cy, m_clientRect.right-2, m_clientRect.bottom-2-tsize.cy), DT_RIGHT);
		pMemDC->SetBkMode(nBkMode);
		pMemDC->SetTextColor(OldClr);
		pMemDC->SelectObject(pOldFont);
	}
	return true;
}

bool AdvPlot::UpdateLine(CDC *pMemDC, CDC *pDC, const CRect &rect)
{
	if (!pMemDC->GetSafeHdc()) return false;
//TRACE("UpdateLine In\n");		
	CPoint orgBrushOrigin = pMemDC->GetBrushOrg();
	pMemDC->SetBrushOrg(rect.left, rect.bottom);

    //创建一个临时的绘图的cdc，为了提供多条曲线叠加时，变色的目的
    CDC TempDC;
	CBitmap TempBitmap;
	CBitmap * pTempOldBmp = NULL;
	//建立一个空的CDC用于绘图的拷贝
	CDC TransDC;
	CBitmap BitmapTrans;
	CBitmap * pTranOldBmp = NULL;

	if (!TempDC.CreateCompatibleDC(pDC))
	{
    	pMemDC->SetBrushOrg(orgBrushOrigin.x, orgBrushOrigin.y);
		return false;
	}
	if (!TempBitmap.CreateCompatibleBitmap(pDC, m_clientRect.Width(), m_clientRect.Height())) 
	{
    	pMemDC->SetBrushOrg(orgBrushOrigin.x, orgBrushOrigin.y);
		TempDC.SelectObject(pTempOldBmp);
     	TempDC.DeleteDC();
		return false;
	}
	pTempOldBmp = TempDC.SelectObject(&TempBitmap);

	if (!TransDC.CreateCompatibleDC(pDC))
	{
    	pMemDC->SetBrushOrg(orgBrushOrigin.x, orgBrushOrigin.y);
		TempBitmap.DeleteObject();
	    TempDC.SelectObject(pTempOldBmp);
	    TempDC.DeleteDC();
		return false;
	}
	BitmapTrans.CreateBitmap(m_clientRect.Width(), m_clientRect.Height(),1,1,NULL);
	TransDC.SetBkColor(m_plotarea.bgClr);
    pTranOldBmp=TransDC.SelectObject(&BitmapTrans);

	// 遍历每一条曲线
    int lastx, lasty, x,y;
	
	for (std::list<lineattribute*>::iterator list_Iter = linelist.begin(); list_Iter != linelist.end(); ++list_Iter) 
	{
		// 根据网格透明度合成曲线颜色
	    BYTE bR = (BYTE)(m_plotarea.fGridTransparent * GetRValue((*list_Iter)->clr) + (1.0 - m_plotarea.fGridTransparent) * GetRValue(m_plotarea.gridClr));
    	BYTE bG = (BYTE)(m_plotarea.fGridTransparent * GetGValue((*list_Iter)->clr) + (1.0 - m_plotarea.fGridTransparent) * GetGValue(m_plotarea.gridClr));
	    BYTE bB = (BYTE)(m_plotarea.fGridTransparent * GetBValue((*list_Iter)->clr) + (1.0 - m_plotarea.fGridTransparent) * GetBValue(m_plotarea.gridClr));
        // 画线
		CPen pen;
		if (!pen.CreatePen((*list_Iter)->nLineStyle, (*list_Iter)->nWidth, RGB(bR, bG, bB))) 
	    {
            pMemDC->SetBrushOrg(orgBrushOrigin.x, orgBrushOrigin.y);
	        goto END;
        }
		CPen * pOldPen = TempDC.SelectObject(&pen);

		TempDC.BitBlt(rect.left, rect.top,  rect.Width(), rect.Height(), &TransDC, 0, 0, SRCCOPY);
			//根据使用左边还是右边的坐标计算范围
		double min,nRange;
		if((*list_Iter)->lr_lengend<0)
		{
			min=m_leftaxis.minrange;
            nRange = m_leftaxis.maxrange-m_leftaxis.minrange;
		}
		else if((*list_Iter)->lr_lengend>0)
		{
			min=m_rightaxis.minrange;
			nRange = m_rightaxis.maxrange-m_rightaxis.minrange;
		}
		int nH = rect.bottom - rect.top;

		//确定绘图范围
		double timerealmin=m_timeaxis.m_mintime;
		double timerealmax=m_timeaxis.m_maxtime;
		if(m_timeaxis.m_mintime<(*list_Iter)->timestart)
			timerealmin=(*list_Iter)->timestart;
		if((*list_Iter)->timeend<m_timeaxis.m_maxtime)
			timerealmax=(*list_Iter)->timeend;
		//计算绘图位置
		int databegin=timerealmin*(*list_Iter)->length/((*list_Iter)->timeend-(*list_Iter)->timestart);
		int datacount=(timerealmax-timerealmin)*(*list_Iter)->length/((*list_Iter)->timeend-(*list_Iter)->timestart);

		int plotbegin=(timerealmin-m_timeaxis.m_mintime)*rect.Width()/(m_timeaxis.m_maxtime-m_timeaxis.m_mintime)+rect.left;

		for(int i=databegin;i<databegin+datacount;i++)
		{
			if(i==databegin)
			{
				lastx=(((*list_Iter)->timeend-(*list_Iter)->timestart)*i/(*list_Iter)->length-timerealmin)*rect.Width()/(m_timeaxis.m_maxtime-m_timeaxis.m_mintime)+plotbegin;
				lasty=rect.bottom-(int)((((*list_Iter)->buf)[i]-min)/nRange*(double)nH);    
				continue;
			}
			x=(((*list_Iter)->timeend-(*list_Iter)->timestart)*i/(*list_Iter)->length-timerealmin)*rect.Width()/(m_timeaxis.m_maxtime-m_timeaxis.m_mintime)+plotbegin;
			y=rect.bottom-(int)((((*list_Iter)->buf)[i]*1000-min)/nRange*(double)nH); 
			TempDC.MoveTo(lastx, lasty);
		    TempDC.LineTo(x, y);
			lastx=x;
			lasty=y;
		}

		// 恢复画笔
	    TempDC.SelectObject(pOldPen);
		pMemDC->BitBlt(rect.left, rect.top,  rect.Width(), rect.Height(), &TempDC, rect.left, rect.top, SRCINVERT);
	}

END:
	BitmapTrans.DeleteObject();
    TransDC.SelectObject(pTranOldBmp);
	TransDC.DeleteDC();
	TempBitmap.DeleteObject();
	TempDC.SelectObject(pTempOldBmp);
	TempDC.DeleteDC();
	pMemDC->SetBrushOrg(orgBrushOrigin.x, orgBrushOrigin.y);
//TRACE("UpdateLine Out\n");	
	return true;
}

BOOL AdvPlot::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

void AdvPlot::PreSubclassWindow()
{
	GetClientRect(m_clientRect);
	//PopupInti();
	ComputeRects();

	CWnd::PreSubclassWindow();
}

void AdvPlot::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (IsWindowVisible() && cx > 50 && cy > 20 ) 
	{
		GetClientRect(m_clientRect);
	    ComputeRects();
		Redraw();
	}
}

BOOL AdvPlot::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

bool AdvPlot::SetCursor(int hCurID, int enhCurID)
{
	HINSTANCE	hInstResource = NULL;
	// Destroy any previous cursor
	if (m_handCur)
	{
		::DestroyCursor(m_handCur);
		m_handCur = NULL;
	} // if

	// Load cursor
	if (hCurID)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(hCurID), RT_GROUP_CURSOR);
		// Load cursor resource
		m_handCur = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(hCurID), IMAGE_CURSOR, 0, 0, 0);
		// If something wrong
		if (m_handCur == NULL) return false;
	} // if

	// Destroy any previous cursor
	if (m_enhandCur)
	{
		::DestroyCursor(m_enhandCur);
		m_enhandCur = NULL;
	} // if

	// Load cursor
	if (enhCurID)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(enhCurID), RT_GROUP_CURSOR);
		// Load cursor resource
		m_enhandCur = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(enhCurID), IMAGE_CURSOR, 0, 0, 0);
		// If something wrong
		if (m_enhandCur == NULL) return false;
	} // if

	return true;
}

//------------------------------------------------------------------------------------------------------------//
bool AdvPlot::Create(const RECT & rect, CWnd * pParentWnd, DWORD dwStyle, UINT nID)
{
		bool bfOk = false;
		bfOk = CWnd::Create("AdvPlot", NULL, dwStyle, rect, pParentWnd, nID) == TRUE ? true : false;
		if (!bfOk) return false;

		ModifyStyle(WS_BORDER, WS_VISIBLE|WS_CHILD|SS_OWNERDRAW);
		return true;
}

bool AdvPlot::Create(const int nTop, const int nLeft, const int nRight, const int nBottom, CWnd * pParentWnd, DWORD dwStyle, UINT nID)
{
		bool bfOk = false;
		bfOk = CWnd::Create("AdvPlot", NULL, dwStyle, CRect(nTop, nLeft, nRight, nBottom), pParentWnd, nID) == TRUE ? true : false;
		if (!bfOk) return false;

		ModifyStyle(WS_BORDER, WS_VISIBLE|WS_CHILD|SS_OWNERDRAW);
		return true;
}
//------------------------------------------------------------------------------------------------------------//
void AdvPlot::SetbgColor(COLORREF clrBg, bool bRepaint)
{
	m_plotarea.bgClr = clrBg;
	if (bRepaint) Redraw();
}

void AdvPlot::GetbgColor(COLORREF &clr)
{
	clr = m_plotarea.bgClr;
}

void AdvPlot::SetGrid(BK_GRID bDrawgGrid, COLORREF clrGrid, int lineStyle, int nTransparence, bool bRepaint)
{
	m_plotarea.bDrawGrid = bDrawgGrid == 0 ? true : false;
	m_plotarea.gridClr = clrGrid;
	m_plotarea.nGridStyle = lineStyle;
	m_plotarea.fGridTransparent = (float)(nTransparence / 100.0);

	if (bRepaint) Redraw();
}

void AdvPlot::GetGrid(bool &bfShow, COLORREF &clr, int &nStyle, int &nTransparence)
{
	bfShow = m_plotarea.bDrawGrid;
	clr = m_plotarea.gridClr;
	nStyle = m_plotarea.nGridStyle;
	nTransparence = int(m_plotarea.fGridTransparent / 100.0);
}

void AdvPlot::SetYLeftDanwei(CString Danwei, bool bRepaint)
{
	m_leftaxis.danwei = Danwei;
	if (bRepaint) 
		InvalidateRect(m_leftaxis.m_axisRect);
}

void AdvPlot::SetYRightDanwei(CString Danwei, bool bRepaint)
{
	m_rightaxis.danwei = Danwei;
	if (bRepaint) 
		InvalidateRect(m_rightaxis.m_axisRect);
}

void AdvPlot::SetYAxisDanweiFontHeight(int fheight,bool bRepaint)
{
	m_leftaxis.danweiHeight=fheight;
	m_rightaxis.danweiHeight=fheight;
	if (bRepaint)
	{
		InvalidateRect(m_leftaxis.m_axisRect);
		InvalidateRect(m_rightaxis.m_axisRect);
	}
}

void AdvPlot::SetYAxisDanweiFontStyle(CString style, bool bRepaint)
{
	m_leftaxis.danweiFontStyle=style;
	m_rightaxis.danweiFontStyle=style;
	if (bRepaint)
	{
		InvalidateRect(m_leftaxis.m_axisRect);
		InvalidateRect(m_rightaxis.m_axisRect);
	}
}

void AdvPlot::SetYAxisDanwei_TextClr(COLORREF clr, bool bRepaint)
{
	m_leftaxis.textclr=clr;
	m_rightaxis.textclr=clr;
	if (bRepaint)
	{
		InvalidateRect(m_leftaxis.m_axisRect);
		InvalidateRect(m_rightaxis.m_axisRect);
	}
}

void AdvPlot::SetYLeftRange(double min, double max, bool bRepaint)
{
	SetRange(&m_leftaxis.minrange, &m_leftaxis.maxrange, &m_leftaxis.m_properity, m_leftaxis.axis_count_y,min, max);
	if (bRepaint)
		Redraw();
}

void AdvPlot::GetYLeftRange(double *min, double *max)
{
	*min=m_leftaxis.minrange;
	*max=m_leftaxis.maxrange;
}

void AdvPlot::SetYRightRange(double min, double max, bool bRepaint)
{
	SetRange(&m_rightaxis.minrange, &m_rightaxis.maxrange, &m_rightaxis.m_properity, m_rightaxis.axis_count_y, min, max);
	if (bRepaint)
		Redraw();
}

void AdvPlot::GetYRightRange(double *min, double *max)
{
	*min=m_rightaxis.minrange;
	*max=m_rightaxis.maxrange;
}

void AdvPlot::SetRange(double *dstmin, double *dstmax, int *dstproperity, int axis_count, double min, double max)
{
	assert(min!=max);
	double inr=(max-min)/axis_count;
	int i=0;
	if(max(min,max)>YAxisInterval[AXIS_COUNT-1])
	{
		i=AXIS_COUNT-1;
	}
	else
	{
		for(i=0;i<AXIS_COUNT;i++)
		{
			if((YAxisInterval[i]<=inr)&&(inr<YAxisInterval[i+1]))
			{
				if((inr!=YAxisInterval[i])||(min/YAxisInterval[i]!=((int)(min/YAxisInterval[i]))))
					i++;
				break;
			}
		}
	}
    double center=((int)((min+(max-min)/2)/YAxisInterval[i]))*YAxisInterval[i];
    *dstmin=center-axis_count/2*YAxisInterval[i];
	*dstmax=center+axis_count/2*YAxisInterval[i];
	*dstproperity=i;
}

void AdvPlot::SetTopTitle(CString Title, bool bRepaint)
{
	m_topaxis.m_szTitle = Title;
	if (bRepaint) 
		InvalidateRect(m_topaxis.m_axisRect);
}

void AdvPlot::SetTopTitleFontHeight(int fonth,bool bRepaint)
{
	m_topaxis.TitleFHeight = fonth;
	if (bRepaint) 
		InvalidateRect(m_topaxis.m_axisRect);
}

void AdvPlot::SetTopTitleFontStyle(CString style, bool bRepaint)
{
	m_topaxis.TitleFStyle = style;
	if (bRepaint) 
		InvalidateRect(m_topaxis.m_axisRect);
}

void AdvPlot::SetTopTitleFontClr(COLORREF clr, bool bRepaint)
{
	m_topaxis.titleclr = clr;
	if (bRepaint) 
		InvalidateRect(m_topaxis.m_axisRect);
}

void AdvPlot::ActiveTimeBar(bool acv, bool bRepaint)
{
	m_timeaxis.barenable=acv;
	if (bRepaint) 
		InvalidateRect(m_timeaxis.m_axisRect);
}

void AdvPlot::SetTimeTitle(CString Title, bool bRepaint)
{
	m_timeaxis.m_szTitle=Title;
	if(bRepaint)
        InvalidateRect(m_timeaxis.m_axisRect);
}

void AdvPlot::SetTimeTitleFontHeight(int fonth, bool bRepaint)
{
    m_timeaxis.TitleFHeight=fonth;
	if(bRepaint)
        InvalidateRect(m_timeaxis.m_axisRect);
}

void AdvPlot::SetTimeTitleFontStyle(CString style, bool bRepaint)
{
	m_timeaxis.TitleFStyle=style;
	if(bRepaint)
        InvalidateRect(m_timeaxis.m_axisRect);
}

void AdvPlot::SetTimeAxisKeduFontStyle(CString style, bool bRepaint)
{
	m_timeaxis.TextFStyle=style;
	if(bRepaint)
        InvalidateRect(m_timeaxis.m_axisRect);
}

void AdvPlot::SetTimeAxisKeduFontHeight(int fheight, bool bRepaint)
{
	m_timeaxis.TextFHeight=fheight;
	if(bRepaint)
        InvalidateRect(m_timeaxis.m_axisRect);
}

void AdvPlot::SetTimeAxisTitle_TextClr(COLORREF clr, bool bRepaint)
{
	m_timeaxis.textclr=clr;
	if(bRepaint)
        InvalidateRect(m_timeaxis.m_axisRect);
}

void AdvPlot::SetTimeAxisKeduLineClr(COLORREF clr, bool bRepaint)
{
	m_timeaxis.axisclr=clr;
	if(bRepaint)
        InvalidateRect(m_timeaxis.m_axisRect);
}

void AdvPlot::SetTimeAxisRange(double min, double max,bool large, bool bRepaint)
{
    m_timeaxis.begintime = min;
	m_timeaxis.m_mintime = min;
	m_timeaxis.m_maxtime = max;
    double interval=(m_timeaxis.m_maxtime-m_timeaxis.m_mintime)/m_timeaxis.axis_count_x;

	if(interval>=10000000000)
		m_timeaxis.m_properity = 31+AXIS_CENTER;   //10s
	else if(interval>=5000000000 && interval<10000000000)
		m_timeaxis.m_properity = 29+AXIS_CENTER;   //5s
	else if(interval>=2000000000 && interval<5000000000)
		m_timeaxis.m_properity = 28+AXIS_CENTER;   //2s
	else if(interval>=1000000000 && interval<2000000000)
		m_timeaxis.m_properity = 27+AXIS_CENTER;   //1s 
	else if(interval>=500000000 && interval<1000000000)
		m_timeaxis.m_properity = 26+AXIS_CENTER;   //500ms
	else if(interval>=200000000 && interval<500000000)
		m_timeaxis.m_properity = 25+AXIS_CENTER;   //200ms
	else if(interval>=100000000 && interval<200000000)
		m_timeaxis.m_properity = 24+AXIS_CENTER;   //100ms
	else if(interval>=50000000 && interval<100000000)
		m_timeaxis.m_properity = 23+AXIS_CENTER;   //50ms
	else if(interval>=20000000 && interval<50000000)
		m_timeaxis.m_properity = 22+AXIS_CENTER;   //20ms
	else if(interval>=10000000 && interval<20000000)
		m_timeaxis.m_properity = 21+AXIS_CENTER;   //10ms
	else if(interval>=5000000 && interval<10000000)
		m_timeaxis.m_properity = 20+AXIS_CENTER;   //5ms
	else if(interval>=2000000 && interval<5000000)
		m_timeaxis.m_properity = 19+AXIS_CENTER;   //2ms
	else if(interval>=1000000 && interval<2000000)
		m_timeaxis.m_properity = 18+AXIS_CENTER;   //1ms
	else if(interval>=500000 && interval<1000000)
		m_timeaxis.m_properity = 17+AXIS_CENTER;   //500us
	else if(interval>=200000 && interval<500000)
		m_timeaxis.m_properity = 16+AXIS_CENTER;   //200us
	else if(interval>=100000 && interval<200000)
		m_timeaxis.m_properity = 15+AXIS_CENTER;   //100us
	else if(interval>=50000 && interval<100000)
		m_timeaxis.m_properity = 14+AXIS_CENTER;   //50us
	else if(interval>=20000 && interval<50000)
		m_timeaxis.m_properity = 13+AXIS_CENTER;   //20us
	else if(interval>=10000 && interval<20000)
		m_timeaxis.m_properity = 12+AXIS_CENTER;   //10us
	else if(interval>=5000 && interval<10000)
		m_timeaxis.m_properity = 11+AXIS_CENTER;   //5us
	else if(interval>=2000 && interval<5000)
		m_timeaxis.m_properity = 10+AXIS_CENTER;   //2us
	else if(interval>=1000 && interval<2000)
		m_timeaxis.m_properity = 9+AXIS_CENTER;   //1us
	else if(interval>=500 && interval<1000)
		m_timeaxis.m_properity = 8+AXIS_CENTER;   //500ns
	else if(interval>=200 && interval<500)
		m_timeaxis.m_properity = 7+AXIS_CENTER;   //200ns
	else if(interval>=100 && interval<200)
		m_timeaxis.m_properity = 6+AXIS_CENTER;   //100ns
	else if(interval>=50 && interval<100)
		m_timeaxis.m_properity = 5+AXIS_CENTER;   //50ns
	else if(interval>=20 && interval<50)
		m_timeaxis.m_properity = 4+AXIS_CENTER;   //20ns
	else if(interval>=10 && interval<20)
		m_timeaxis.m_properity = 3+AXIS_CENTER;   //10ns
	else if(interval>=5 && interval<10)
		m_timeaxis.m_properity = 2+AXIS_CENTER;   //5ns
	else if(interval>=2 && interval<5)
		m_timeaxis.m_properity = 1+AXIS_CENTER;   //2ns
	else
        m_timeaxis.m_properity = 0+AXIS_CENTER;   //1ns

	if(large&&(interval!=YAxisInterval[m_timeaxis.m_properity]))
		m_timeaxis.m_properity++;
	m_timeaxis.m_maxtime=m_timeaxis.m_mintime+YAxisInterval[m_timeaxis.m_properity]*m_timeaxis.axis_count_x;
	if (bRepaint) 
		Redraw();
}

void AdvPlot::GetTimeAxisRange(double *min, double *max)
{
	*min=m_timeaxis.m_mintime;
	*max=m_timeaxis.m_maxtime;
}

void AdvPlot::MoveTimeAxis(double time)
{
	EnterCriticalSection(&g_Time_CS); 
	double timeintvl=(m_timeaxis.m_maxtime-m_timeaxis.m_mintime)/m_timeaxis.axis_count_x;
	double move=ceil(time/timeintvl)*timeintvl;
	m_timeaxis.m_mintime+=move;
	m_timeaxis.m_maxtime+=move;
	LeaveCriticalSection(&g_Time_CS);
}

void AdvPlot::SetLeftTopText(CString text, bool bRepaint)
{
	lefttoptext=text;
	if(bRepaint)
        InvalidateRect(CRect(m_clientRect.left, m_clientRect.top, m_clientRect.right, m_plotarea.m_plotRect.top));
}

void AdvPlot::SetLeftTopText2(CString text, bool bRepaint)
{
	lefttoptext2=text;
	if(bRepaint)
        InvalidateRect(CRect(m_clientRect.left, m_clientRect.top, m_clientRect.right, m_plotarea.m_plotRect.top));
}

void AdvPlot::SetRightTopText(CString text, bool bRepaint)
{
	righttoptext=text;
	if(bRepaint)
        InvalidateRect(CRect(m_clientRect.left, m_clientRect.top, m_clientRect.right, m_plotarea.m_plotRect.top));
}

void AdvPlot::SetRightTopText2(CString text, bool bRepaint)
{
	righttoptext2=text;
	if(bRepaint)
        InvalidateRect(CRect(m_clientRect.left, m_clientRect.top, m_clientRect.right, m_plotarea.m_plotRect.top));
}

void AdvPlot::SetCenterTopText(CString text, bool bRepaint)
{
	centertoptext=text;
	if(bRepaint)
        InvalidateRect(CRect(m_clientRect.left, m_clientRect.top, m_clientRect.right, m_plotarea.m_plotRect.top));
}

void AdvPlot::SetCenterTopText2(CString text, bool bRepaint)
{
	centertoptext2=text;
	if(bRepaint)
        InvalidateRect(CRect(m_clientRect.left, m_clientRect.top, m_clientRect.right, m_plotarea.m_plotRect.top));
}

void AdvPlot::SetRightBottomText(CString text, bool bRepaint)
{
	rightbottomtext=text;
	if(bRepaint)
        InvalidateRect(CRect(m_clientRect.left, m_plotarea.m_plotRect.bottom, m_clientRect.right, m_clientRect.bottom));
}

void AdvPlot::SetRightBottomText2(CString text, bool bRepaint)
{
	rightbottomtext2=text;
	if(bRepaint)
        InvalidateRect(CRect(m_clientRect.left, m_plotarea.m_plotRect.bottom, m_clientRect.right, m_clientRect.bottom));
}

void AdvPlot::SetTopTextFontHeight(int fonth,bool bRepaint)
{
	textheight=fonth;
	if(bRepaint)
        InvalidateRect(CRect(m_clientRect.left, m_clientRect.top, m_clientRect.right, m_plotarea.m_plotRect.top));
}

void AdvPlot::SetTopTextFontClr(COLORREF clr, bool bRepaint)
{
	textclr=clr;
	if(bRepaint)
        InvalidateRect(CRect(m_clientRect.left, m_clientRect.top, m_clientRect.right, m_plotarea.m_plotRect.top));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AdvPlot::MoveVerTop(bool left_right, bool bRepaint)
{
	 if(left_right)
	{
		 if(m_leftaxis.minrange-YAxisInterval[m_leftaxis.m_properity]<-YAxisInterval[sizeof(YAxisInterval)/sizeof(double)-1]*10) 
			 return ;
		 m_leftaxis.minrange -= YAxisInterval[m_leftaxis.m_properity];
		 m_leftaxis.maxrange -= YAxisInterval[m_leftaxis.m_properity];
	}
    else
	{
		if(m_rightaxis.minrange-YAxisInterval[m_rightaxis.m_properity]<-YAxisInterval[sizeof(YAxisInterval)/sizeof(double)-1]*10) 
		     return ;
		 m_rightaxis.minrange -= YAxisInterval[m_rightaxis.m_properity];
		 m_rightaxis.maxrange -= YAxisInterval[m_rightaxis.m_properity];
	}
	if (bRepaint) Redraw();
}

void AdvPlot::MoveVerBottom(bool left_right, bool bRepaint)
{
	if(left_right)
	{
		if(m_leftaxis.maxrange+YAxisInterval[m_leftaxis.m_properity]>YAxisInterval[sizeof(YAxisInterval)/sizeof(double)-1]*10) 
	          return ;
		m_leftaxis.minrange += YAxisInterval[m_leftaxis.m_properity];
		m_leftaxis.maxrange += YAxisInterval[m_leftaxis.m_properity];
	}
	else
	{ 
		if(m_rightaxis.maxrange+YAxisInterval[m_rightaxis.m_properity]>YAxisInterval[sizeof(YAxisInterval)/sizeof(double)-1]*10) 
		     return ;
		m_rightaxis.minrange += YAxisInterval[m_rightaxis.m_properity];
		m_rightaxis.maxrange += YAxisInterval[m_rightaxis.m_properity];
	}
	 if (bRepaint) Redraw();
}

void AdvPlot::MoveLeft(int cnt, bool bRepaint)
{
	EnterCriticalSection(&g_Time_CS);
	if(m_timeaxis.m_maxtime+YAxisInterval[m_timeaxis.m_properity]>YAxisInterval[sizeof(YAxisInterval)/sizeof(double)-1]*10) 
	{
		LeaveCriticalSection(&g_Time_CS);
		return ;
	}
    m_timeaxis.m_mintime += YAxisInterval[m_timeaxis.m_properity];
	m_timeaxis.m_maxtime += YAxisInterval[m_timeaxis.m_properity];
	if (bRepaint) Redraw();
	LeaveCriticalSection(&g_Time_CS);
}

void AdvPlot::MoveRight(int cnt, bool bRepaint)
{
	EnterCriticalSection(&g_Time_CS); 
	if(m_timeaxis.m_mintime-YAxisInterval[m_timeaxis.m_properity]<-YAxisInterval[sizeof(YAxisInterval)/sizeof(double)-1]*10) 
	{
		LeaveCriticalSection(&g_Time_CS);
		return ;
	}
    m_timeaxis.m_mintime -= YAxisInterval[m_timeaxis.m_properity];
	m_timeaxis.m_maxtime -= YAxisInterval[m_timeaxis.m_properity];
	if (bRepaint) Redraw();
	LeaveCriticalSection(&g_Time_CS);
}

void AdvPlot::ZoomIn(bool bRepaint)
{
	EnterCriticalSection(&g_Time_CS); 
	if( m_timeaxis.m_properity<=AXIS_CENTER) 
	{
		 m_timeaxis.m_properity=AXIS_CENTER;
		 LeaveCriticalSection(&g_Time_CS);
		 return;
	}
	m_timeaxis.m_properity-=1;
	m_timeaxis.m_mintime = (m_timeaxis.m_mintime)/YAxisInterval[m_timeaxis.m_properity+1]*YAxisInterval[m_timeaxis.m_properity];
	m_timeaxis.m_maxtime = (m_timeaxis.m_maxtime)/YAxisInterval[m_timeaxis.m_properity+1]*YAxisInterval[m_timeaxis.m_properity];

	 if (bRepaint) Redraw();
	 LeaveCriticalSection(&g_Time_CS);
}

void AdvPlot::ZoomInPos(bool bRepaint)
{
	EnterCriticalSection(&g_Time_CS); 
	if( m_timeaxis.m_properity<=AXIS_CENTER) 
	{
		m_timeaxis.m_properity=AXIS_CENTER;
		LeaveCriticalSection(&g_Time_CS);
		 return;
	}
	int xxx=(int)((double)(m_timeaxis.leftbutdownpointx-m_timeaxis.m_axisRect.left)/(double)m_timeaxis.m_axisRect.Width()*m_timeaxis.axis_count_x+0.5);
	double center=YAxisInterval[m_timeaxis.m_properity]*xxx+m_timeaxis.m_mintime;

	double dd=(int)(center/YAxisInterval[m_timeaxis.m_properity-1])*YAxisInterval[m_timeaxis.m_properity-1];

    double cc=dd-YAxisInterval[m_timeaxis.m_properity-1]*xxx;

	m_timeaxis.m_mintime = cc;
	m_timeaxis.m_maxtime = cc+YAxisInterval[m_timeaxis.m_properity-1]*m_timeaxis.axis_count_x;

	m_timeaxis.m_properity-=1;

	if (bRepaint) Redraw();
	LeaveCriticalSection(&g_Time_CS);
}

void AdvPlot::ZoomInCenter(bool bRepaint)
{
	EnterCriticalSection(&g_Time_CS); 
	if( m_timeaxis.m_properity<=AXIS_CENTER) 
	{
		m_timeaxis.m_properity=AXIS_CENTER;
		LeaveCriticalSection(&g_Time_CS);
		 return;
	}
	 double center=m_timeaxis.m_mintime+YAxisInterval[m_timeaxis.m_properity]*m_timeaxis.axis_count_x/2;
	 int xx=(int)(center/YAxisInterval[m_timeaxis.m_properity-1]);
	 center = xx*YAxisInterval[m_timeaxis.m_properity-1];

	 m_timeaxis.m_properity-=1;
     m_timeaxis.m_mintime = center-YAxisInterval[m_timeaxis.m_properity]*m_timeaxis.axis_count_x/2;
     m_timeaxis.m_maxtime = center+YAxisInterval[m_timeaxis.m_properity]*m_timeaxis.axis_count_x/2;

	 if (bRepaint) Redraw();
	 LeaveCriticalSection(&g_Time_CS);
}

void AdvPlot::ZoomOut(bool bRepaint)
{
	EnterCriticalSection(&g_Time_CS);
	if( m_timeaxis.m_properity>=sizeof(YAxisInterval)/sizeof(double)-1) 
	{
		m_timeaxis.m_properity=sizeof(YAxisInterval)/sizeof(double)-1;
		LeaveCriticalSection(&g_Time_CS);
		return;
	} 
    m_timeaxis.m_properity+=1;
	m_timeaxis.m_mintime = (m_timeaxis.m_mintime)/YAxisInterval[m_timeaxis.m_properity-1]*YAxisInterval[m_timeaxis.m_properity];
	m_timeaxis.m_maxtime = (m_timeaxis.m_maxtime)/YAxisInterval[m_timeaxis.m_properity-1]*YAxisInterval[m_timeaxis.m_properity];
    if (bRepaint) Redraw();
	LeaveCriticalSection(&g_Time_CS);
}

void AdvPlot::ZoomOutPos(bool bRepaint)
{
	EnterCriticalSection(&g_Time_CS);
	if( m_timeaxis.m_properity>=sizeof(YAxisInterval)/sizeof(double)-1) 
	{
		m_timeaxis.m_properity=sizeof(YAxisInterval)/sizeof(double)-1;
		LeaveCriticalSection(&g_Time_CS);
		return;
	} 

	int xxx=(int)((double)(m_timeaxis.leftbutdownpointx-m_timeaxis.m_axisRect.left)/(double)m_timeaxis.m_axisRect.Width()*m_timeaxis.axis_count_x+0.5);
	double center=YAxisInterval[m_timeaxis.m_properity]*xxx+m_timeaxis.m_mintime;

	double dd=(int)(center/YAxisInterval[m_timeaxis.m_properity+1])*YAxisInterval[m_timeaxis.m_properity+1];

    double cc=dd-YAxisInterval[m_timeaxis.m_properity+1]*xxx;

	m_timeaxis.m_mintime = cc;
	m_timeaxis.m_maxtime = cc+YAxisInterval[m_timeaxis.m_properity+1]*m_timeaxis.axis_count_x;

    m_timeaxis.m_properity+=1;
	//m_timeaxis.m_mintime = (m_timeaxis.m_mintime)/YAxisInterval[m_timeaxis.m_properity-1]*YAxisInterval[m_timeaxis.m_properity];
	//m_timeaxis.m_maxtime = (m_timeaxis.m_maxtime)/YAxisInterval[m_timeaxis.m_properity-1]*YAxisInterval[m_timeaxis.m_properity];
    if (bRepaint) Redraw();
	LeaveCriticalSection(&g_Time_CS);
}

void AdvPlot::ZoomOutCenter(bool bRepaint)
{
	EnterCriticalSection(&g_Time_CS); 
	if( m_timeaxis.m_properity>=sizeof(YAxisInterval)/sizeof(double)-1) 
	{
		m_timeaxis.m_properity=sizeof(YAxisInterval)/sizeof(double)-1;
		LeaveCriticalSection(&g_Time_CS);
		return;
	} 
	 double center=m_timeaxis.m_mintime+YAxisInterval[m_timeaxis.m_properity]*m_timeaxis.axis_count_x/2;
	 int xx=(int)(center/YAxisInterval[m_timeaxis.m_properity+1]);
	 center = xx*YAxisInterval[m_timeaxis.m_properity+1];

	 m_timeaxis.m_properity+=1;
     m_timeaxis.m_mintime = center-YAxisInterval[m_timeaxis.m_properity]*m_timeaxis.axis_count_x/2;
     m_timeaxis.m_maxtime = center+YAxisInterval[m_timeaxis.m_properity]*m_timeaxis.axis_count_x/2;

	 if (bRepaint) Redraw();
	 LeaveCriticalSection(&g_Time_CS);
}

void AdvPlot::ZoomVerIn(bool left_right, bool bRepaint)
{
	 if(left_right)
	 {
		 if( m_leftaxis.m_properity<=Y_AXIS_PROPERITY_MIN) 
	     {
		     m_leftaxis.m_properity=Y_AXIS_PROPERITY_MIN;
	  	     return;
	     }
		 m_leftaxis.m_properity-=1;
		 m_leftaxis.minrange = (m_leftaxis.minrange)/YAxisInterval[m_leftaxis.m_properity+1]*YAxisInterval[m_leftaxis.m_properity];
		 m_leftaxis.maxrange = (m_leftaxis.maxrange)/YAxisInterval[m_leftaxis.m_properity+1]*YAxisInterval[m_leftaxis.m_properity];
	 }
	 else
	 {
		 if( m_rightaxis.m_properity<=Y_AXIS_PROPERITY_MIN) 
	     {
		     m_rightaxis.m_properity=Y_AXIS_PROPERITY_MIN;
	  	     return;
	     }
		 m_rightaxis.m_properity-=1;
		 m_rightaxis.minrange = (m_rightaxis.minrange)/YAxisInterval[m_rightaxis.m_properity+1]*YAxisInterval[m_rightaxis.m_properity];
		 m_rightaxis.maxrange = (m_rightaxis.maxrange)/YAxisInterval[m_rightaxis.m_properity+1]*YAxisInterval[m_rightaxis.m_properity];
	 }
	 if (bRepaint) Redraw();
}

void AdvPlot::ZoomVerInCenter(bool left_right, bool bRepaint)
{
	 if(left_right)
	 {
		 if( m_leftaxis.m_properity<=Y_AXIS_PROPERITY_MIN) 
	     {
		     m_leftaxis.m_properity=Y_AXIS_PROPERITY_MIN;
	  	     return;
	     }

		 double center=m_leftaxis.minrange+YAxisInterval[m_leftaxis.m_properity]*m_leftaxis.axis_count_y/2;
		 int xx=(int)(center/YAxisInterval[m_leftaxis.m_properity-1]);
		 center = xx*YAxisInterval[m_leftaxis.m_properity-1];

		 m_leftaxis.m_properity-=1;
         m_leftaxis.minrange = center-YAxisInterval[m_leftaxis.m_properity]*m_leftaxis.axis_count_y/2;
		 m_leftaxis.maxrange = center+YAxisInterval[m_leftaxis.m_properity]*m_leftaxis.axis_count_y/2;
	 }
	 else
	 {
		 if( m_rightaxis.m_properity<=Y_AXIS_PROPERITY_MIN) 
	     {
		     m_rightaxis.m_properity=Y_AXIS_PROPERITY_MIN;
	  	     return;
	     }

		 double center=m_rightaxis.minrange+YAxisInterval[m_rightaxis.m_properity]*m_rightaxis.axis_count_y/2;
		 int xx=(int)(center/YAxisInterval[m_rightaxis.m_properity-1]);
		 center = xx*YAxisInterval[m_rightaxis.m_properity-1];

		 m_rightaxis.m_properity-=1;
         m_rightaxis.minrange = center-YAxisInterval[m_rightaxis.m_properity]*m_rightaxis.axis_count_y/2;
		 m_rightaxis.maxrange = center+YAxisInterval[m_rightaxis.m_properity]*m_rightaxis.axis_count_y/2;
	 }
	 if (bRepaint) Redraw();
}

void AdvPlot::ZoomVerOut(bool left_right, bool bRepaint)
{
	 if(left_right)
	 {
		 if(m_leftaxis.m_properity>=Y_AXIS_PROPERITY_MAX)
		 {
			 m_leftaxis.m_properity=Y_AXIS_PROPERITY_MAX;
			 return ;
		 }
	     m_leftaxis.m_properity+=1;
		 m_leftaxis.minrange = (m_leftaxis.minrange)/YAxisInterval[m_leftaxis.m_properity-1]*YAxisInterval[m_leftaxis.m_properity];
		 m_leftaxis.maxrange = (m_leftaxis.maxrange)/YAxisInterval[m_leftaxis.m_properity-1]*YAxisInterval[m_leftaxis.m_properity];
	 }
     else 
	 {
		  if(m_rightaxis.m_properity>=Y_AXIS_PROPERITY_MAX)
		 {
			 m_rightaxis.m_properity=Y_AXIS_PROPERITY_MAX;
			 return ;
		 }
		 m_rightaxis.m_properity+=1;
		 m_rightaxis.minrange = (m_rightaxis.minrange)/YAxisInterval[m_rightaxis.m_properity-1]*YAxisInterval[m_rightaxis.m_properity];
		 m_rightaxis.maxrange = (m_rightaxis.maxrange)/YAxisInterval[m_rightaxis.m_properity-1]*YAxisInterval[m_rightaxis.m_properity];
	 }
	 if (bRepaint) Redraw();
}

void AdvPlot::ZoomVerOutCenter(bool left_right, bool bRepaint)
{
	 if(left_right)
	 {
		 if(m_leftaxis.m_properity>=Y_AXIS_PROPERITY_MAX)
		 {
			 m_leftaxis.m_properity=Y_AXIS_PROPERITY_MAX;
			 return ;
		 }

		 double center=m_leftaxis.minrange+YAxisInterval[m_leftaxis.m_properity]*m_leftaxis.axis_count_y/2;
		 int xx=(int)(center/YAxisInterval[m_leftaxis.m_properity+1]);
		 center = xx*YAxisInterval[m_leftaxis.m_properity+1];

		 m_leftaxis.m_properity+=1;
         m_leftaxis.minrange = center-YAxisInterval[m_leftaxis.m_properity]*m_leftaxis.axis_count_y/2;
		 m_leftaxis.maxrange = center+YAxisInterval[m_leftaxis.m_properity]*m_leftaxis.axis_count_y/2;
	 }
     else 
	 {
		  if(m_rightaxis.m_properity>=Y_AXIS_PROPERITY_MAX)
		 {
			 m_rightaxis.m_properity=Y_AXIS_PROPERITY_MAX;
			 return ;
		 }

		 double center=m_rightaxis.minrange+YAxisInterval[m_rightaxis.m_properity]*m_rightaxis.axis_count_y/2;
		 int xx=(int)(center/YAxisInterval[m_rightaxis.m_properity+1]);
		 center = xx*YAxisInterval[m_rightaxis.m_properity+1];

		 m_rightaxis.m_properity+=1;
         m_rightaxis.minrange = center-YAxisInterval[m_rightaxis.m_properity]*m_rightaxis.axis_count_y/2;
		 m_rightaxis.maxrange = center+YAxisInterval[m_rightaxis.m_properity]*m_rightaxis.axis_count_y/2;
	 }
	 if (bRepaint) Redraw();
}

void AdvPlot::CreateLine(lineattribute* line)
{
	EnterCriticalSection(&g_Time_CS);

	lineattribute* lineatr=new lineattribute();
	lineatr->pctName=line->pctName;
	lineatr->clr=line->clr;
	lineatr->lr_lengend=line->lr_lengend;
	lineatr->nLineStyle=line->nLineStyle;
	lineatr->nWidth=line->nWidth;
	lineatr->length=line->length;
	lineatr->buf=new double[lineatr->length];
	if(line->buf!=NULL)
		memcpy(lineatr->buf,line->buf,lineatr->length*sizeof(double));
	linelist.push_back(lineatr);
	LeaveCriticalSection(&g_Time_CS);
}

void AdvPlot::ChangeDatas(std::string name, double *buf, int length, double timestart, double timeend)
{
	EnterCriticalSection(&g_Time_CS);
	for (std::list<lineattribute*>::iterator list_Iter = linelist.begin(); list_Iter != linelist.end(); ++list_Iter) 
	{
		if((*list_Iter)->pctName==name)
		{
			(*list_Iter)->timestart=timestart;
			(*list_Iter)->timeend=timeend;
			memcpy((*list_Iter)->buf, buf, length*sizeof(double));
			break;
		}
	}
	LeaveCriticalSection(&g_Time_CS);
}

bool AdvPlot::HaveLine(std::string name)
{
	bool res=false;
	EnterCriticalSection(&g_Time_CS);
	for (std::list<lineattribute*>::iterator list_Iter = linelist.begin(); list_Iter != linelist.end(); ++list_Iter) 
	{
		if((*list_Iter)->pctName==name)
		{
			res=true;
			break;
		}
	}
	LeaveCriticalSection(&g_Time_CS);
	return res;
}

void AdvPlot::DestoryLine(std::string name)
{
	EnterCriticalSection(&g_Time_CS);
	for (std::list<lineattribute*>::iterator list_Iter = linelist.begin(); list_Iter != linelist.end(); ++list_Iter) 
	{
		if((*list_Iter)->pctName==name)
		{
			delete (*list_Iter);
			linelist.erase(list_Iter);
			break;
		}
	}
	LeaveCriticalSection(&g_Time_CS);
}

void AdvPlot::DestoryAllLine()
{
	EnterCriticalSection(&g_Time_CS);
	for (std::list<lineattribute*>::iterator list_Iter = linelist.begin(); list_Iter != linelist.end(); ++list_Iter) 
	{
		delete (*list_Iter);
	}
	linelist.erase(linelist.begin(), linelist.end());
    linelist.clear();
	LeaveCriticalSection(&g_Time_CS);
}

int AdvPlot::GetBufferLength(std::string name)
{
	int length=0;
	EnterCriticalSection(&g_Time_CS);
	for (std::list<lineattribute*>::iterator list_Iter = linelist.begin(); list_Iter != linelist.end(); ++list_Iter) 
	{
		if((*list_Iter)->pctName==name)
		{
			length=(*list_Iter)->length;
			break;
		}
	}
	LeaveCriticalSection(&g_Time_CS);
	return length;
}