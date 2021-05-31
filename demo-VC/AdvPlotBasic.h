#pragma once
#include <afxwin.h>
//#include "defheader.h"

#define PLOT_RBUTDOWN  WM_USER + 9502  

static enum BK_GRID {GRID = 0, NOGRID = 1};

static const char* GRID_DISPLAY_NAME={"格"};


// 曲线每个方格线长度上绘图的的离散点数（分辨率）,其长度由GRID_COUNT确定    
#define POINT_COUNT   10
// 控件边角弧度半径
#define BOUNDARY_ROUND_RADIUS 5
//大步进值线的长度
#define WIDTH_LARGRAXIS  5
//小步进值线的长度
#define WIDTH_SMALLAXIS  2
//Selectline上下三角的大小
#define LINRRANGLE 5

#define AXIS_CENTER   30
#define AXIS_WEISHU(x)  (x>AXIS_CENTER? (x-AXIS_CENTER)/3:-(AXIS_CENTER-x)/3)
#define AXIS_COUNT  (sizeof(YAxisInterval)/sizeof(double))
#define IS_DUISHU_AXIS(x) (x%3==0)

static double YAxisInterval[]={  //17个
	                    0.0000000001,    //1
						0.0000000002,
						0.0000000005,
	                    0.000000001,  
						0.000000002,
						0.000000005,
						0.00000001,
						0.00000002,
						0.00000005,
						0.0000001,
						0.0000002,
						0.0000005,
						0.000001,
						0.000002,
						0.000005,
						0.00001,  //15
						0.00002,
						0.00005,
						0.0001,
						0.0002,
						0.0005,
						0.001, //21
						0.002,
						0.005,
	                    0.01,
	                    0.02,
	                    0.05,
	                    0.1,
	                    0.2,
	                    0.5,   //30
	                    1,  //mv  1
						2,
						5,
						10,
						20,
						50,
						100,
						200,
						500,
						1000,  //1v 10
						2000,
						5000,
						10000, //10v
						20000,
						50000,
						100000,
						200000,  //200v   17
						500000,
					    1000000,   
					    2000000,  //20
					    5000000,
					    10000000, 
					    20000000,
					    50000000,   //24 
					    100000000,  
					    200000000,
					    500000000,  //27
					    1000000000,//  28
					    2000000000,
						5000000000,  //30
					    10000000000,//10s  31
					    20000000000,
					    50000000000,
					    100000000000,//100s  34
					    200000000000,
					    500000000000  //36
                    };
//*******************************************************************************************************/
//* non-time axis. used for left and right y axis.
//*******************************************************************************************************/
class advaxis
{
public:
	CRect		m_axisRect;		// axisi rect

	double axis_resolution_y;// 刻度 Y 方向间距
	int axis_count_y;  //刻度 Y 轴方向的线数

	CString danwei; //用于左右轴显示一个单位
	int     danweiHeight;
    CString danweiFontStyle;

	int   TextFHeight;
	CString  FStyle;
	COLORREF textclr;  //单位和刻度的颜色
	COLORREF axisclr;   //刻度线的颜色

	bool bDrawLargeaxisY;  //画大标记
	bool bDrawSmallaxisY;  //画小标记
	
    char format;  //用于格式控制
	int nl,nr;    //用于格式控制   //%nl.nr format 例如 %0.6f
	double	minrange;
	double	maxrange;
	int m_properity;  //使用哪个缩放比例 数组中的值

	advaxis()
	{
		axis_resolution_y=0.0;
	    axis_count_y=10;
		danwei = _T("");
		danweiHeight = 13;
		danweiFontStyle=_T("宋体");
		FStyle = _T("Times New Roman");
		TextFHeight = 15;
		format='f';
		nl=0;
		nr=0;
		minrange	= -500.0; //-0.0000005; //-500.0;//-5000000.0; 
		maxrange	= 500.0; //0.0000005; //500.0;//5000000.0;
		m_properity = 36; //9; //36;//48;  //100mv
		textclr = RGB(255,0,255);
        axisclr     = RGB(255,0,0);
		bDrawLargeaxisY = true;
        bDrawSmallaxisY = true;
	};
};

//*******************************************************************************************************/
//* time axis
//*******************************************************************************************************/
class advtimeaxis
{
public:
	CRect	m_axisRect;		// bottom x time axis
	bool    axis_enable; //用于控制是否绘制

