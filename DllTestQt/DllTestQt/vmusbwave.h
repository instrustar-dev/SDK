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

    Q_PROPERTY(int32_t devState READ getdevState WRITE setdevState NOTIFY devStateChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    //QML_ELEMENT

    static void CALLBACK UsbDevAddCallBack(void* ppara);
    static void CALLBACK UsbDevRemoveCallBack(void* ppara);
    static void CALLBACK UsbDataReadyCallBack(void* ppara);
public:
    explicit VmUsbWave(QObject *parent = nullptr);
    ~VmUsbWave();

    int32_t getdevState() const { return m_devState; };
    void setdevState(int32_t devstate);

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
    void setsample(int32_t sample);
    int32_t getsample();

    int32_t getMaxCaptureLength();
    void setCaptureLength(int32_t kb);
    int32_t getCaptureLength();
    void captureEnable(bool en);

    bool getAcDcCh1();
    void setAcDcCh1(bool ac);
    bool getAcDcCh2();
    void setAcDcCh2(bool ac);

    bool isSupportHardTrigger();
    int32_t getTriggerMode();
    void setTriggerMode(int32_t mode);
    int32_t getTriggerStyle();
    void setTriggerStyle(int32_t style);
    int32_t getTriggerSource();
    void setTriggerSource(int32_t sr);
    int32_t getTriggerLevel();
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
    int32_t m_devState;
    QString mFileName;

    QStringList m_support_samples;
    int32_t m_sample;

    int32_t m_captureLength;
    int32_t m_real_length;
    bool m_captureEnable;

    //double* buffer_ch1;
    //double* buffer_ch2;
    //int32_t buffer_length;

    double m_ch1_range_min, m_ch1_range_max;
    double m_ch2_range_min, m_ch2_range_max;
    double m_ch1_plot_range_min, m_ch1_plot_range_max;
    double m_ch2_plot_range_min, m_ch2_plot_range_max;
};

#endif // VMUSBWAVE_H
