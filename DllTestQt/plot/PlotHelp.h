#pragma once
/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
    www.instrustar.com
*********************************************************************************************************************/


#include <QColor>
#include <QString>

#include "plottype.h"
#include "Axis.h"

//*************************************gdi+******************************

//--------------------------------Value2String---------------------------------------
QString SampleSaStr(unsigned int sample, QString Str_K, QString Str_M, QString Sample_short);

QString PhaseStr(bool display_phase, double xiangwei);

/*------------------------------------------------------- AxisValue----------------------------------------------------*/
class PlotHelp {
public:
    PlotHelp();
	virtual ~PlotHelp();

	//----------------------------------------------------------------
    QString DepthStr(unsigned int depth_length);
	//----------------------------------------------------------------------
    void XAxisValueStr(QString& string_v, Axis::X_AXIS_DANWEI m_disp_danwei, double m_axis_value, char pos_l = 0, char pos_r = 3, bool disp_danwei = true);
	//----------------------------------------------------------------------
    void YAxisValueStr(QString& string_v, Axis::Y_AXIS_DANWEI m_disp_danwei, double m_axis_value, char pos_l = 0, char pos_r = 3, bool disp_danwei = true);
    void YAxisValueStr(QString& string_v, Axis::Y_AXIS_DANWEI m_disp_danwei, double interval, double m_axis_value, char pos_l = 0, char pos_r = 3, bool disp_danwei = true);
    void YAxisValueStr(QString& string_v, Axis::Y_AXIS_DANWEI m_disp_danwei, double m_axis_value, QString& grid);
	//------------------------------------------------------------------
    int SampleStr(char* string, size_t strlength, unsigned int sample, QString Sample);  //考虑2个函数合并
    QString SampleStr(unsigned int sample, QString Sample);
    QString SampleStr(unsigned int sample);

    QString CenterFreq(double sample);

    QString Time2FrequenceStr(double time);
    QString FrequenceStr(double freq);

    QString HarmonicFreqStr(double m_harmonic);

    QString FrequenceMeterStr(QString Frequency_Counter, bool display_hard_freq_meter, bool success, double freq);
    QString FrequenceMeterStr(bool success, double freq);

    QString ValueStrVolt(bool active, double value);
    QString VminStr(QString& LeftStr, bool display_min, double maxv, double minv, double extreme_max, double extreme_min);
    QString VmaxStr(QString& LeftStr, bool display_max, double maxv, double minv, double extreme_max, double extreme_min);
    QString VppStr(QString& LeftStr, bool display_ff, double maxv, double minv, double mrms, double extreme_max, double extreme_min);

    QString RmsStr(QString& LeftStr, bool display_rms, double maxv, double minv, double mrms, double extreme_max, double extreme_min);
    QString VAverageStr(QString& LeftStr, bool display_average, double maxv, double minv, double average, double sdev, double extreme_max, double extreme_min);
    QString VSdevStr(QString& LeftStr, bool display_sdev, double maxv, double minv, double average, double sdev, double extreme_max, double extreme_min);

	//------------------------------------
    QString ValueStrTime(double time, int pos_l);
    QString TimeStr(double time, char pos_l, char pos_r);
    QString TimeStr(int time_properity, double time, char pos_l, char pos_r);

    QString CycleStr(QString& LeftStr, bool display_cycle, double frequence);

private:
    QString m_str_hz;
    QString m_str_khz;
    QString m_str_mhz;

    QString m_str_ns;
    QString m_str_us;
    QString m_str_ms;
    QString m_str_s;

    QString m_str_mv;
    QString m_str_v;
    QString m_str_kv;

    QString m_str_db;
    QString m_str_dbv;
    QString m_str_dbmv;
    QString m_str_dbmw;
    QString m_str_dbu;

    QString m_str_rad;
    QString m_str_deg;

    QString m_str_b;
    QString m_str_kb;
    QString m_str_mb;
};

