#ifndef LOGIC_H
#define LOGIC_H

#include <cmath>
#include <random>
#include <QPixmap>
#include <QDebug>
#include <cstdlib>
#include <thread>
#include <chrono>

class Cell;
class Logic : QObject
{
    Q_OBJECT

    Cell** cell_array;
    Cell** cell_array_buffer;

    int x,y;
    void init();

    float compute_red(const int &arr_i, const int &arr_j);
    float compute_green(const int &arr_i, const int &arr_j);
    float compute_blue(const int &arr_i, const int &arr_j);
    float compute_opacity(const int &arr_i, const int &arr_j);

    const float micro, pi, e;
    float deviation, range;
    float sigma;
    float throw_micro;
    void set_to_normal(float &normal);
    void gauss(float &to_gauss);
    float layers[4];
    float og_array[9][9];

    void ccr() {for (int i = 4; i < (x+4); i++) for(int j = 4; j < (y+4); j++) compute_red(i,j);}
    void ccg() {for (int i = 4; i < (x+4); i++) for(int j = 4; j < (y+4); j++) compute_green(i,j);}
    void ccb() {for (int i = 4; i < (x+4); i++) for(int j = 4; j < (y+4); j++) compute_blue(i,j);}
    void cco() {for (int i = 4; i < (x+4); i++) for(int j = 4; j < (y+4); j++) compute_opacity(i,j);}
public:
    Logic(int xsize, int ysize);
    ~Logic();
    Cell** get_frame_info();
    void compute_frame();
    void set_sigma(){sigma-=0.00005;};
};

class Cell
{
    friend class Logic;
    float red, green, blue, opacity;

    Cell();
    Cell(int r, int g, int b, int a);
    Cell(int a);

public:
    const float get_red() {return red;};
    const float get_green() {return green;};
    const float get_blue() {return blue;};
    const float get_opacity() {return opacity;};
};
#endif // LOGIC_H
