#include "vmusbwave.h"
#include <QDebug>

VmUsbWave::VmUsbWave(QObject *parent)
    : QObject{parent}
    , m_devState(0)
    , m_captureEnable(false)
    , m_captureLength(0)
    , m_real_length(0)
    , m_sample(0)
    //, buffer_ch1(NULL)
    //, buffer_ch2(NULL)
    //, buffer_length(0)
    , m_ch1_range_min(0)
    , m_ch1_range_max(0)
    , m_ch2_range_min(0)
    , m_ch2_range_max(0)
    , m_ch1_plot_range_min(0)
    , m_ch1_plot_range_max(0)
    , m_ch2_plot_range_min(0)
    , m_ch2_plot_range_max(0)
{
     InitDll();
     SetDevNoticeCallBack(this, UsbDevAddCallBack, UsbDevRemoveCallBack);
     SetDataReadyCallBack(this, UsbDataReadyCallBack);
}

VmUsbWave::~VmUsbWave()
{
    //if(buffer_ch1!=NULL)
    //    delete [] buffer_ch1;
    //if(buffer_ch2!=NULL)
    //    delete [] buffer_ch2;
    FinishDll();
}

void CALLBACK VmUsbWave::UsbDevAddCallBack(void* ppara)
{
    VmUsbWave* vmusbwave = (VmUsbWave*)ppara;
    vmusbwave->m_captureLength = GetMemoryLength();
    vmusbwave->m_sample = GetOscSample();

    /*if(vmusbwave->buffer_length!=vmusbwave->m_captureLength)
    {
        if(vmusbwave->buffer_ch1!=NULL)
            delete [] vmusbwave->buffer_ch1;
        vmusbwave->buffer_ch1 = new double[vmusbwave->m_captureLength*1024];
        if(vmusbwave->buffer_ch2!=NULL)
            delete [] vmusbwave->buffer_ch2;
        vmusbwave->buffer_ch2 = new double[vmusbwave->m_captureLength*1024];
        vmusbwave->buffer_length=vmusbwave->m_captureLength;
    }*/
    vmusbwave->setdevState(1);
    qInfo("UsbDevAddCallBack\n");
}

void CALLBACK VmUsbWave::UsbDevRemoveCallBack(void* ppara)
{
    VmUsbWave* vmusbwave = (VmUsbWave*)ppara;
    vmusbwave->setdevState(-1);
    qInfo("UsbDevRemoveCallBack\n");
}

void VmUsbWave::setdevState(int devstate)
{
    m_devState = devstate;
    emit devStateChanged(devstate);
}

void CALLBACK VmUsbWave::UsbDataReadyCallBack(void* ppara)
{
    //qDebug()<<"UsbDataReadyCallBack";
    VmUsbWave* vmusbwave = (VmUsbWave*)ppara;
    vmusbwave->UsbDataProcess();
}

 void VmUsbWave::UsbDataProcess()
 {
     /*int length_ch1 = ReadVoltageDatas(0, buffer_ch1, m_real_length);
     int length_ch2 = ReadVoltageDatas(0, buffer_ch2, m_real_length);
     //qDebug()<<"ReadVoltageDatas "<< length_ch1 <<" "<<length_ch2;
     int outrange_ch1 = IsVoltageDatasOutRange(0);
     int outrange_ch2 = IsVoltageDatasOutRange(1);
     //qDebug()<<" is outrange "<< outrange_ch1 <<" "<<outrange_ch2;

      //将采集点数换算成时间ns
     double timelength = min(length_ch1,length_ch2)*1000000000.0/m_sample;

     //qInfo() << buffer_ch1 << " " << length_ch1 << " " << buffer_ch2 << " " << length_ch2 << " " << timelength;
     QVariantList list;
     list << QVariant::fromValue(buffer_ch1) << length_ch1 << QVariant::fromValue(buffer_ch2) << length_ch2 << timelength;
    */
     emit updateDatas(m_real_length, m_sample);

     //NextCapture();
 }

void VmUsbWave::qmlcallcpp_style1()
{
    qDebug()<<"qmlcallcpp_style1";
}

#include <thread>

void VmUsbWave::qmlcallcpp_style2(const int8_t chn)
{
    qDebug()<<"qmlcallcpp_style2";
}

void VmUsbWave::resetDll()
{
    //销毁Dll
    FinishDll();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //初始化Dll
    InitDll();
    SetDevNoticeCallBack(this, UsbDevAddCallBack, UsbDevRemoveCallBack);
    SetDataReadyCallBack(this, UsbDataReadyCallBack);
}

void VmUsbWave::resetDevice()
{
    ResetDevice();
}

QStringList VmUsbWave::getsupportsamples()
{
    m_support_samples.clear();
    int num = GetOscSupportSampleNum();
    unsigned int* samples = new unsigned int[num];
    num = GetOscSupportSamples(samples, num);
    if(num)
    {
        for(int i=0; i<num; i++)
            m_support_samples.push_back(QString::number(samples[i]));
    }
    delete []samples;
    return m_support_samples;
}

