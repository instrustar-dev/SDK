#include "SimplePlotBasic.h"

//---------------------------------------------------------------------------------------------------
void simplebasictimeaxis::adjust()
{
	const double zero_pos = convervalue2pos(0);

	double pos = m_axisRect.left() + m_axisRect.width() / 2.0;
	if (zero_pos <= m_axisRect.left())
		pos = m_axisRect.left();
	else if (zero_pos >= m_axisRect.right())
		pos = m_axisRect.right();
	else
	{
		const double temp = (zero_pos - m_axisRect.left()) / ((double)m_axisRect.width() / axis_count_x);
		double intptr;
		const double fractpart = modf(temp, &intptr);
		if (fractpart >= 0.5)
			intptr++;
		pos = m_axisRect.left() + intptr * ((double)m_axisRect.width() / axis_count_x);
	}
	movetime(move2value(zero_pos - pos));
}

void simplebasictimeaxis::reset()
{
	double v = getaxisindexvalue();
	setrange(-v * axis_count_x / 2, v);
}

//-------------------------------------------------------------------------simpledsotimeaxis----------------------------------------------
int simpledsotimeaxis::value2axisindex(double value)
{
	int i = AxisIntervalMinIndex;
	for (; i <= AxisIntervalMaxIndex; i++)
	{
		if (Axis::AxisInterval[i] >= value)
			break;
	}
	return i;
}

void simpledsotimeaxis::gen_aixs_scales()
{
	for (int i = AxisIntervalMinIndex; i <= AxisIntervalMaxIndex; i++)
	{
		QString* scale = new QString();
		PlotHelp m_plothelp;
		m_plothelp.XAxisValueStr(*scale, m_axis_danwei, Axis::AxisInterval[i], 0, 0, true);
		m_aixs_scale_strs.push_back(scale);
	}
}

void simpledsotimeaxis::destroy_aixs_scales()
{
	for (auto list_Iter = m_aixs_scale_strs.begin(); list_Iter != m_aixs_scale_strs.end(); ++list_Iter)
	{
		delete (*list_Iter);
		(*list_Iter) = NULL;
	}
	m_aixs_scale_strs.erase(m_aixs_scale_strs.begin(), m_aixs_scale_strs.end());
	m_aixs_scale_strs.clear();
}

void simpledsotimeaxis::changeproperity(double value, int zoomcenterpos)
{
	//计算缩放中心
	//int pos = convervalue2pos(zoomcenter);
	double zoomcenter = converpos2value(zoomcenterpos);
	m_axis_index = value2axisindex(value);
	m_start = zoomcenter - (zoomcenterpos - m_axisRect.left()) * getrange() / m_axisRect.width();
}

bool simpledsotimeaxis::ZoomInPos(QPoint point)
{
	bool res = false;
	if (m_axis_index > GetAxisIntervalMinIndex())
	{
		double zoomcenter = converpos2value(point.x());
		m_axis_index--;
		m_start = zoomcenter - (point.x() - m_axisRect.left()) * getrange() / m_axisRect.width();
		res = true;
	}
	return res;
}

bool simpledsotimeaxis::ZoomOutPos(QPoint point)
{
	bool res = false;
	if (m_axis_index < GetAxisIntervalMaxIndex())
	{
		double zoomcenter = converpos2value(point.x());
		m_axis_index++;
		m_start = zoomcenter - (point.x() - m_axisRect.left()) * getrange() / m_axisRect.width();
		res = true;
	}
	return res;
}

void simpledsotimeaxis::SetAxisDanwei(Axis::X_AXIS_DANWEI dabwei)
{
	if (dabwei != m_axis_danwei)
	{
		basictimeaxis::SetAxisDanwei(dabwei);
		destroy_aixs_scales();
		gen_aixs_scales();
	}
}
