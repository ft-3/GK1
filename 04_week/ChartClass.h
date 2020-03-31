#pragma once

#include <memory>
#include <tuple>

#include "ConfigClass.h"
#include "vecmat.h"

class ChartClass {
private:
    int _w, _h;
    std::shared_ptr<ConfigClass> cfg;
    int x_step;         // liczba odcinkow z jakich bedzie sie skladal wykres
    double x_min,x_max; // zakres zmiennej x
    double y_min,y_max; // zakres wartosci przyjmowanych przez funkcje
    double GetFunctionValue(double x); // zwraca wartosci rysowanej funkcji

    std::tuple<Vector, Vector> line2d(Matrix t, double, double, double, double);
    void draw_line(wxDC *dc, std::tuple<Vector, Vector>);
    void draw_axis(wxDC *dc, Matrix t);
    Matrix SetTranslation(double, double);
    Matrix SetRotation(double);
    Matrix SetScale();
   
public:
    ChartClass(std::shared_ptr<ConfigClass> c, int w = 600, int h = 400);
    void Set_Range();   // ustala wartosci zmiennych x_min,y_min,x_max,y_max
    double Get_Y_min(); // zwraca y_min
    double Get_Y_max(); // zwraca y_max
    void Draw(wxDC *dc, int, int);  // rysuje wykres
};
