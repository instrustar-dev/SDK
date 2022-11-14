#pragma once

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
    www.instrustar.com
*********************************************************************************************************************/

#include <QRect>
#include <vector>
#include "Axis.h"
#include "plottype.h"
#include "PlotHelp.h"
#include "PlotAxis.h"

//dpi适应20160527
#define BPI_96_ICON_SIZE 16.0 //一般电脑96dpi的时候图标尺寸16

#define TOP_INFO_NUM 1//一行文字
#define TOP_INTERVA_NUM 0 //一行空格
#define WAVEPOSBAR_NO_INTERVA_NUM 1.5
#define BOTTOM_INTERVA_BOTTOM_NUM 0.5
#define BOTTOM_INTERVA_TOP_NUM 1
#define LEFT_INFO_NUM 3
#define LEFT_INTERVA_NUM 0 //一行空格
#define RIGHT_INFO_NUM 3
#define RIGHT_INTERVA_NUM 0 //一行空格

#define SIMPLOT_AXISBAR_INTERVA_NUM 0.8
#define SIMPLOT_AXISBAR_WIDTH_NUM 9
//#define SIMPLOT_AXISBAR_DB_WIDTH_NUM 7.5
#define SIMPLOT_AXISBAR_FREQ_WIDTH_NUM 7

#define SMALL_FONT_BILI 0.9

#define ARROW_RANGE_NUM 0.1
#define Y_ARROW_WIDTH_NUM 2
#define Y_ARROW_HEIGHT_NUM 1.1
#define Y_ARROW_SHORT_WIDTH_NUM 0.5
#define X_ARROW_WIDTH_NUM 1.1
#define X_ARROW_HEIGHT_NUM 2
#define X_ARROW_SHORT_HEIGHT_NUM 0.5

#define SMALL_ARROW_RANGE_NUM 0.15
#define Y_SMALL_ARROW_WIDTH_NUM 1
#define Y_SMALL_ARROW_HEIGHT_NUM 0.8
#define Y_SMALL_ARROW_SHORT_WIDTH_NUM 0.2
#define X_SMALL_ARROW_WIDTH_NUM 0.8
#define X_SMALL_ARROW_HEIGHT_NUM 1
#define X_SMALL_ARROW_SHORT_HEIGHT_NUM 0.2

//Selectline上下三角的大小
#define LINRRANGLE_NUM 0.3125

//大步进值线的长度
#define WIDTH_LARGRAXIS_NUM  0.3125
//小步进值线的长度
#define WIDTH_SMALLAXIS_NUM  0.125
//Grid长度
#define GRID_PRO_SMALL_N 1
#define GRID_SMALL_1 1
#define GRID_LARGE_1 2

#define AXIS_LENGTH_NUM 0.3125
#define AXIS_ARROW_NUM 0.25
#define AXIS_OUTSIDE_NUM 0.125
#define AXIS_INTERVAL_NUM 0.75

#define TOP_BOTTOM_DRAW_INV_NUM 0.4

#define SIMPLE_OUTSIDE_NUM 0.25

class basictimeaxis
{
public:
    basictimeaxis(Axis::X_AXIS_DANWEI disp_danwei, int IntervalMinIndex = 0, int IntervalMaxIndex = Axis::AxisIntervalCount-1)
		: axis_resolution_x(0.0)
		, axis_count_x(10)
		, m_axis_danwei(disp_danwei)
		, m_Axis_clr(SIMPLE_AXIS_CLR)
		, AxisIntervalMinIndex(IntervalMinIndex)
		, AxisIntervalMaxIndex(IntervalMaxIndex)
	{
	}

	~basictimeaxis()
	{
	};

	virtual void SetAxisDanwei(Axis::X_AXIS_DANWEI dabwei) { m_axis_danwei = dabwei; };
	Axis::X_AXIS_DANWEI& GetAxisDanwei() { return m_axis_danwei; };

	int GetAxisIntervalMinIndex() { return AxisIntervalMinIndex; };
	int GetAxisIntervalMaxIndex() { return AxisIntervalMaxIndex; };

	virtual double getminrange()=0;
	virtual double getmaxrange()=0;
	virtual double getrange()=0;
	virtual void setrange(double start, double value) = 0;
	virtual void changeproperity(double value, int zoomcenterpos) = 0;

	virtual double getaxisindexvalue() = 0;

	virtual void movetime(double time) = 0;

	double movex2value(int move);

	//pos & value
	double converpos2value(int pos);
	int convervalue2pos(double value);
	double move2value(int movey);
public:
	double axis_resolution_x;
	int axis_count_x;

    QRect	m_axisRect;		// bottom x time axis

    QColor m_Axis_clr;
protected:
	int AxisIntervalMinIndex;  //最小刻度间隔index
	int AxisIntervalMaxIndex;  //最大刻度间隔index

	Axis::X_AXIS_DANWEI m_axis_danwei;
};

class basicplotarea
{
public:
      QRect		m_plotRect;			// clientRect - margins
	  bool bDrawGrid;// 是否绘网格
      Qt::PenStyle nGridStyle;// 网格线型

	  double grid_resolution_x;// 网格 X、Y 方向间距
	  double grid_resolution_y;
	  int grid_count_x;  //网格X轴方向的线数
	  int grid_count_y;  //网格Y轴方向的线数

      QColor m_bgClr, m_bgOutClr, m_gridClr;

	basicplotarea()
		: bDrawGrid(true)
        , nGridStyle(Qt::SolidLine)
		, grid_resolution_x(0)
		, grid_resolution_y(0)
		, grid_count_x(10)
		, grid_count_y(10)
		, m_bgClr(SIMPLE_PLOT_BACKCLR)
        , m_bgOutClr(SIMPLE_PLOT_OUTBACKCLR)
        , m_gridClr(SIMPLE_GRID_CLR)
	{
	}
};

