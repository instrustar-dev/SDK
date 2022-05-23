#ifndef VMSIMPLEPLOT_H
#define VMSIMPLEPLOT_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
    www.instrustar.com
*********************************************************************************************************************/
#include <QQuickPaintedItem>
#include "SimplePlotBasic.h"
#include "PlotBasic.h"

class VmSimplePlot : public QQuickPaintedItem
{
    Q_OBJECT
    //Q_PROPERTY(QColor backgroud_color READ backgroud_color WRITE setBackgroudColor)
public:
    VmSimplePlot(QQuickItem *parent = 0);
    virtual ~VmSimplePlot();

    //-----------------------------------------------axis------------------------------------------------------
    Q_INVOKABLE bool addAxis(const QString& m_trackname, QColor m_lineclr);
    Q_INVOKABLE void removeAxis(const QString& m_trackname);
    Q_INVOKABLE void destoryAllAxis();

    //更新增减线后，的位置问题  <XP没有初始化界面有问题>
    Q_INVOKABLE void computeAxisPos(){ComputeRects();};
    Q_INVOKABLE double getAxisInterval(int32_t k){return Axis::AxisInterval[k];};
    Q_INVOKABLE void setYAxisRange(const QString& m_trackname, double start, double value);
    Q_INVOKABLE void setTimeAxisRange(double start, double value);

    Q_INVOKABLE void updatDevState(int32_t devstate);
    Q_INVOKABLE void updateDatas(int32_t real_length, int32_t sample/*QVariantList list*/);

    QColor backgroud_color() const;
    void setBackgroudColor(const QColor &color);

    Q_INVOKABLE void redraw(){update();};
    void paint(QPainter *painter);
signals:
    void yAxisRangeChanged(QString trackname, QVariantList list);
    void updateDataSuccess();
protected:
    virtual int GetYMinAxisInterval(Axis::Y_AXIS_DANWEI danwei);
    virtual int GetYMaxAxisInterval(Axis::Y_AXIS_DANWEI danwei);
    virtual int GetTimeMinAxisInterval();
    virtual int GetTimeMaxAxisInterval();

    void GetLeftArrow5Points(QPoint* points, int x, int y, int heigth, int width, int width_arrow, int trigger_arrow_range, QPoint* trigger_points=NULL);
    void GetRightArrow5Points(QPoint* points, int x, int y, int heigth, int width, int width_arrow, int trigger_arrow_range, QPoint* trigger_points=NULL);
    void GetTopArrow5Points(QPoint* points, int x, int y, int heigth, int width, int heigth_arrow, int trigger_arrow_range, QPoint* trigger_points=NULL);
    void GetBottomArrow5Points(QPoint* points, int x, int y, int heigth, int width, int width_arrow, int trigger_arrow_range, QPoint* trigger_points=NULL);

    bool DrawPointsArrow(QPainter* painter, QColor clr, QPoint* points, int point_count);

    virtual bool UpdateLine(QPainter* painter, QPixmap& MemPixmap, QRect& m_rect, double time_min, double time_max, bool full=false);
    virtual bool UpdateLine(QPainter* painter, QPixmap& MemPixmap);

    virtual void ComputeRects();

    virtual bool DrawBackgroud(QPainter *painter);
    virtual bool DrawGrid(QPainter *painter);
    virtual bool DrawYArrows(QPainter *painter);
    virtual bool DrawTimeArrows(QPainter *painter){return true;};
    virtual bool DrawTimeArrowsNotice(QPainter *painter);
    virtual bool OtherDraw(QPainter *mempainter, QPainter *painter);

    void SetRange(double *dstmin, double *dstmax, int *dstproperity, int axis_count, double min, double max);

    //20160527 为了dpi问题，将字体大小统一
    QSize RegularSize;

    QFont RegularFont;
    QFont SmallFont;

    bool m_support_top_info;
    int bootom_axis_disp_num;//低端刻度行数
    bool m_cursor_value_pos_istop;

    //显示信息
    QColor m_infotext_clr;

    PlotHelp m_plothelp;

    QRect m_clientRect;

    advplotarea m_plotarea;
    simpledsotimeaxis m_timeaxis;
    std::vector<simpleaxis*> axisvector;

    //底层数据读取
    double* buffer_ch1;
    double* buffer_ch2;
    int32_t buffer_length;
};

#endif // VMSIMPLEPLOT_H
