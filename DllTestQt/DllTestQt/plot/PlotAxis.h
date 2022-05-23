#pragma once
/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
    www.instrustar.com
*********************************************************************************************************************/

#include <vector>
#include <QColor>
#include <QRect>

#include "Axis.h"

class simpleaxis
{
public:
    simpleaxis(const QString& trackname, QColor lineclr, Qt::PenStyle linestyle = Qt::SolidLine, int linewidth = 1,
        Axis::Y_AXIS_DANWEI disp_danwei = Axis::Y_AXIS_DANWEI::NO, int IntervalMinIndex = 0, int IntervalMaxIndex = Axis::AxisIntervalCount-1)
        : m_trackname(trackname)
		, m_lineclr(lineclr)
		, m_linestyle(linestyle)
		, m_linewidth(linewidth)
		, m_disp_danwei(disp_danwei)
		, m_start(-Axis::AxisInterval[IntervalMinIndex] * 5)
		, m_axis_index(IntervalMinIndex)
		, axis_count_y(10)
		, axis_resolution_y(0.0)
		, AxisIntervalMinIndex(IntervalMinIndex)
		, AxisIntervalMaxIndex(IntervalMaxIndex)
        , m_arrow_value(0)
        , m_buffer(NULL)
        , m_buffer_length(0)
        , m_buffer_time_min(0)
        , m_buffer_time_max(0)
	{
	};

    ~simpleaxis()
	{
	};

	virtual void SetAxisDanwei(Axis::Y_AXIS_DANWEI dabwei, int IntervalMinIndex, int IntervalMaxIndex);
	Axis::Y_AXIS_DANWEI& GetAxisDanwei() { return m_disp_danwei; };

	double getinterval() { return getaxisindexvalue(); };
	double getminrange() { return getstart(); };
	double getmaxrange() { return getstart() + getrange(); };
	double getrange() { return getaxisindexvalue() * axis_count_y; };
	void move(double move) { m_move(move); };

    virtual void setproperityvalue(double start, double value);

	//pos & value
    double converpos2value(int pos);
    int convervalue2pos(double value);
    double movey2value(int movey);
    double getyarrowvalue() { return m_arrow_value; };
    int getyarrowpos() { return convervalue2pos(m_arrow_value); };

	//zoom
	virtual bool VerZoomInPos(int zoompos);
	virtual bool VerZoomOutPos(int zoompos);
protected:
    double getstart() { return m_start; };
    void setstart(double start) { m_start = start; };
    void m_move(double move) { m_start += move ; };

	double getaxisindexvalue() { return axisindex2value(m_axis_index); };
private:
	int value2axisindex(double value);
	double axisindex2value(int index);
public:
    QString m_trackname;

    QColor m_lineclr; // 线色
    Qt::PenStyle m_linestyle;  // 线风格// 实线、虚线、点线、点划线、点点划线（SOLID, DASH, DOT, DASHDOT, DASHDOTDOT）
	int m_linewidth; // 线宽

	int axis_count_y;  //刻度 Y 轴方向的线数;
	double axis_resolution_y;// 刻度 Y 方向间距
    QRect		m_axisRect;		// axisi rect
    QRect		m_chnRect;
    QPoint      m_chnleftbuttonpoint;

    double* m_buffer;
    int32_t m_buffer_length;
    double m_buffer_time_min;
    double m_buffer_time_max;
protected:
	int AxisIntervalMinIndex;  //最小刻度间隔index
	int AxisIntervalMaxIndex;  //最大刻度间隔index
private:
	Axis::Y_AXIS_DANWEI m_disp_danwei;
	double	m_start;
	int m_axis_index;  //使用哪个缩放比例 数组中的值

    double m_arrow_value;
};

