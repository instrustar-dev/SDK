#ifndef VMUSBWAVE_H
#define VMUSBWAVE_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
    www.instrustar.com
*********************************************************************************************************************/

#include <QVariant>
#include <QObject>
#include "VdsoLib.h"

class VmUsbWave : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int devState READ getdevState WRITE setdevState NOTIFY devStateChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    //QML_ELEMENT

    static void CALLBACK UsbDevAddCallBack(void* ppara);
    static void CALLBACK UsbDevRemoveCallBack(void* ppara);
    static void CALLBACK UsbDataReadyCallBack(void* ppara);
public:
    explicit VmUsbWave(QObject *parent = nullptr);
    ~VmUsbWave();

    int getdevState() const { return m_devState; };
    void setdevState(int devstate);

    QString fileName() const { return mFileName; }
    void setFileName(QString fileName);

    Q_INVOKABLE void qmlcallcpp_style1();
    Q_INVOKABLE void setPlotRangeCh1(QVariantList list);
    Q_INVOKABLE void setPlotRangeCh2(QVariantList list);
    Q_INVOKABLE void nextCapture();
public Q_SLOTS:
    void qmlcallcpp_style2(const int8_t chn);

    void resetDll();
    void resetDevice();

    QStringList getsupportsamples();
    void setsample(int sample);
    int getsample();

    int getMaxCaptureLength();
    void setCaptureLength(int kb);
    int getCaptureLength();
    void captureEnable(bool en);

    bool getAcDcCh1();
    void setAcDcCh1(bool ac);
    bool getAcDcCh2();
    void setAcDcCh2(bool ac);

    bool isSupportHardTrigger();
    int getTriggerMode();
    void setTriggerMode(int mode);
    int getTriggerStyle();
    void setTriggerStyle(int style);
    int getTriggerSource();
    void setTriggerSource(int sr);
    int getTriggerLevel();
    void setTriggerLevel(int level);
signals:
    void fileNameChanged();
    void devStateChanged(int devstate);

    void updateDatas(int real_length, int sample/*QVariantList list*/);
private:
    void UsbDataProcess();
    void DisplayZoomCtrlCh1(bool start = false);
    void DisplayZoomCtrlCh2(bool start = false);
private:
    int m_devState;
    QString mFileName;

    QStringList m_support_samples;
    int m_sample;

    int m_captureLength;
    int m_real_length;
    bool m_captureEnable;

    //double* buffer_ch1;
    //double* buffer_ch2;
    //int buffer_length;

    double m_ch1_range_min, m_ch1_range_max;
    double m_ch2_range_min, m_ch2_range_max;
    double m_ch1_plot_range_min, m_ch1_plot_range_max;
    double m_ch2_plot_range_min, m_ch2_plot_range_max;
};

#endif // VMUSBWAVE_H
