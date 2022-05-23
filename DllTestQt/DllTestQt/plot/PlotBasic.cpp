#include "PlotBasic.h"


double basictimeaxis::movex2value(int move) 
{ 
    return move * getrange() / m_axisRect.width();
}

//pos & value
double basictimeaxis::converpos2value(int pos) 
{
    return getminrange() + (pos - m_axisRect.left()) * (getrange()) / m_axisRect.width();
}

int basictimeaxis::convervalue2pos(double value)
{ 
    return (int)(m_axisRect.left() + (value - getminrange()) * m_axisRect.width() / getrange());
}

double basictimeaxis::move2value(int movey) 
{ 
    return (double)movey * getrange() / m_axisRect.width();
}
