#include "VmSimplePlot.h"
#include <QGuiApplication>
#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>
#include "VdsoLib.h"

VmSimplePlot::VmSimplePlot(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_timeaxis(Axis::X_AXIS_DANWEI::S, GetTimeMinAxisInterval(), GetTimeMaxAxisInterval())
    , RegularSize(16,16)
    , RegularFont(qApp->font())
    , SmallFont(qApp->font())
    , m_support_top_info(true)
    , bootom_axis_disp_num(0)
    , m_cursor_value_pos_istop(true)
    , m_infotext_clr(SIMPLE_FACE_FONT)
    , m_clientRect(0,0,0,0)
    , buffer_ch1(NULL)
    , buffer_ch2(NULL)
    , buffer_length(0)
{

}

VmSimplePlot::~VmSimplePlot()
{
    if(buffer_ch1!=NULL)
        delete [] buffer_ch1;
    if(buffer_ch2!=NULL)
        delete [] buffer_ch2;
    destoryAllAxis();
}

//-----------------------------------------------axis------------------------------------------------------
bool VmSimplePlot::addAxis(const QString& m_trackname, QColor m_lineclr)
{
    for (auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        if((*Vector_Iter)->m_trackname==m_trackname)
            return false;
    }
    Axis::Y_AXIS_DANWEI disp_danwei =  Axis::Y_AXIS_DANWEI::VOLTAGE;
    simpleaxis *axis=new simpleaxis(m_trackname, m_lineclr, Qt::SolidLine, 1, disp_danwei, GetYMinAxisInterval(disp_danwei), GetYMaxAxisInterval(disp_danwei));
    axisvector.push_back(axis);
    return true;
}

void VmSimplePlot::removeAxis(const QString& m_trackname)
{
    for (auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        if((*Vector_Iter)->m_trackname==m_trackname)
        {
            delete (*Vector_Iter);
            axisvector.erase(Vector_Iter);
            break;
        }
    }
}

void VmSimplePlot::destoryAllAxis()
{
    for (auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        delete (*Vector_Iter);
        (*Vector_Iter) = NULL;
    }
    // 如果 list 声明对象为类，则 clear 不能释放该内存，要辅以 erase 或 pop 或 delete
    axisvector.erase(axisvector.begin(), axisvector.end());
    axisvector.clear();
}


void VmSimplePlot::setYAxisRange(const QString& m_trackname, double start, double value)
{
    for (auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        if((*Vector_Iter)->m_trackname==m_trackname)
        {
            (*Vector_Iter)->setproperityvalue(start, value);
            //qInfo()<<m_trackname<<" "<<(*Vector_Iter)->getminrange()<<" "<<(*Vector_Iter)->getmaxrange();

            QVariantList list;
            list.push_back((*Vector_Iter)->getminrange());
            list.push_back((*Vector_Iter)->getmaxrange());
            emit yAxisRangeChanged(m_trackname, list);
            break;
        }
    }
}

void VmSimplePlot::setTimeAxisRange(double start, double value)
{
    m_timeaxis.setrange(start, value);
}

//--------------------------------------------------------------------------------
#define Y_AXIS_PROPERITY_MIN 0
#define Y_AXIS_PROPERITY_MAX (Axis::AxisIntervalCount-1)

#define TIME_AXIS_PROPERITY_MIN 0
#define TIME_AXIS_PROPERITY_MAX (Axis::AxisIntervalCount-1)

int VmSimplePlot::GetYMinAxisInterval(Axis::Y_AXIS_DANWEI danwei)
{
    return Y_AXIS_PROPERITY_MIN;
}

int VmSimplePlot::GetYMaxAxisInterval(Axis::Y_AXIS_DANWEI danwei)
{
    return Y_AXIS_PROPERITY_MAX;
}

int VmSimplePlot::GetTimeMinAxisInterval()
{
    return TIME_AXIS_PROPERITY_MIN;
}

int VmSimplePlot::GetTimeMaxAxisInterval()
{
    return TIME_AXIS_PROPERITY_MAX;
}

void VmSimplePlot::GetLeftArrow5Points(QPoint* points, int x, int y, int heigth, int width, int width_arrow, int trigger_arrow_range, QPoint* trigger_points/*=NULL*/)
{
    points[0] = QPoint(x, y);
    points[1] = QPoint(x - width_arrow, y + heigth / 2);
    points[2] = QPoint(x - width, y + heigth / 2);
    points[3] = QPoint(x - width, y - heigth / 2);
    points[4] = QPoint(x - width_arrow, y - heigth / 2);

    if (trigger_points != NULL)
    {
        trigger_points[0].rx() = x - trigger_arrow_range;
        trigger_points[0].ry() = y;
        trigger_points[1].rx() = points[2].x() + trigger_arrow_range;
        trigger_points[1].ry() = y;
        trigger_points[2].rx() = points[2].x() + trigger_arrow_range;
        trigger_points[2].ry() = points[2].y() - trigger_arrow_range;
        trigger_points[3].rx() = points[2].x() + trigger_arrow_range;
        trigger_points[3].ry() = points[3].y() + trigger_arrow_range;
    }
}

void VmSimplePlot::GetRightArrow5Points(QPoint* points, int x, int y, int heigth, int width, int width_arrow, int trigger_arrow_range, QPoint* trigger_points/*=NULL*/)
{
    points[0] = QPoint(x, y);
    points[1] = QPoint(x + width_arrow, y + heigth / 2);
    points[2] = QPoint(x + width, y + heigth / 2);
    points[3] = QPoint(x + width, y - heigth / 2);
    points[4] = QPoint(x + width_arrow, y - heigth / 2);

    if (trigger_points != NULL)
    {
        trigger_points[0].rx() = x + trigger_arrow_range;
        trigger_points[0].ry() = y;
        trigger_points[1].rx() = points[2].x() - trigger_arrow_range;
        trigger_points[1].ry() = y;
        trigger_points[2].rx() = points[2].x() - trigger_arrow_range;
        trigger_points[2].ry() = points[4].y() + trigger_arrow_range;
        trigger_points[3].rx() = points[2].x() - trigger_arrow_range;
        trigger_points[3].ry() = points[1].y() - trigger_arrow_range;
    }
}

void VmSimplePlot::GetTopArrow5Points(QPoint* points, int x, int y, int heigth, int width, int heigth_arrow, int trigger_arrow_range, QPoint* trigger_points/*=NULL*/)
{
    points[0] = QPoint(x, y);
    points[1] = QPoint(x - width / 2, y - heigth_arrow);
    points[2] = QPoint(x - width / 2, y - heigth);
    points[3] = QPoint(x + width / 2, y - heigth);
    points[4] = QPoint(x + width / 2, y - heigth_arrow);

    if (trigger_points != NULL)
    {
        trigger_points[0].rx() = x;
        trigger_points[0].ry() = y - trigger_arrow_range;
        trigger_points[1].rx() = x;
        trigger_points[1].ry() = points[2].y() + trigger_arrow_range;
        trigger_points[2].rx() = points[2].x() + trigger_arrow_range;
        trigger_points[2].ry() = points[2].y() + trigger_arrow_range;
        trigger_points[3].rx() = points[3].x() - trigger_arrow_range;
        trigger_points[3].ry() = points[3].y() + trigger_arrow_range;
    }
}

void VmSimplePlot::GetBottomArrow5Points(QPoint* points, int x, int y, int heigth, int width, int heigth_arrow, int trigger_arrow_range, QPoint* trigger_points/*=NULL*/)
{
    points[0] = QPoint(x, y);
    points[1] = QPoint(x + width / 2, y + heigth_arrow);
    points[2] = QPoint(x + width / 2, y + heigth);
    points[3] = QPoint(x - width / 2, y + heigth);
    points[4] = QPoint(x - width / 2, y + heigth_arrow);

    if (trigger_points != NULL)
    {
        trigger_points[0].rx() = x;
        trigger_points[0].ry() = y + trigger_arrow_range;
        trigger_points[1].rx() = x;
        trigger_points[1].ry() = points[2].y() - trigger_arrow_range;
        trigger_points[2].rx() = points[3].x() + trigger_arrow_range;
        trigger_points[2].ry() = points[3].y() - trigger_arrow_range;
        trigger_points[3].rx() = points[2].x() - trigger_arrow_range;
        trigger_points[3].ry() = points[2].y() - trigger_arrow_range;
    }
}

bool VmSimplePlot::DrawPointsArrow(QPainter* painter, QColor clr, QPoint* points, int point_count)
{
    QPen pen(clr);
    pen.setWidth(RegularSize.height() / BPI_96_ICON_SIZE);
    painter->setPen(pen);

    QBrush brush(clr);
    painter->setBrush(brush);

    QPolygon polygon = QPolygon();
    for(int k=0; k<point_count; k++)
        polygon << points[k];
    painter->drawPolygon(polygon);
    return true;
}

void VmSimplePlot::ComputeRects()
{
    m_clientRect = QRect(0,0,width(),height());

    //m_plotarea.m_plotRect
    int height = (m_support_top_info? TOP_INFO_NUM:0 + TOP_INTERVA_NUM ) * RegularSize.height();
    height += (WAVEPOSBAR_NO_INTERVA_NUM) * RegularSize.height();
    m_plotarea.m_plotRect.setTop(m_clientRect.top() + height);

    height = (bootom_axis_disp_num + BOTTOM_INTERVA_TOP_NUM + BOTTOM_INTERVA_BOTTOM_NUM ) * RegularSize.height();
    m_plotarea.m_plotRect.setBottom(m_clientRect.bottom() - height);

    int width = (LEFT_INFO_NUM + LEFT_INTERVA_NUM*2 ) * RegularSize.height();
    m_plotarea.m_plotRect.setLeft(m_clientRect.left() + width);

    width = (RIGHT_INFO_NUM + RIGHT_INTERVA_NUM*2 ) * RegularSize.height();
    m_plotarea.m_plotRect.setRight(m_clientRect.right() - width);

    m_plotarea.grid_resolution_x = m_plotarea.m_plotRect.width()/m_plotarea.grid_count_x;
    m_plotarea.grid_resolution_y = m_plotarea.m_plotRect.height()/m_plotarea.grid_count_y;
    //重新调整大小
    QPoint center = m_plotarea.m_plotRect.center();
    m_plotarea.m_plotRect.setLeft(center.x()-m_plotarea.grid_resolution_x*m_plotarea.grid_count_x/2);
    m_plotarea.m_plotRect.setRight(center.x()+m_plotarea.grid_resolution_x*m_plotarea.grid_count_x/2);
    m_plotarea.m_plotRect.setTop(center.y()-m_plotarea.grid_resolution_y*m_plotarea.grid_count_y/2);
    m_plotarea.m_plotRect.setBottom(center.y()+m_plotarea.grid_resolution_y*m_plotarea.grid_count_y/2);

    //m_timeaxis.m_axisRect
    m_timeaxis.m_axisRect.setLeft(m_plotarea.m_plotRect.left());
    m_timeaxis.m_axisRect.setRight(m_plotarea.m_plotRect.right());
    m_timeaxis.m_axisRect.setTop(m_plotarea.m_plotRect.bottom());
    m_timeaxis.m_axisRect.setBottom(m_clientRect.bottom());
    m_timeaxis.axis_resolution_x=m_plotarea.grid_resolution_x;
    m_timeaxis.axis_count_x=m_plotarea.grid_count_x;

    //Y Axis
    QRect m_axisRect;
    m_axisRect.setLeft(m_clientRect.left());
    m_axisRect.setRight(m_plotarea.m_plotRect.left());
    m_axisRect.setTop(m_plotarea.m_plotRect.top());
    m_axisRect.setBottom(m_plotarea.m_plotRect.bottom());
    for (auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        (*Vector_Iter)->m_axisRect=m_axisRect;
        (*Vector_Iter)->axis_resolution_y = m_plotarea.grid_resolution_y;
        (*Vector_Iter)->axis_count_y = m_plotarea.grid_count_y;
        //(*Vector_Iter)->setcenterpos( (*Vector_Iter)->m_axisRect.top+(*Vector_Iter)->m_deault_center_pos_bili*(*Vector_Iter)->m_axisRect.Height() );
    }
}

void VmSimplePlot::paint(QPainter *painter)
{
    ComputeRects();

    //内存绘图
    QPixmap MemPixmap(m_clientRect.width(), m_clientRect.height());
    QPainter MemPainter(&MemPixmap);

    //网格背景
    DrawBackgroud(&MemPainter);
    DrawGrid(&MemPainter);
    DrawYArrows(&MemPainter);
    DrawTimeArrows(&MemPainter);
    // 绘曲线
    UpdateLine(&MemPainter, MemPixmap);
    // 绘制继承类的多余绘图
    OtherDraw(&MemPainter, painter);

    painter->drawPixmap(0, 0, MemPixmap);
}

bool VmSimplePlot::DrawBackgroud(QPainter *painter)
{
    // 填充背景
    QBrush brush(m_plotarea.m_bgOutClr);
    painter->fillRect(m_clientRect, brush);
    QBrush brush1(m_plotarea.m_bgClr);
    painter->fillRect(m_plotarea.m_plotRect, brush1);
    return true;
}

bool VmSimplePlot::DrawGrid(QPainter *painter)
{
    // 画网格
   if (m_plotarea.bDrawGrid)
    {
        QPen pen(m_plotarea.m_gridClr);
        pen.setStyle(m_plotarea.nGridStyle);
        int line_large = RegularSize.height()/BPI_96_ICON_SIZE;
        pen.setWidth(line_large*2);

        //画边框
        painter->setPen(pen);
        painter->drawRect(m_plotarea.m_plotRect);

        pen.setWidth(line_large);
        painter->setPen(pen);

        // 网格竖线
        for(int l=0; l<=m_plotarea.grid_count_x; l++)
        {
            int x = (int)(m_plotarea.m_plotRect.left()+m_plotarea.grid_begin_x+l*m_plotarea.grid_resolution_x);
            if(x>m_plotarea.m_plotRect.right()+1)
                continue;

            //画大步进线
            //画小步进线用到和确定字体的大小
            double smallaxis = m_plotarea.grid_resolution_y/5.0;
            for(int i=0;i<=m_plotarea.grid_count_y;i++)
            {
                double y=m_plotarea.m_plotRect.top()+i*m_plotarea.grid_resolution_y;

                if (l > 0 && l < m_plotarea.grid_count_x)
                {
                    painter->drawLine((l == 5) || (i == 5) ? x - line_large*GRID_LARGE_1 : x - line_large, (int)y,
                                      (l == 5) || (i == 5) ? x + line_large*GRID_LARGE_1 : x + line_large, (int)y);
                }

                // 画小步进线
                if(y<m_plotarea.m_plotRect.bottom())
                {
                    if(smallaxis<=0)
                        break;  //防止因图太小出现死循环
                    for(int j=1; j<5; j++)
                    {
                        painter->drawLine(l==5? x-line_large*GRID_SMALL_1 :x-line_large, (int)(y+smallaxis*j),
                                          l==5? x+line_large*GRID_SMALL_1 :x+line_large, (int)(y+smallaxis*j));
                    }
                }
            }
        }

        // 网格横线
        for(int l=0; l<=m_plotarea.grid_count_y; l++)
        {
            int y = (int)(m_plotarea.m_plotRect.top()+l*m_plotarea.grid_resolution_y);
            if(y>m_plotarea.m_plotRect.bottom()+1)
                continue;

            //画小步进线用到
            double smallaxis = m_timeaxis.axis_resolution_x/5.0;
            //画大步进线
            for(int i=0; i<=m_timeaxis.axis_count_x; i++) //为了消除启动网格移动后最后一个刻度超出界限
            {
                double x = m_timeaxis.m_axisRect.left()+i*m_timeaxis.axis_resolution_x;
                if (l > 0 && l < m_plotarea.grid_count_y && (x>=m_timeaxis.m_axisRect.left()) )
                {
                    painter->drawLine((int)x, (l == 5) || (i == 5) ? y-line_large*GRID_LARGE_1 :y-line_large,
                                      (int)x, (l == 5) || (i == 5) ? y+line_large*GRID_LARGE_1 :y+line_large);
                }
                //画小步进线
                if(i!=m_timeaxis.axis_count_x+1)
                {
                    if(smallaxis<=0)
                        break;  //防止因图太小出现死循环
                    for(int j=0; j<5; j++)
                    {
                        if( x+smallaxis*j < m_timeaxis.m_axisRect.left()) continue;
                        if( x+smallaxis*j > m_timeaxis.m_axisRect.right()) break;
                        painter->drawLine( (int)(x+smallaxis*j), l==5? y-line_large*GRID_SMALL_1 :y-line_large,
                                           (int)(x+smallaxis*j), l==5? y+line_large*GRID_SMALL_1 :y+line_large);
                    }
                }
            }
        }
    }// 绘网格完毕
    return true;
}

bool VmSimplePlot::DrawYArrows(QPainter *painter)
{
    //-----------------------------y center----------------------------------
    QPoint points[5];
    for(auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        painter->setFont(SmallFont);
        QFontMetrics fontMetrics(SmallFont);
        int pixelsWidth = fontMetrics.horizontalAdvance((*Vector_Iter)->m_trackname);
        int pixelsHeight = fontMetrics.height();

        int left=m_plotarea.m_plotRect.left()-4;
        if((*Vector_Iter)->getyarrowpos()<(*Vector_Iter)->m_axisRect.top())
        {
            GetBottomArrow5Points(points, left-X_ARROW_WIDTH_NUM*RegularSize.height()/2, m_plotarea.m_plotRect.top(), X_ARROW_HEIGHT_NUM*RegularSize.height(),
                                    X_ARROW_WIDTH_NUM*RegularSize.height(), X_ARROW_SHORT_HEIGHT_NUM*RegularSize.height(), ARROW_RANGE_NUM*RegularSize.height());
            DrawPointsArrow(painter, (*Vector_Iter)->m_lineclr, points, 5);

            (*Vector_Iter)->m_chnRect=QRect(left-X_ARROW_WIDTH_NUM*RegularSize.height()/2, m_plotarea.m_plotRect.top(),
                                            X_ARROW_WIDTH_NUM*RegularSize.height()/2, X_ARROW_HEIGHT_NUM*RegularSize.height());

            painter->setPen(m_plotarea.m_bgOutClr);
            painter->translate((*Vector_Iter)->m_chnRect.center().x(), (*Vector_Iter)->m_chnRect.bottom());
            painter->rotate(-90);
            painter->drawText(0, 0, (*Vector_Iter)->m_trackname);
            painter->rotate(90);
            painter->translate(0,0);
        }
        else if((*Vector_Iter)->getyarrowpos()>(*Vector_Iter)->m_axisRect.bottom())
        {
            GetTopArrow5Points(points, left-X_ARROW_WIDTH_NUM*RegularSize.height()/2, m_plotarea.m_plotRect.bottom(), X_ARROW_HEIGHT_NUM*RegularSize.height(),
                                X_ARROW_WIDTH_NUM*RegularSize.height(), X_ARROW_SHORT_HEIGHT_NUM*RegularSize.height(), ARROW_RANGE_NUM*RegularSize.height());
            DrawPointsArrow(painter, (*Vector_Iter)->m_lineclr, points, 5);

            (*Vector_Iter)->m_chnRect = QRect(left-X_ARROW_WIDTH_NUM*RegularSize.height()/2, m_plotarea.m_plotRect.bottom()-X_ARROW_HEIGHT_NUM*RegularSize.height(),
                                            X_ARROW_WIDTH_NUM*RegularSize.height(), X_ARROW_HEIGHT_NUM*RegularSize.height());

            painter->setPen(m_plotarea.m_bgOutClr);
            painter->translate((*Vector_Iter)->m_chnRect.center().x(), (*Vector_Iter)->m_chnRect.top());
            painter->rotate(90);
            painter->drawText(0, 0, (*Vector_Iter)->m_trackname);
            painter->rotate(-90);
            painter->translate(0,0);
        }
        else
        {
            GetLeftArrow5Points(points, (*Vector_Iter)->m_axisRect.right(), (*Vector_Iter)->getyarrowpos(), Y_ARROW_HEIGHT_NUM*RegularSize.height(),
                                    Y_ARROW_WIDTH_NUM*RegularSize.height(), Y_ARROW_SHORT_WIDTH_NUM*RegularSize.height(), ARROW_RANGE_NUM*RegularSize.height());
            DrawPointsArrow(painter, (*Vector_Iter)->m_lineclr, points, 5);

            (*Vector_Iter)->m_chnRect = QRect((*Vector_Iter)->m_axisRect.right()-Y_ARROW_WIDTH_NUM*RegularSize.height(), (*Vector_Iter)->getyarrowpos()-pixelsHeight/2,
                                                Y_ARROW_WIDTH_NUM*RegularSize.height(), pixelsHeight);
            painter->setPen(m_plotarea.m_bgOutClr);
            painter->drawText((*Vector_Iter)->m_chnRect, Qt::AlignLeft, (*Vector_Iter)->m_trackname);
        }
    }
    return true;
}

bool VmSimplePlot::DrawTimeArrowsNotice(QPainter *painter)
{
    painter->setPen(m_infotext_clr);
    painter->setFont(RegularFont);

    QString str;
    m_plothelp.XAxisValueStr(str, m_timeaxis.GetAxisDanwei(), m_timeaxis.GetHorizontalOffset());
    str = "T-> " + str;
    painter->drawText(m_plotarea.m_plotRect.left()+8, m_plotarea.m_plotRect.top()+8, str);

    return true;
}

bool VmSimplePlot::UpdateLine(QPainter* painter, QPixmap& MemPixmap, QRect& rect, double time_min, double time_max, bool full)
{
    // 遍历每一条曲线
    QPoint* pts = new QPoint[rect.width()*2];
    int32_t pts_count = 0;
    int32_t x,y;

    for (auto list_Iter = axisvector.begin(); list_Iter != axisvector.end(); ++list_Iter)
    {
        if(((*list_Iter)->m_buffer!=NULL)&&((*list_Iter)->m_buffer_length!=0))
        {
            pts_count = 0;

            QPen pen((*list_Iter)->m_lineclr);
            pen.setWidth((*list_Iter)->m_linewidth);
            pen.setStyle((*list_Iter)->m_linestyle);
            painter->setPen(pen);

            double nMin = (*list_Iter)->getminrange();
            double nRange = (*list_Iter)->getrange();
            int nH = rect.bottom() - rect.top();
            int Wd = rect.width();
            int Bt = rect.bottom();

            //确定绘图范围
            double timerealmin = m_timeaxis.getminrange()>(*list_Iter)->m_buffer_time_min? m_timeaxis.getminrange():(*list_Iter)->m_buffer_time_min;
            double timerealmax = m_timeaxis.getmaxrange()<(*list_Iter)->m_buffer_time_max? m_timeaxis.getmaxrange():(*list_Iter)->m_buffer_time_max;
            double timereal_range = timerealmax-timerealmin;
            double line_time_range = (*list_Iter)->m_buffer_time_max - (*list_Iter)->m_buffer_time_min;
            //qInfo() << timerealmin << " " << timerealmax <<" " << timereal_range << " " << line_time_range;

            //计算绘图位置
            int databegin = (timerealmin-(*list_Iter)->m_buffer_time_min)*(*list_Iter)->m_buffer_length/line_time_range;
            int datacount = (timerealmax-timerealmin)*(*list_Iter)->m_buffer_length/line_time_range;
            //qInfo() << databegin << " " << datacount <<" " ;
            //qInfo() << rect;

            for(int i=databegin;i<databegin+datacount;i++)
            {
                x = ( (line_time_range)*i/(*list_Iter)->m_buffer_length + (*list_Iter)->m_buffer_time_min - m_timeaxis.getminrange() ) *Wd / m_timeaxis.getrange() + rect.left();
                y = Bt-(int)((((*list_Iter)->m_buffer)[i]-nMin)/nRange*(double)nH);

                if(i==databegin)
                {
                    pts[pts_count+1].rx() = pts[pts_count].rx() = x;
                    pts[pts_count+1].ry() = pts[pts_count].ry() = y;
                }
                else
                {
                    if(pts[pts_count].rx()==x)
                    {
                        pts[pts_count].rx() = x;
                        pts[pts_count].ry() = y < pts[pts_count].ry()? y:pts[pts_count].ry();
                        pts[pts_count+1].ry() = y > pts[pts_count+1].ry()? y:pts[pts_count+1].ry();
                    }
                    else
                    {
                        //qInfo() << pts[pts_count].rx() << " " << pts[pts_count].ry() <<" " << pts[pts_count+1].ry();
                        pts_count+=2;
                        pts[pts_count+1].rx() = pts[pts_count].rx() = x;
                        pts[pts_count+1].ry() = pts[pts_count].ry() = y;
                    }
                }
            }
            painter->drawLines(pts, pts_count);
        }
    }

    delete [] pts;
    return true;
}

bool VmSimplePlot::UpdateLine(QPainter* painter, QPixmap& MemPixmap)
{
    UpdateLine(painter, MemPixmap, m_plotarea.m_plotRect, m_timeaxis.getminrange(), m_timeaxis.getmaxrange());
    return true;
}

bool VmSimplePlot::OtherDraw(QPainter *mempainter, QPainter *painter)
{
    mempainter->setFont(RegularFont);

    int index = 0;
    for(auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        mempainter->setPen((*Vector_Iter)->m_lineclr);
        QString str;
        m_plothelp.YAxisValueStr(str, (*Vector_Iter)->GetAxisDanwei(), (*Vector_Iter)->getinterval());
        mempainter->drawText(m_plotarea.m_plotRect.left()+index*RegularSize.height()*10, m_plotarea.m_plotRect.bottom()+RegularSize.height(), str);
        index++;
    }

    mempainter->setPen(m_infotext_clr);
    QString str;
    m_plothelp.XAxisValueStr(str, m_timeaxis.GetAxisDanwei(), m_timeaxis.getaxisindexvalue());
    mempainter->drawText(m_plotarea.m_plotRect.right()-RegularSize.height()*10, m_plotarea.m_plotRect.bottom()+RegularSize.height(), str);

    return true;
}

void VmSimplePlot::updateDatas(int32_t real_length, int32_t sample/*QVariantList list*/)
{
    /*double* datas_ch1 = NULL;
    int32_t length_ch1 = 0;
    double* datas_ch2 = NULL;
    int32_t length_ch2 = 0;
    double timelength = 0;

    for(auto iter : list)
    {
        if(iter.typeName()==QString("double*"))
        {
            if(datas_ch1==NULL)
                datas_ch1 = (double*)iter.value<double*>();
            else
                datas_ch2 = (double*)iter.value<double*>();
        }
        else if(iter.typeName()==QString("int"))
        {
            if(datas_ch2==NULL)
                length_ch1 = iter.toInt();
            else
                length_ch2 = iter.toInt();
        }
        else if(iter.typeName()==QString("double"))
            timelength = iter.toDouble();
    }

    if((datas_ch1!=NULL)&&(length_ch1!=0)&&(datas_ch2!=NULL)&&(length_ch2!=0)&&(timelength!=0))
    {
        //qInfo() << (uint64_t)datas_ch1 << " " << length_ch1 <<" " << (uint64_t)datas_ch2 << " " << length_ch2 << " " << timelength;

        for(auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
        {
            if((*Vector_Iter)->m_trackname=="CH1")
            {
                (*Vector_Iter)->m_buffer = datas_ch1;
                (*Vector_Iter)->m_buffer_length = length_ch1;
                (*Vector_Iter)->m_buffer_time_min = -timelength/2.0;
                (*Vector_Iter)->m_buffer_time_max = timelength/2.0;
            }
            else if((*Vector_Iter)->m_trackname=="CH2")
            {
                (*Vector_Iter)->m_buffer = datas_ch2;
                (*Vector_Iter)->m_buffer_length = length_ch2;
                (*Vector_Iter)->m_buffer_time_min = -timelength/2.0;
                (*Vector_Iter)->m_buffer_time_max = timelength/2.0;
            }
        }
    }*/

    qDebug()<<" real_length "<< real_length <<"sample "<<sample;
    int32_t length_ch1 = ReadVoltageDatas(0, buffer_ch1, real_length);
    int32_t length_ch2 = ReadVoltageDatas(1, buffer_ch2, real_length);
    qDebug()<<"ReadVoltageDatas "<< length_ch1 <<" "<<length_ch2;
    int outrange_ch1 = IsVoltageDatasOutRange(0);
    int outrange_ch2 = IsVoltageDatasOutRange(1);
    qDebug()<<" is outrange "<< outrange_ch1 <<" "<<outrange_ch2;

     //将采集点数换算成时间ns
    double timelength = std::min(length_ch1,length_ch2)*1000000000.0/sample;

    //qInfo() << buffer_ch1 << " " << length_ch1 << " " << buffer_ch2 << " " << length_ch2 << " " << timelength;
    //QVariantList list;
    //list << QVariant::fromValue(buffer_ch1) << length_ch1 << QVariant::fromValue(buffer_ch2) << length_ch2 << timelength;

    for(auto Vector_Iter = axisvector.begin(); Vector_Iter != axisvector.end(); ++Vector_Iter)
    {
        if((*Vector_Iter)->m_trackname=="CH1")
        {
            (*Vector_Iter)->m_buffer = buffer_ch1;
            (*Vector_Iter)->m_buffer_length = length_ch1;
            (*Vector_Iter)->m_buffer_time_min = -timelength/2.0;
            (*Vector_Iter)->m_buffer_time_max = timelength/2.0;
        }
        else if((*Vector_Iter)->m_trackname=="CH2")
        {
            (*Vector_Iter)->m_buffer = buffer_ch2;
            (*Vector_Iter)->m_buffer_length = length_ch2;
            (*Vector_Iter)->m_buffer_time_min = -timelength/2.0;
            (*Vector_Iter)->m_buffer_time_max = timelength/2.0;
        }
    }

    emit updateDataSuccess();
}

void VmSimplePlot::updatDevState(int32_t devstate)
{
    if(devstate==1)
    {
        int32_t m_captureLength = GetMemoryLength()*1024;
        if(buffer_length!=m_captureLength)
        {
            if(buffer_ch1!=NULL)
                delete [] buffer_ch1;
            buffer_ch1 = new double[m_captureLength];
            if(buffer_ch2!=NULL)
                delete [] buffer_ch2;
            buffer_ch2 = new double[m_captureLength];
            buffer_length = m_captureLength;
        }
    }
}


