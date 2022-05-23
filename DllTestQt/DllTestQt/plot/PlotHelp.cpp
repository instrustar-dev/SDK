#include "PlotHelp.h"
#include "plottype.h"
#include <assert.h>
#include <QObject>

//--------------------------------Value2String---------------------------------------
QString SampleSaStr(unsigned int sample, QString Str_K, QString Str_M, QString Sample_short)
{
    QString temp;
	if(sample>=1000000)
        temp = QString("%1 %2").arg(sample/1000000, 0, 10).arg(Str_M);
	else if(sample>=1000)
        temp = QString("%1 %2").arg(sample/1000, 0, 10).arg(Str_K);
	else
        temp = QString("%1").arg(sample, 0, 10);
	return temp+ Sample_short;
}

QString PhaseStr(bool display_phase, double xiangwei)
{
    QString str = "";
	if (display_phase)
	{
        str = QString("P= %1").arg(xiangwei, 0, 'g', 3);
	}
	return str;
}

PlotHelp::PlotHelp()
    : m_str_hz(QObject::tr("Hz"))
    , m_str_khz(QObject::tr("KHz"))
    , m_str_mhz(QObject::tr("MHz"))

    , m_str_ns(QObject::tr("ns"))
    , m_str_us(QObject::tr("us"))
    , m_str_ms(QObject::tr("ms"))
    , m_str_s(QObject::tr("s"))

    , m_str_mv(QObject::tr("mV"))
    , m_str_v(QObject::tr("V"))
    , m_str_kv(QObject::tr("kV"))

    , m_str_db(QObject::tr("dB"))
    , m_str_dbv(QObject::tr("dBV"))
    , m_str_dbmv(QObject::tr("dBmV"))
    , m_str_dbmw(QObject::tr("dBmW"))
    , m_str_dbu(QObject::tr("dBu"))

    , m_str_rad(QObject::tr("rad"))
    , m_str_deg(QObject::tr("deg"))

    , m_str_b(QObject::tr("B"))
    , m_str_kb(QObject::tr("KB"))
    , m_str_mb(QObject::tr("MB"))
{
}

PlotHelp::~PlotHelp()
{

}

QString PlotHelp::DepthStr(unsigned int depth_length)
{
    QString temp;
	if (depth_length >= 1024 * 1024)
        temp = QString("%1%2").arg(depth_length / 1024 / 1024, 0, 10).arg(m_str_mb);
	else if (depth_length >= 1000 * 1024)
        temp = QString("%1%2").arg(depth_length / 1000 / 1024, 0, 10).arg(m_str_mb);
	else if (depth_length >= 1024)
        temp = QString("%1%2").arg(depth_length / 1024, 0, 10).arg(m_str_kb);
	else if (depth_length >= 1000)
        temp = QString("%1%2").arg(depth_length / 1000, 0, 10).arg(m_str_kb);
	else
        temp = QString("%1%2").arg(depth_length, 0, 10).arg(m_str_b);
	return temp;
}

void PlotHelp::XAxisValueStr(QString& string_v, Axis::X_AXIS_DANWEI m_axis_danwei, double m_axis_value, char pos_l, char pos_r, bool disp_danwei)
{
	if (m_axis_danwei == Axis::X_AXIS_DANWEI::NO)
        string_v = QString("%1").arg(m_axis_value, pos_l, 'f', pos_r);
	else if (m_axis_danwei == Axis::X_AXIS_DANWEI::S)
	{
		if (fabs(m_axis_value) < 1000) //ns
		{
            string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_ns: QString(""));
		}
		else if (fabs(m_axis_value) < 1000000) //us
		{
            string_v = QString("%1%2").arg(m_axis_value / 1000, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_us: QString(""));
		}
		else if (fabs(m_axis_value) < 1000000000) //ms
		{
            string_v = QString("%1%2").arg(m_axis_value / 1000000, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_ms: QString(""));
		}
		else //s
		{
            string_v = QString("%1%2").arg(m_axis_value / 1000000000, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_s: QString(""));
		}
	}
    else if ((m_axis_danwei == Axis::X_AXIS_DANWEI::FREQ))
	{
		if (fabs(m_axis_value) >= 1000000)
		{
            string_v = QString("%1%2").arg(m_axis_value / 1000000.0, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_mhz: QString(""));
		}
		else if (fabs(m_axis_value) >= 1000)
		{
            string_v = QString("%1%2").arg(m_axis_value / 1000.0, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_khz: QString(""));
		}
		else
		{
            string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_hz: QString(""));
		}
	}
	else if (m_axis_danwei == Axis::X_AXIS_DANWEI::VOLTAGE)
	{
		if (fabs(m_axis_value) >= 1000.0) //V
            string_v = QString("%1%2").arg(m_axis_value/1000.0, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_kv: QString(""));
		else if (fabs(m_axis_value) >= 1.0) //V
            string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_v: QString(""));
		else
            string_v = QString("%1%2").arg(m_axis_value * 1000.0, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_mv: QString(""));
	}
}

