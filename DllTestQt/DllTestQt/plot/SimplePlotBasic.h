#pragma once
/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
    www.instrustar.com
*********************************************************************************************************************/

#include "PlotBasic.h"
#include "PlotAxis.h"

class simplebasictimeaxis : public basictimeaxis
{
public:
	simplebasictimeaxis(Axis::X_AXIS_DANWEI disp_danwei, int IntervalMinIndex = 0, int IntervalMaxIndex = 63)
		: basictimeaxis(disp_danwei, IntervalMinIndex, IntervalMaxIndex)
	{
	}

	virtual void adjust();
	virtual void reset();

    virtual bool ZoomInPos(QPoint point) = 0;
    virtual bool ZoomOutPos(QPoint point) = 0;

	//水平的Offset显示中间位置的时间
	//double CalHorizontalOffsetPos(double offset) { return m_axisRect.CenterPoint().x - offset*m_axisRect.Width() / getrange(); };
	double GetHorizontalOffset() { return getminrange() + getrange()/2; };
	void SetHorizontalOffset(double offset) { movetime(offset - GetHorizontalOffset()); };
};

class simpledsotimeaxis : public simplebasictimeaxis
{
public:
	simpledsotimeaxis(Axis::X_AXIS_DANWEI disp_danwei, int IntervalMinIndex = 0, int IntervalMaxIndex = 63)
		: simplebasictimeaxis(disp_danwei, IntervalMinIndex, IntervalMaxIndex)
		, m_start(-Axis::AxisInterval[IntervalMinIndex] * 5)
		, m_axis_index(IntervalMinIndex)
		, m_trigger(0)
        , m_arrowleftbuttonpoint( QPoint(-1,-1))
	{
		gen_aixs_scales();
	}

	~simpledsotimeaxis()
	{
		destroy_aixs_scales();
	};

	virtual void SetAxisDanwei(Axis::X_AXIS_DANWEI dabwei);

	double getminrange(){return m_start;};
	double getmaxrange(){return m_start + getrange();};
	double getrange(){return getaxisindexvalue(m_axis_index)*axis_count_x;};
	void setrange(double start, double value)
	{
		m_start = start;
		setaxisindex(value);
	}

	virtual double getaxisindexvalue() { return getaxisindexvalue(m_axis_index); };

	virtual void changeproperity(double value, int zoomcenterpos);

	void movetime(double time){ m_start +=time;};

    bool ZoomInPos(QPoint point);
    bool ZoomOutPos(QPoint point);

	//Scale String
    std::vector<QString*>* GetAxisScaleStrings() { return &m_aixs_scale_strs; };
	int GetAxisScaleStringIndex() { return getaxisindex() - AxisIntervalMinIndex; };
	int GetAxisScaleStringCount() { return m_aixs_scale_strs.size(); }
	void SetAxisScaleStringIndex(int index, int zoomcenterpos) { changeproperity(getaxisindexvalue(index + AxisIntervalMinIndex), zoomcenterpos); };
private:
	int getaxisindex() { return m_axis_index; };
	int setaxisindex(double value) { return m_axis_index = value2axisindex(value); };
	double getaxisindexvalue(int index) { return Axis::AxisInterval[index]; };

	int value2axisindex(double value);

	void gen_aixs_scales();
	void destroy_aixs_scales();
public:
	//-------------------
	double	m_trigger;
    QRect		m_arrowRect;
    QPoint      m_arrowleftbuttonpoint;

private:
	double m_start;
	int m_axis_index;  //使用哪个缩放比例 数组中的值
    std::vector<QString*> m_aixs_scale_strs; //20201127用于刻度选择
};

class advplotarea  : public basicplotarea
{
public:
      int grid_begin_x; //绘制grid的开始坐标

    advplotarea()
        : grid_begin_x(0)
    {
    }
};


