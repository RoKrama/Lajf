#include "logic.h"


Logic::Logic(int xsize, int ysize):
    x(xsize), y(ysize),
    micro(), sigma(0.22),
    pi(3.1416), e(2.718),
    throw_micro(micro),
    layers {0.8, 0.41, 0.25, 1.35}
{
    deviation = 1.f/255.f;

    gauss(throw_micro); range = throw_micro;

    typedef Cell* cellptr;
    cell_array = new cellptr[(x+8)];
    for(int i = 0; i < (x+8); i++)
        cell_array[i] = new Cell[(y+8)];

    cell_array_buffer = new cellptr[(x+8)];
    for(int i = 0; i < (x+8); i++)
        cell_array_buffer[i] = new Cell[(y+8)];

    for (int ij = 0; ij < 5; ij++) for(int i = ij; i < (9-ij); i++) for(int j = ij; j < (9-ij); j++)
        og_array[i][j] = layers[ij];
    og_array[4][4] = 0;

//    for(int i = 0; i < 9; i++) for(int j = 0; j < 9; j++) og_array[i][j] = layers[3];
//    for(int i = 1; i < 8; i++) for(int j = 1; j < 8; j++) og_array[i][j] = layers[0];
//    for(int i = 2; i < 7; i++) for(int j = 2; j < 7; j++) og_array[i][j] = layers[1];
//    for(int i = 3; i < 6; i++) for(int j = 3; j < 6; j++) og_array[i][j] = layers[2];


qDebug()<< range;
    init();

}
void Logic::init()
{
    std::random_device rnn;
    std::mt19937 rng(rnn());
    std::uniform_int_distribution<>distrib(0, 255);

    for (int i = 0; i < (x+8); i++) for(int j = 0; j < (y+8); j++)
        cell_array[i][j] = Cell(distrib(rng));

    for (int i = 50; i < 158; i++) for(int j = 50; j < 158; j++)
        cell_array[i][j].opacity = 200;
}
void Logic::compute_frame()
{

    for(int i = 4; i < (x+4); i++) for(int j = 4; j < (y+4); j++)
    {
        cell_array_buffer[i][j].blue = 0;
        cell_array_buffer[i][j].green = 0;
        cell_array_buffer[i][j].opacity = 0;
        cell_array_buffer[i][j].red = 0;
    }
//    for (int i = 4; i < (x+4); i++) for(int j = 4; j < (y+4); j++)
//    {
//        compute_red(i, j);
//        compute_green(i, j);
//        compute_blue(i, j);
//        compute_opacity(i, j);
//    }
    // about 2.5 times faster
    std::thread t_r(&Logic::ccr, this);
    std::thread t_g(&Logic::ccg, this);
    std::thread t_b(&Logic::ccb, this);
    std::thread t_o(&Logic::cco, this);

    t_r.join();
    t_g.join();
    t_b.join();
    t_o.join();

    for(int i = 4; i < (x+4); i++) for(int j = 4; j < (y+4); j++)
        cell_array[i][j] = cell_array_buffer[i][j];
}
float Logic::compute_opacity(const int &arr_i,const int &arr_j)
{
    for (int i = -4; i < 5; i++) for(int j = -4; j < 5; j++)
        cell_array_buffer[arr_i][arr_j].opacity += (cell_array[arr_i+i][arr_j+j].opacity * og_array[i+4][j+4]);

    cell_array_buffer[arr_i][arr_j].opacity *= 0.0125;
    set_to_normal(cell_array_buffer[arr_i][arr_j].opacity);
    (cell_array_buffer[arr_i][arr_j].opacity += cell_array[arr_i][arr_j].opacity) /= 2;

    return cell_array_buffer[arr_i][arr_j].opacity;
}

