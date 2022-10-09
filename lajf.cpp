#include "lajf.h"
#include "qtimer.h"
#include <QDebug>

#define t_now std::chrono::high_resolution_clock::now();
#define ms  std::chrono::duration_cast<std::chrono::milliseconds>

Lajf::Lajf(QWidget *parent): QMainWindow(parent),
      color(QColor()),
      pixframe(350),
      painter(new QPainter()),
      timer(QTimer(this)),
      xlength(760),ylength(750),
      driver(Logic(xlength, ylength)),
      frame_count(0)
{
    pixmap = new QPixmap[pixframe],
    pixmap[0] = QPixmap(xlength,ylength);
    pixmap[0].fill(Qt::white);
    connect(&timer, &QTimer::timeout, this, &Lajf::paint);
    save_pixmap();
    showMaximized();
    timer.start(40);
}
void Lajf::save_pixmap()
{
    QPixmap loader = pixmap[0];
    for (int frame = 0; frame < pixframe; frame++)
    {
        pixmap[frame] = loader;
        auto time_start = t_now;
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

            painter->begin(&pixmap[frame]);
            painter->setPen(color);
            painter->drawPoint(k, l);
            painter->end();
        }

        loader = pixmap[frame];

        auto time_end = t_now;
        qDebug()<<"time: "<<ms(time_end-time_start).count()<<"  frame: "<<(frame+1);
    }
}
void Lajf::paint()
{
    if (frame_count < pixframe-1)
    {
        update(104,104,(xlength+4),(ylength+4));
        frame_count++;
    }
    else frame_count = 0;
}

void Lajf::paintEvent(QPaintEvent *)
{
    painter->begin(this);
    painter->drawPixmap(100,100,pixmap[frame_count]);
    painter->end();
}
Lajf::~Lajf()
{
    timer.stop();
    delete[] pixmap;
    delete painter;
}

