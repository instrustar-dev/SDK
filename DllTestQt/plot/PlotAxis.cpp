#include "PlotAxis.h"
#include "PlotHelp.h"
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------
void simpleaxis::SetAxisDanwei(Axis::Y_AXIS_DANWEI dabwei, int IntervalMinIndex, int IntervalMaxIndex)
{
	AxisIntervalMinIndex = IntervalMinIndex;
	AxisIntervalMaxIndex = IntervalMaxIndex;

	if (dabwei != m_disp_danwei)
	{
		m_disp_danwei = dabwei;
	}
}

double simpleaxis::axisindex2value(int index)
{ 
    return Axis::AxisInterval[index];
}

int simpleaxis::value2axisindex(double value)
{
	int i = AxisIntervalMinIndex;
	for (i; i <= AxisIntervalMaxIndex; i++)
	{
		if (Axis::AxisInterval[i] >= value)
			break;
	}
	return i;
}

void simpleaxis::setproperityvalue(double start, double value)
{
    m_axis_index = value2axisindex(value);
    setstart(start);
}

//zoom
bool simpleaxis::VerZoomInPos(int zoompos)
{
	bool result = false;
	if (m_axis_index > AxisIntervalMinIndex)
	{
		const double zoomcenter = converpos2value(zoompos);
		m_axis_index--;
        setstart(zoomcenter - (m_axisRect.bottom() - zoompos) * getrange() / m_axisRect.height());
		result = true;
	}
	return result;
}

bool simpleaxis::VerZoomOutPos(int zoompos)
{
	bool result = false;
	if (m_axis_index < AxisIntervalMaxIndex)
	{
		const double zoomcenter = converpos2value(zoompos);
		m_axis_index++;
        setstart(zoomcenter - (m_axisRect.bottom() - zoompos) * getrange() / m_axisRect.height());
		result = true;
	}
	return result;
}

double simpleaxis::converpos2value(int pos)
{
    return getstart() + (m_axisRect.bottom() - pos) * (getrange()) / m_axisRect.height();
}

int simpleaxis::convervalue2pos(double value)
{
    return (int)(m_axisRect.bottom()- (value - getstart()) * m_axisRect.height() / getrange());
}

double simpleaxis::movey2value(int movey)
{
    return (double)movey * getrange() / m_axisRect.height();
}