void PlotHelp::YAxisValueStr(QString& string_v, Axis::Y_AXIS_DANWEI m_disp_danwei, double m_axis_value, char pos_l, char pos_r, bool disp_danwei)
{
	if (m_disp_danwei == Axis::Y_AXIS_DANWEI::VOLTAGE)
	{
		if (fabs(m_axis_value) >= 1000.0) //V
            string_v = QString("%1%2").arg(m_axis_value / 1000.0, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_kv: QString(""));
		else if (fabs(m_axis_value) >= 1.0) //V
            string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_v: QString(""));
		else
            string_v = QString("%1%2").arg(m_axis_value* 1000.0, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_mv: QString(""));
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB)
	{
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_db: QString(""));
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB_V)
	{
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_dbv: QString(""));
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB_MV)
	{
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_dbmv: QString(""));
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB_M)
	{
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_dbmw: QString(""));
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB_U)
	{
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_dbu: QString(""));
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::RAD)
    {
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_rad: QString(""));
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DEG)
	{
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_deg: QString(""));
	}
	else
	{
        string_v = QString("%1").arg(m_axis_value, pos_l, 'f', pos_r);
	}
}

void PlotHelp::YAxisValueStr(QString& string_v, Axis::Y_AXIS_DANWEI m_disp_danwei, double interval, double m_axis_value, char pos_l, char pos_r, bool disp_danwei)
{
	if (m_disp_danwei == Axis::Y_AXIS_DANWEI::VOLTAGE)
	{
		if (fabs(interval) >= 1000.0) //V
            string_v = QString("%1%2").arg(m_axis_value / 1000.0, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_kv: QString(""));
		else if (fabs(interval) >= 1.0) //V
            string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_v: QString(""));
		else
            string_v = QString("%1%2").arg(m_axis_value* 1000.0, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_mv: QString(""));
	}
    else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB)
    {
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_db: QString(""));
    }
    else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB_V)
    {
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_dbv: QString(""));
    }
    else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB_MV)
    {
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_dbmv: QString(""));
    }
    else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB_M)
    {
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_dbmw: QString(""));
    }
    else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB_U)
    {
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_dbu: QString(""));
    }
    else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::RAD)
    {
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_rad: QString(""));
    }
    else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DEG)
    {
        string_v = QString("%1%2").arg(m_axis_value, pos_l, 'f', pos_r).arg(disp_danwei ? m_str_deg: QString(""));
    }
    else
    {
        string_v = QString("%1").arg(m_axis_value, pos_l, 'f', pos_r);
    }
}