	double axis_resolution_x;
    int axis_count_x;

	bool        bDrawTitle;
	CString		m_szTitle;		// time axis title;
	int         TitleFHeight;
	CString     TitleFStyle;

	int         TextFHeight;
	CString     TextFStyle;
    COLORREF    textclr;
    COLORREF    axisclr;
	bool        bDrawLargeaxisX;
	bool        bDrawSmallaxisX;

	bool        time_x_zeroline; //绘制对应time轴0的垂直线

	//long long int    m_movebegintime;  //用于支持动态的移动增加
	double    begintime;  //支持左右拖动控制绘图的起始点
	double    m_mintime;		// min time
	double 	  m_maxtime;		// max time
	int       m_properity;  //使用哪个缩放比例 数组中的值
	double    m_spe_properity;//用于不使用YAxisInterval数组，的time轴控制
	double    m_org_mintime;		// min time  //用于不使用YAxisInterval数组，的time轴控制
	double 	  m_org_maxtime;		// max time  //用于不使用YAxisInterval数组，的time轴控制

	char format;  //用于格式控制
	int nl,nr;    //用于格式控制   //%nl.nr format 例如 %0.6f

    //bar的控制
	CRect       m_BarRect;       //用于拖动和缩放
	COLORREF timebarclr; //移动和缩放半透明工具颜色
	bool barenable;
	bool barpointbuttondown;  //指示是否鼠标按下
	CPoint barbeginPoint;
	bool barpointin;   //用于指示鼠标进入了，启动垂直缩放功能

	int leg; //绘制时间的起点
	int leftbutdownpointx; //用于中心缩放
	advtimeaxis()
	{
		axis_enable=true; 
		axis_resolution_x=0; 
	    axis_count_x=10;
		bDrawTitle = true;
		m_szTitle = "Time";
		TitleFHeight = 18;
		TitleFStyle = _TEXT("Times New Roman");
		m_mintime = -5000000.0; //-0.0000005; //-500.0;//-5000000.0; 
		m_maxtime = 5000000.0; //0.0000005; //500.0;//5000000.0;
		m_properity = 48; //9; //36;//48;  

		//m_movebegintime = 0;
		format='f';
		nl=0;
		nr=0;
		axisclr = RGB(255,0,0);
		TextFHeight = 15;
		TextFStyle = _TEXT("Times New Roman");
		textclr = RGB(255,0,255);
		bDrawLargeaxisX = true;
        bDrawSmallaxisX = true;

		time_x_zeroline=false;
		timebarclr=RGB(64, 96, 64);
		barenable=true;
		barpointbuttondown=false;
		barbeginPoint=(-1,-1);
		barpointin=false;

		leftbutdownpointx=0;
	}
};

//*******************************************************************************************************/
//* top axis
//*******************************************************************************************************/
class advtopaxis
{
public:
	CRect       m_axisRect;		// top x axis
	CString		m_szTitle;		// top axis title;
	int         TitleFHeight;
	CString     TitleFStyle;
	COLORREF    titleclr;
	advtopaxis()
	{
		m_szTitle = "数字示波器";
		TitleFHeight = 25;
		TitleFStyle = _TEXT("Times New Roman");
		titleclr = RGB(255,0,255);
	}
};

//*******************************************************************************************************/
//* plotarea
//*******************************************************************************************************/

class advplotarea
{
public:
	  CRect		m_plotRect;			// clientRect - margins
	  bool bDrawGrid;// 是否绘网格

	  COLORREF bgClr, gridClr;// 背景、网格
	  float fGridTransparent;// 网格透明度
	  int nGridStyle;// 网格线型
	  double grid_resolution_x;// 网格 X、Y 方向间距
	  double grid_resolution_y;
	  int grid_count_x;  //网格X轴方向的线数
	  int grid_count_y;  //网格Y轴方向的线数
	  int grid_begin_x; //绘制grid的开始坐标

	advplotarea(){
		bDrawGrid=true;
		bgClr=RGB(0, 0, 0);
	    gridClr=RGB(64, 96, 64);
		fGridTransparent=(float)1.0;
		nGridStyle=PS_SOLID;  
		grid_resolution_x=0; 
	    grid_resolution_y=0;
	    grid_count_x=10;
	    grid_count_y=10;
		grid_begin_x=0;
	}
};