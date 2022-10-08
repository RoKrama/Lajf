#ifndef LAJF_H
#define LAJF_H

#include "qtimer.h"
#include <QMainWindow>
#include <QPainter>
#include "logic.h"
#include <cstdlib>
#include <chrono>

class Lajf : public QMainWindow
{
    Q_OBJECT

    QPainter* painter;
    QPixmap* pixmap;
    QTimer timer;
    QColor color;

    int xlength, ylength;
    void save_pixmap();
    int frame_count;
    int pixframe;

    Logic driver;

public:
    Lajf(QWidget *parent = nullptr);
    ~Lajf();

public slots:
    void paint();
    void paintEvent(QPaintEvent*) override;

};
#endif // LAJF_H