void PlotHelp::YAxisValueStr(QString& string_v, Axis::Y_AXIS_DANWEI m_disp_danwei, double m_axis_value, QString& grid)
{
	if (m_disp_danwei == Axis::Y_AXIS_DANWEI::VOLTAGE)
	{
		if (fabs(m_axis_value) >= 1000.0) //V
            string_v = QString("%1 %2/%3").arg(m_axis_value / 1000.0, 0, 'f', 0).arg(m_str_kv).arg(grid);
		else if (fabs(m_axis_value) >= 1.0) //V
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 0).arg(m_str_v).arg(grid);
		else if (fabs(m_axis_value) >= 0.001)
            string_v = QString("%1 %2/%3").arg(m_axis_value * 1000.0, 0, 'f', 0).arg(m_str_mv).arg(grid);
		else
            string_v = QString("%1 %2/%3").arg(m_axis_value * 1000.0, 0, 'f', 3).arg(m_str_mv).arg(grid);
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB)
	{
		if (fabs(m_axis_value) >= 1.0)
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 0).arg(m_str_db).arg(grid);
        else
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 3).arg(m_str_db).arg(grid);
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB_V)
	{
        if (fabs(m_axis_value) >= 1.0)
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 0).arg(m_str_dbv).arg(grid);
        else
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 3).arg(m_str_dbv).arg(grid);
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB_MV)
	{
        if (fabs(m_axis_value) >= 1.0)
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 0).arg(m_str_dbmv).arg(grid);
        else
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 3).arg(m_str_dbmv).arg(grid);
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB_M)
	{
        if (fabs(m_axis_value) >= 1.0)
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 0).arg(m_str_dbmw).arg(grid);
        else
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 3).arg(m_str_dbmw).arg(grid);
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DB_U)
	{
        if (fabs(m_axis_value) >= 1.0)
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 0).arg(m_str_dbu).arg(grid);
        else
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 3).arg(m_str_dbu).arg(grid);
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::RAD)
	{
        if (fabs(m_axis_value) >= 1.0)
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 0).arg(m_str_rad).arg(grid);
        else
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 3).arg(m_str_rad).arg(grid);
	}
	else if (m_disp_danwei == Axis::Y_AXIS_DANWEI::DEG)
	{
        if (fabs(m_axis_value) >= 1.0)
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 0).arg(m_str_deg).arg(grid);
        else
            string_v = QString("%1 %2/%3").arg(m_axis_value, 0, 'f', 3).arg(m_str_deg).arg(grid);
	}
	else
	{
        if (fabs(m_axis_value) >= 1.0)
            string_v = QString("%1/%3").arg(m_axis_value, 0, 'f', 0).arg(grid);
        else
            string_v = QString("%1/%3").arg(m_axis_value, 0, 'f', 3).arg(grid);
	}
}
/*
int PlotHelp::SampleStr(char* string, size_t strlength, unsigned int sample, std::string Sample)
{
	int index = 0;
	if (sample >= 1000000)
		sprintf_s(string, strlength, "%s %d %s", Sample.c_str(), sample / 1000000, m_str_mhz.c_str());
	else if (sample >= 1000)
		sprintf_s(string, strlength, "%s %d %s", Sample.c_str(), sample / 1000, m_str_khz.c_str());
	else
		sprintf_s(string, strlength, "%s %d %s", Sample.c_str(), sample, m_str_hz.c_str());
	index = strlen(string);
	return index;
}

QString PlotHelp::SampleStr(unsigned int sample, QString Sample)
{
    QString temp;
	if (sample >= 1000000)
		temp.Format("%s %d %s", Sample, sample / 1000000, m_str_mhz.c_str());
	else if (sample >= 1000)
		temp.Format("%s %d %s", Sample, sample / 1000, m_str_khz.c_str());
	else
		temp.Format("%s %d %s", Sample, sample, m_str_hz.c_str());
	return temp;
}

QString PlotHelp::SampleStr(unsigned int sample)
{
    QString temp;
	if (sample >= 1000000)
		temp.Format("%d %s", sample / 1000000, m_str_mhz.c_str());
	else if (sample >= 1000)
		temp.Format("%d %s", sample / 1000, m_str_khz.c_str());
	else
		temp.Format("%d %s", sample, m_str_hz.c_str());
	return temp;
}

QString PlotHelp::CenterFreq(double m_center)
{
    QString str;
	if (fabs(m_center) >= 1000000)
		str.Format("%0.6f %s", m_center / 1000000.0, m_str_mhz.c_str());
	else if (fabs(m_center) >= 1000)
		str.Format("%0.6f %s", m_center / 1000.0, m_str_khz.c_str());
	else
		str.Format("%0.6f %s", m_center, m_str_hz.c_str());
	return str;
}

QString PlotHelp::Time2FrequenceStr(double time)
{
    QString temp = "";
	const double frequence = (time == 0 ? 0 : 1000000000.0 / time);
	if (fabs(frequence) > 1000000)
		temp.Format("%0.3f %s", frequence / 1000000.0, m_str_mhz.c_str());
	else if (fabs(frequence) > 1000)
		temp.Format("%0.3f %s", frequence / 1000.0, m_str_khz.c_str());
	else
		temp.Format("%0.3f %s", frequence, m_str_hz.c_str());
	return temp;
}

QString PlotHelp::FrequenceStr(double frequence)
{
    QString temp = "";
	if (fabs(frequence) >= 1000000)
		temp.Format("%0.3f %s", frequence / 1000000.0, m_str_mhz.c_str());
	else if (fabs(frequence) >= 1000)
		temp.Format("%0.3f %s", frequence / 1000.0, m_str_khz.c_str());
	else
		temp.Format("%0.3f %s", frequence, m_str_hz.c_str());
	return temp;
}

QString PlotHelp::HarmonicFreqStr(double m_harmonic)
{
    QString str = "";
	if (m_harmonic == 0)
		str = "********         ";
	else if (m_harmonic >= 1000000)
		str.Format("%3.4f %s  ", m_harmonic / 1000000, m_str_mhz.c_str());
	else if (m_harmonic >= 1000)
		str.Format("%3.4f %s  ", m_harmonic / 1000, m_str_khz.c_str());
	else
		str.Format("%3.4f %s  ", m_harmonic, m_str_hz.c_str());
	return str;
}

QString PlotHelp::FrequenceStr(bool display_soft_freq_meter, bool display_phase, NEICHA_STYLE neicha, double frequence, double xiangwei, QString Interpolation, QString Sine, QString Spline)
{
    QString str = "";
	if (display_soft_freq_meter)
	{
		if (neicha == ZHENGXUAN)
			str.Format("%s:%s  ", Interpolation, Sine);
		else if (neicha == SPLINE3)
			str.Format("%s:%s  ", Interpolation, Spline);

        QString temp;
		if (frequence > 1000000)
			temp.Format("F= %0.3f %s  ", frequence / 1000000.0, m_str_mhz.c_str());
		else if (frequence > 1000)
			temp.Format("F= %0.3f %s  ", frequence / 1000.0, m_str_khz.c_str());
		else
			temp.Format("F= %0.3f %s  ", frequence, m_str_hz.c_str());
		str = str + temp;
	}
	if (display_phase)
	{
        QString temp;
		if (frequence > 1000000)
			temp.Format("P= %0.0f", xiangwei);
		else if (frequence > 1000)
			temp.Format("P= %0.0f", xiangwei);
		else
			temp.Format("P= %0.0f", xiangwei);
		str = str + temp;
	}
	return str;
}

QString PlotHelp::FrequenceMeterStr(QString Frequency_Counter, bool display_hard_freq_meter, bool success, double freq)
{
    QString temp = "";
	if (display_hard_freq_meter)
	{
		if (success)
		{
			if (freq >= 1000000)
				temp.Format(" F = %0.6f %s", freq / 1000000, m_str_mhz.c_str());
			else if (freq >= 1000)
				temp.Format(" F = %0.6f %s", freq / 1000, m_str_khz.c_str());
			else if (freq >= 5)
				temp.Format(" F = %0.6f %s", freq, m_str_hz.c_str());
			else
				temp.Format(" F < 5 %s", m_str_hz.c_str());
		}
		else
			temp.Format(" F < 5 %s", m_str_hz.c_str());
		temp = Frequency_Counter + temp;
	}
	return temp;
}

QString PlotHelp::FrequenceMeterStr(bool success, double freq)
{
    QString temp;
	temp.Format(" F < 5 %s", m_str_hz.c_str());
	if (success)
	{
		if (freq >= 1000000)
			temp.Format("%0.6f %s", freq / 1000000, m_str_mhz.c_str());
		else if (freq >= 1000)
			temp.Format("%0.6f %s", freq / 1000, m_str_khz.c_str());
		else if (freq >= 5)
			temp.Format("%0.6f %s", freq, m_str_hz.c_str());
		//else
		//	temp="F < 5 Hz";
	}
	return temp;
}

QString PlotHelp::FrequenceStr(bool display_soft_freq_meter, NEICHA_STYLE neicha, double frequence)
{
    QString str = "";
	if (display_soft_freq_meter)
	{
        QString temp;
		if (frequence > 1000000)
			temp.Format("F= %0.3f %s  ", frequence / 1000000.0, m_str_mhz.c_str());
		else if (frequence > 1000)
			temp.Format("F= %0.3f %s  ", frequence / 1000.0, m_str_khz.c_str());
		else
			temp.Format("F= %0.3f %s  ", frequence, m_str_hz.c_str());
		str = str + temp;
	}
	return str;
}

QString PlotHelp::ValueStrVolt(bool active, double value)
{
    QString str = "";
	if (active)
	{
		if (fabs(value) >= 1000)
			str.Format("%0.3f %s", value / 1000.0, m_str_kv.c_str());
		else if (fabs(value) > 1)
			str.Format("%0.3f %s", value, m_str_v.c_str());
		else
			str.Format("%0.3f %s", value * 1000.0, m_str_mv.c_str());
	}
	else
		str = "****";
	return str;
}

QString PlotHelp::VminStr(QString& LeftStr, bool display_min, double maxv, double minv, double extreme_max, double extreme_min)
{
    QString str = "";
	if (display_min)
		str = LeftStr + "= " + ValueStrVolt(!((minv == extreme_min) || ((minv == maxv) && (maxv == extreme_max))), minv) + "  ";
	return str;
}

QString PlotHelp::VmaxStr(QString& LeftStr, bool display_max, double maxv, double minv, double extreme_max, double extreme_min)
{
    QString str = "";
	if (display_max)
		str = LeftStr + "= " + ValueStrVolt(!((maxv == extreme_max) || ((minv == maxv) && (minv == extreme_min))), maxv) + "  ";
	return str;
}

QString PlotHelp::VppStr(QString& LeftStr, bool display_ff, double maxv, double minv, double mrms, double extreme_max, double extreme_min)
{
    QString str = "";
	if (display_ff)
		str = LeftStr + "= " + ValueStrVolt(!((maxv == extreme_max) || (minv == extreme_min)), maxv - minv) + "  ";
	return str;
}

QString PlotHelp::RmsStr(QString& LeftStr, bool display_rms, double maxv, double minv, double mrms, double extreme_max, double extreme_min)
{
    QString str = "";
	if (display_rms)
		str = LeftStr + "= " + ValueStrVolt(!((maxv == extreme_max) || (minv == extreme_min)), mrms) + "  ";
	return str;
}

QString PlotHelp::VAverageStr(QString& LeftStr, bool display_average, double maxv, double minv, double average, double sdev, double extreme_max, double extreme_min)
{
    QString str = "";
	if (display_average)
		str = LeftStr + "= " + ValueStrVolt(!((maxv == extreme_max) || (minv == extreme_min)), average) + "  ";
	return str;
}

QString PlotHelp::VSdevStr(QString& LeftStr, bool display_sdev, double maxv, double minv, double average, double sdev, double extreme_max, double extreme_min)
{
    QString str = "";
	if (display_sdev)
		str = LeftStr + "= " + ValueStrVolt(!((maxv == extreme_max) || (minv == extreme_min)), sdev) + "  ";
	return str;
}

QString PlotHelp::CmeanStr(QString& LeftStr, bool display_cycle_mean, double maxv, double minv, double cmean, double extreme_max, double extreme_min)
{
    QString str = "";
	if (display_cycle_mean)
		str = LeftStr + "= " + ValueStrVolt(!((maxv == extreme_max) || (minv == extreme_min)), cmean) + "  ";
	return str;
}

QString PlotHelp::CRmsStr(QString& LeftStr, bool display_cycle_rms, double maxv, double minv, double crms, double extreme_max, double extreme_min)
{
    QString str = "";
	if (display_cycle_rms)
		str = LeftStr + "= " + ValueStrVolt(!((maxv == extreme_max) || (minv == extreme_min)), crms) + "  ";
	return str;
}

QString PlotHelp::CsdevStr(QString& LeftStr, bool display_cycle_sdev, double maxv, double minv, double csdev, double extreme_max, double extreme_min)
{
    QString str = "";
	if (display_cycle_sdev)
		str = LeftStr + "= " + ValueStrVolt(!((maxv == extreme_max) || (minv == extreme_min)), csdev) + "  ";
	return str;
}

QString PlotHelp::VbaseStr(QString& LeftStr, bool display_base, en_value topv, en_value basev)
{
    QString str = "";
	if (display_base)
		str = LeftStr + "= " + ValueStrVolt(basev.m_active, basev.m_value) + "  ";
	return str;
}

QString PlotHelp::VtopStr(QString& LeftStr, bool display_top, en_value topv, en_value basev)
{
    QString str = "";
	if (display_top)
		str = LeftStr + "= " + ValueStrVolt(topv.m_active, topv.m_value) + "  ";
	return str;
}

QString PlotHelp::VAmplStr(QString& LeftStr, bool display_ampl, en_value topv, en_value basev)
{
    QString str = "";
	if (display_ampl)
		str = LeftStr + "= " + ValueStrVolt(topv.m_active && basev.m_active, topv.m_value - basev.m_value) + "  ";
	return str;
}

QString PlotHelp::ValueStrTime(double time, int pos_l)
{
    QString str;
	if (fabs(time) < 1000)
		str.Format("%0.*f %s", pos_l, time, m_str_ns.c_str());
	else if (fabs(time) < 1000000)
		str.Format("%0.*f %s", pos_l, time / 1000.0, m_str_us.c_str());
	else if (fabs(time) < 1000000000)
		str.Format("%0.*f %s", pos_l, time / 1000000.0, m_str_ms.c_str());
	else
		str.Format("%0.*f %s", pos_l, time / 1000000000.0, m_str_s.c_str());
	return str;
}

QString PlotHelp::TimeStr(double time, char pos_l, char pos_r)
{
    QString str;
	if (fabs(time) < 1000) //ns
		str.Format("%*.*f %s", pos_l, pos_r, time, m_str_ns.c_str());
	else if (fabs(time) < 1000000) //us
		str.Format("%*.*f %s", pos_l, pos_r, time / 1000, m_str_us.c_str());
	else if (fabs(time) < 1000000000) //ms
		str.Format("%*.*f %s", pos_l, pos_r, time / 1000000, m_str_ms.c_str());
	else //s
		str.Format("%*.*f %s", pos_l, pos_r, time / 1000000000, m_str_s.c_str());
	return str;
}

QString PlotHelp::TimeStr(int time_properity, double time, char pos_l, char pos_r)
{
	double timediv;
    QString str, daiweititle;
	if (time_properity >= Axis::AxisIntervalCenter && time_properity <= 8 + Axis::AxisIntervalCenter)
	{
		daiweititle = m_str_ns.c_str();
		timediv = 1;
	}
	else if (time_properity >= 9 + Axis::AxisIntervalCenter && time_properity <= 17 + Axis::AxisIntervalCenter)
	{
		daiweititle = m_str_us.c_str();
		timediv = 1000.0;
	}
	else if (time_properity >= 18 + Axis::AxisIntervalCenter && time_properity <= 26 + Axis::AxisIntervalCenter)
	{
		daiweititle = m_str_ms.c_str();
		timediv = 1000000.0;
	}
	else if (time_properity >= 27 + Axis::AxisIntervalCenter)
	{
		daiweititle = m_str_s.c_str();
		timediv = 1000000000.0;
	}
	else
		assert(false);

	str.Format("%*.*f %s", pos_l, pos_r, time / timediv, daiweititle);
	return str;
}

QString PlotHelp::CycleStr(QString& LeftStr, bool display_cycle, double frequence)
{
    QString str = "";
	if (display_cycle)
	{
		str = LeftStr + "= " + ValueStrTime(frequence == 0 ? 0 : 1000000000.0 / frequence, 3);
	}
	return str;
}

QString PlotHelp::PulsePositiveWidthStr(QString& LeftStr, bool display_positive_pulse_width, en_value positive_pulse_width)
{
    QString str = "";
	if (display_positive_pulse_width)
		str = LeftStr + "= " + (positive_pulse_width.m_active ? ValueStrTime(positive_pulse_width.m_value, 2) : "****");
	return str;
}

QString PlotHelp::PulseNegativeWidthStr(QString& LeftStr, bool display_negative_pulse_width, en_value negative_pulse_width)
{
    QString str = "";
	if (display_negative_pulse_width)
		str = LeftStr + "= " + (negative_pulse_width.m_active ? ValueStrTime(negative_pulse_width.m_value, 2) : "****");
	return str;
}

QString PlotHelp::DutyPositiveStr(QString& LeftStr, bool display_positive_duty, en_value positive_duty)
{
    QString str = "";
	if (display_positive_duty)
		positive_duty.m_active ? str.Format("%s= %0.1f%%", LeftStr, positive_duty.m_value) : str = LeftStr+"= ****  ";
	return str;
}

QString PlotHelp::DutyNegativeStr(QString& LeftStr, bool display_negative_duty, en_value negative_duty)
{
    QString str = "";
	if (display_negative_duty)
		negative_duty.m_active ? str.Format("%s= %0.1f%%", LeftStr, negative_duty.m_value) : str = LeftStr+"= ****  ";
	return str;
}*/
