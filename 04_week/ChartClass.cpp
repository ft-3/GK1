#include <wx/dc.h>
#include <memory>

#include "ChartClass.h"
#include "vecmat.h"

#define RGB wxColor

ChartClass::ChartClass(std::shared_ptr<ConfigClass> c, int w, int h) : _w(w), _h(h)
{
    cfg = std::move(c);
    x_step = 200;
}

void ChartClass::Set_Range()
{
 double xmin=9999.9,xmax=-9999.9,ymin=9999.9,ymax=-9999.9;
 double x,y,step;
 int i;

 xmin=cfg->Get_x_start();
 xmax=cfg->Get_x_stop();

 step=(cfg->Get_x_stop()-cfg->Get_x_start())/(double)x_step;
 x=cfg->Get_x_start();

 for (i=0;i<=x_step;i++)
  {
   y=GetFunctionValue(x);
   if (y>ymax) ymax=y;
   if (y<ymin) ymin=y;
   x=x+step;
  }

 y_min=ymin;
 y_max=ymax;
 x_min=xmin;
 x_max=xmax;
}


double ChartClass::GetFunctionValue(double x) {
 if (cfg->Get_F_type()==1) return x*x;
 if (cfg->Get_F_type()==2) return 0.5*exp(4*x-3*x*x);
 return x+sin(x*4.0);
}

void ChartClass::Draw(wxDC *dc, int w, int h) {
    _w = w;
    _h = h;
    Matrix trans;
    if (cfg->RotateScreenCenter())
        trans = SetRotation(cfg->Get_Alpha())
                * SetTranslation(cfg->Get_dX(), cfg->Get_dY())
                * SetScale();
    else trans = SetTranslation(cfg->Get_dX(), cfg->Get_dY())
                * SetRotation(cfg->Get_Alpha())
                * SetScale();

    dc->SetBackground(wxBrush(RGB(255, 255, 255)));
    dc->Clear();
    dc->SetPen(wxPen(RGB(255, 0, 0)));
    dc->DrawRectangle(10, 10, _w - 20, _h - 20);
    dc->SetClippingRegion(wxRect(10, 10, _w - 20, _h - 20));
    dc->SetPen(wxPen(RGB(0, 0, 255)));
    draw_axis(dc, trans);

    dc->SetPen(wxPen(RGB(0, 255, 0)));

    auto stp = (x_max - x_min) / x_step;

    for (auto i = x_min; i < x_max; i += stp)
        draw_line(dc, line2d(trans, i, GetFunctionValue(i), i + stp, GetFunctionValue(i + stp)));
}

void ChartClass::draw_axis(wxDC* dc, Matrix t) {
    // Axis
	draw_line(dc, line2d(t, cfg->Get_x_start(), 0, cfg->Get_x_stop(), 0));
	draw_line(dc, line2d(t, 0, Get_Y_min(), 0, Get_Y_max()));

    // Arrows
	draw_line(dc, line2d(t, x_max, 0, x_max - 0.05, 0.05));
	draw_line(dc, line2d(t, x_max, 0, x_max - 0.05, -0.05));
	draw_line(dc, line2d(t, 0, y_max, 0.05, y_max - 0.05));
	draw_line(dc, line2d(t, 0, y_max, -0.05, y_max - 0.05));
}

void ChartClass::draw_line(wxDC *dc, std::tuple<Vector, Vector> vectors) {
   dc->DrawLine(std::get<0>(vectors).GetX(), _h - std::get<0>(vectors).GetY(), std::get<1>(vectors).GetX(), _h - std::get<1>(vectors).GetY()); 
}

std::tuple<Vector, Vector> ChartClass::line2d(Matrix t, double x1, double y1, double x2, double y2)
{
    // Initialize vector to multiply by matrix
    Vector start(x1, y1), end(x2, y2);
    // Multiply
    start = t * start; end = t * end;
    return std::make_tuple(start, end);
}

double ChartClass::Get_Y_min()
{
    Set_Range(); return y_min;
}

double ChartClass::Get_Y_max()
{
    Set_Range(); return y_max;
}

Matrix ChartClass::SetRotation(double alpha) {
	double x, y;

	if(cfg->RotateScreenCenter()) {
		x = _w / 2.0;
		y = _h / 2.0;
	}
	else {
		Matrix tmp = SetScale();
		x = tmp.data[0][2];
		y = tmp.data[1][2];
	}

	Matrix rotation;
	double cosAlpha = cos(alpha * M_PI / 180);
	double sinAlpha = sin(alpha * M_PI / 180);
	rotation.data[0][0] = cosAlpha;
	rotation.data[0][1] = -sinAlpha;
	rotation.data[1][0] = sinAlpha;
	rotation.data[1][1] = cosAlpha;
	rotation.data[0][2] = x * (1 - cosAlpha) + y * sinAlpha;
	rotation.data[1][2] = -x * sinAlpha + y * (1 - cosAlpha);
	return rotation;
}

Matrix ChartClass::SetTranslation(double dx, double dy) {
	Matrix translation;
	translation.data[0][0] = 1.0;
	translation.data[1][1] = 1.0;
	translation.data[0][2] = dx;
	translation.data[1][2] = -dy;
	return translation;
}

Matrix ChartClass::SetScale() {
	Matrix scale;
	scale.data[0][0] = (_w - 20.0) / (cfg->Get_x1() - cfg->Get_x0());
	scale.data[1][1] = (_h - 20.0) / (cfg->Get_y1() - cfg->Get_y0());
	scale.data[0][2] = 10.0 - scale.data[0][0] * cfg->Get_x0();
	scale.data[1][2] = 10.0 - scale.data[1][1] * cfg->Get_y0();
	return scale;
}


