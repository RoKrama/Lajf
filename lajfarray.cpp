#include "lajf.h"
#include "qtimer.h"
#include <QDebug>


Lajf::Lajf(QWidget *parent)
    : QMainWindow(parent),

      color(QColor()),
      pixmap(new QPixmap[100]),
      painter(new QPainter()),
      timer(QTimer(this)),
      xlength(400),ylength(400),
      driver(Logic(xlength, ylength)),
      frame_count(0)
{
    setGeometry(200,200,500,500);
    for(int i = 0; i<100; i++)
    {
        pixmap[i] = QPixmap(500,500);
        pixmap[i].fill();
    }
    connect(&timer, &QTimer::timeout, this, &Lajf::paint);
    save_pixmap();
    timer.start(100);
}
void Lajf::save_pixmap()
{
    for (int frame = 0; frame < 100; frame++)
    {
        driver.compute_frame();

        for(int k = 3; k < xlength; k++) for(int l = 3; l < ylength; l++)
        {
            color.setRgb
            (
                driver.get_frame_info()[k][l].get_red(),
                driver.get_frame_info()[k][l].get_green(),
                driver.get_frame_info()[k][l].get_blue(),
                driver.get_frame_info()[k][l].get_opacity()
            );
    //        qDebug()<<driver.get_frame_info()[k][l].get_red()
    //               <<driver.get_frame_info()[k][l].get_green()
    //              <<driver.get_frame_info()[k][l].get_blue()
    //                 <<driver.get_frame_info()[k][l].get_opacity();
            painter->begin(&pixmap[frame]);
            painter->setPen(color);
            painter->drawPoint(k, l);
            painter->end();
        }
        qDebug()<<frame;
    }
}
void Lajf::paint()
{
    if(frame_count < 100)
    {
        update();
        frame_count++;
    }
    else frame_count = 0;
}

void Lajf::paintEvent(QPaintEvent *)
{
    painter->begin(this);
    painter->drawPixmap(10,10,pixmap[frame_count]);
    painter->end();
}
Lajf::~Lajf()
{
    timer.stop();
    delete[] pixmap;
    delete painter;
}