void VmUsbWave::setsample(int s)
{
    m_sample = s;
    SetOscSample(m_sample);
    qDebug()<<"setsample " << s;
}

int VmUsbWave::getsample()
{
    return m_sample;
}

int VmUsbWave::getMaxCaptureLength()
{
    return GetMemoryLength();
}

void VmUsbWave::setCaptureLength(int kb)
{
    m_captureLength = kb;
}

int VmUsbWave::getCaptureLength()
{
    return m_captureLength;
}

void VmUsbWave::captureEnable(bool en)
{
    m_captureEnable=en;
    if(m_captureEnable)
    {
        DisplayZoomCtrlCh1(true);
        DisplayZoomCtrlCh2(true);
        nextCapture();
    }
    //qDebug()<<"captureEnable " << en;
}

void VmUsbWave::nextCapture()
{
    if (m_captureEnable)
    {
        DisplayZoomCtrlCh1();
        DisplayZoomCtrlCh2();

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        m_real_length = Capture(m_captureLength, 0);
        m_real_length *= 1024;  //转换成长度
    }
}

void VmUsbWave::setFileName(QString fileName)
{
    mFileName = fileName;
    emit fileNameChanged();
}

bool VmUsbWave::getAcDcCh1()
{
    return GetAcDc(0);
}

void VmUsbWave::setAcDcCh1(bool ac)
{
    SetAcDc(0, ac? 1:0);
}

bool VmUsbWave::getAcDcCh2()
{
    return GetAcDc(1);
}

void VmUsbWave::setAcDcCh2(bool ac)
{
    SetAcDc(1, ac? 1:0);
}

bool VmUsbWave::isSupportHardTrigger()
{
    return IsSupportHardTrigger();
}

int VmUsbWave::getTriggerMode()
{
    return GetTriggerMode();
}

void VmUsbWave::setTriggerMode(int mode)
{
    SetTriggerMode(mode);
}

static unsigned int trigger_index[]={0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100};

int VmUsbWave::getTriggerStyle()
{
    unsigned int style = GetTriggerStyle();

    int i=0;
    for(int k=0; k<sizeof(trigger_index)/sizeof(int); k++)
    {
        if(style==trigger_index[k])
        {
            i=k;
            break;
        }
    }
    return i;
}

void VmUsbWave::setTriggerStyle(int style)
{
    SetTriggerStyle(trigger_index[style]);
}

int VmUsbWave::getTriggerSource()
{
    return GetTriggerSource();
}

void VmUsbWave::setTriggerSource(int sr)
{
    SetTriggerSource(sr);
}

int VmUsbWave::getTriggerLevel()
{
    return GetTriggerLevel();
}

void VmUsbWave::setTriggerLevel(int level)
{
    SetTriggerLevel(level);
}

void VmUsbWave::setPlotRangeCh1(QVariantList list)
{
    if(list.size()==2)
    {
        m_ch1_plot_range_min = list.at(0).toDouble()*1000;
        m_ch1_plot_range_max = list.at(1).toDouble()*1000;
        qInfo() << "setPlotRangeCh1 " << m_ch1_plot_range_min << " " << m_ch1_plot_range_max;
    }
}

void VmUsbWave::setPlotRangeCh2(QVariantList list)
{
    if(list.size()==2)
    {
        m_ch2_plot_range_min = list.at(0).toDouble()*1000;
        m_ch2_plot_range_max = list.at(1).toDouble()*1000;
        qInfo() << "setPlotRangeCh2 " << m_ch2_plot_range_min << " " << m_ch2_plot_range_max;
    }
}

void VmUsbWave::DisplayZoomCtrlCh1(bool start)
{
    if (start||((m_ch1_plot_range_min != m_ch1_range_min) || (m_ch1_plot_range_max != m_ch1_range_max)))
    {
        qInfo() << m_ch1_plot_range_min << " " << m_ch1_plot_range_max << " " << m_ch1_range_min << " " << m_ch1_range_max;
        SetOscChannelRange(0, m_ch1_plot_range_min, m_ch1_plot_range_max);
        m_ch1_range_min = m_ch1_plot_range_min;
        m_ch1_range_max = m_ch1_plot_range_max;
    }
}

void VmUsbWave::DisplayZoomCtrlCh2(bool start)
{
    if (start||((m_ch2_range_min != m_ch2_plot_range_min) || (m_ch2_range_max != m_ch2_plot_range_max)))
    {
        qInfo() << m_ch2_plot_range_min << " " << m_ch2_plot_range_max << " " << m_ch2_range_min << " " << m_ch2_range_max;
        SetOscChannelRange(1, m_ch2_plot_range_min, m_ch2_plot_range_max);
        m_ch2_range_min = m_ch2_plot_range_min;
        m_ch2_range_max = m_ch2_plot_range_max;
    }
}

#include "moc_vmusbwave.cpp"