float Logic::compute_red(const int &arr_i,const int &arr_j)
{
    for (int i = -1; i < 2; i++) for(int j = -1; j < 2; j++)
        cell_array_buffer[arr_i][arr_j].red += cell_array[arr_i+i][arr_j+j].opacity;
//    for(int j = -4; j < 5; j++)
//    {
//        cell_array_buffer[arr_i][arr_j].red += cell_array[arr_i-4][arr_j+j].opacity;
//        cell_array_buffer[arr_i][arr_j].red += cell_array[arr_i+4][arr_j+j].opacity;
//    }
//    for(int i = -3; i < 4 ; i++)
//    {
//        cell_array_buffer[arr_i][arr_j].red += cell_array[arr_i+i][arr_j+4].opacity;
//        cell_array_buffer[arr_i][arr_j].red += cell_array[arr_i+i][arr_j-4].opacity;
//    }
    cell_array_buffer[arr_i][arr_j].red *= 0.125;
    set_to_normal(cell_array_buffer[arr_i][arr_j].red);
    (cell_array_buffer[arr_i][arr_j].red += cell_array[arr_i][arr_j].red) /= 2;

    return cell_array_buffer[arr_i][arr_j].red;
}
float Logic::compute_green(const int &arr_i,const int &arr_j)
{
    for(int j = -2; j < 3; j++)
    {
        cell_array_buffer[arr_i][arr_j].green += cell_array[arr_i-2][arr_j+j].opacity;
        cell_array_buffer[arr_i][arr_j].green += cell_array[arr_i+2][arr_j+j].opacity;
    }
    for(int i = -1; i < 2 ; i++)
    {
        cell_array_buffer[arr_i][arr_j].green += cell_array[arr_i+i][arr_j+2].opacity;
        cell_array_buffer[arr_i][arr_j].green += cell_array[arr_i+i][arr_j-2].opacity;
    }

    cell_array_buffer[arr_i][arr_j].green *= 0.0625;
    set_to_normal(cell_array_buffer[arr_i][arr_j].green);
    (cell_array_buffer[arr_i][arr_j].green += cell_array[arr_i][arr_j].green) /= 2;

    return cell_array_buffer[arr_i][arr_j].green;
}
float Logic::compute_blue(const int &arr_i,const int &arr_j)
{
    for(int j = -3; j < 4; j++)
    {
        cell_array_buffer[arr_i][arr_j].blue += cell_array[arr_i-3][arr_j+j].opacity;
        cell_array_buffer[arr_i][arr_j].blue += cell_array[arr_i+3][arr_j+j].opacity;
    }
    for(int i = -2; i < 3 ; i++)
    {
        cell_array_buffer[arr_i][arr_j].blue += cell_array[arr_i+i][arr_j+3].opacity;
        cell_array_buffer[arr_i][arr_j].blue += cell_array[arr_i+i][arr_j-3].opacity;
    }

    cell_array_buffer[arr_i][arr_j].blue *= 0.0417;
    set_to_normal(cell_array_buffer[arr_i][arr_j].blue);
    (cell_array_buffer[arr_i][arr_j].blue += cell_array[arr_i][arr_j].blue) /= 2;

    return cell_array_buffer[arr_i][arr_j].blue;
}

void Logic::set_to_normal(float &normal)
{
    normal *= deviation;
    gauss(normal);
    if(normal < 0) { normal = 0; return; }
    normal /= deviation;
    if(normal > 255) normal = 255;

}
void Logic::gauss(float &to_gauss)
{
//    to_gauss = (std::pow(e,((-(std::pow((to_gauss-micro),2)))
//                         /(2*sigma*sigma))));
////             /((std::sqrt(2*pi))*sigma)); //oof
///
    to_gauss =
        (
            (
            .5*(std::pow(e,((-(std::pow((to_gauss-micro),2)))
                            /(2*sigma*sigma))))
            +
            (std::pow(e,((-(std::pow((to_gauss-(micro+ .5f)),2)))
                            /(2*sigma*sigma))))
            )
            *0.5
        );
    }
Logic::~Logic()
{
    for(int i = 0; i < (x+6); i++)
        delete[] cell_array[i];
    delete[] cell_array;

    for(int i = 0; i < (x+6); i++)
        delete[] cell_array_buffer[i];
    delete[] cell_array_buffer;
}

Cell** Logic::get_frame_info()
{
    return cell_array;
}
Cell::Cell():
    red(0), green(0), blue(0),
    opacity(0)
{}
Cell::Cell(int a):
    red(0), green(0), blue(0),
    opacity(a)
{}
Cell::Cell(int r, int g, int b, int a):
    red(r), green(g), blue(b),
    opacity(a)
{}
