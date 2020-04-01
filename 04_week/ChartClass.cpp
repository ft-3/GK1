#include <wx/dc.h>
#include <memory>

#include "ChartClass.h"
#include "vecmat.h"

ChartClass::ChartClass(std::shared_ptr<ConfigClass> c) : _w(600), _h(400)
{
    cfg = std::move(c);
    x_step = 200;
}

void
ChartClass::Set_Range()
{
    double xmin=9999.9,xmax=-9999.9,ymin=9999.9,ymax=-9999.9;
    double x,y,step;
    int i;

    xmin=cfg->Get_x_start();
    xmax=cfg->Get_x_stop();

    step=(cfg->Get_x_stop()-cfg->Get_x_start())/(double)x_step;
    x=cfg->Get_x_start();

    for (i=0;i<=x_step;i++) {
        y=GetFunctionValue(x);
        if (y>ymax) ymax=y;
        if (y<ymin) ymin=y;
        x=x+step;
    }

    y_min=ymin; y_max=ymax;
    x_min=xmin; x_max=xmax;
}


double
ChartClass::GetFunctionValue(double x)
{
    if (cfg->Get_F_type()==1) return x*x;
    if (cfg->Get_F_type()==2) return 0.5*exp(4*x-3*x*x);
    return x+sin(x*4.0);
}

void
ChartClass::Draw(wxDC *dc, int w, int h)
{
    // Set variables in object
    _w = w; _h = h;
    // Get rotation and all other matrix transforms
    Matrix axis_trans, chart_trans;
    if (cfg->RotateScreenCenter()) {
        axis_trans = set_rotation(cfg->Get_Alpha()) * set_scale();
        chart_trans = set_rotation(cfg->Get_Alpha())
                * set_translation(cfg->Get_dX(), cfg->Get_dY())
                * set_scale();
    }
    else {
        axis_trans = set_rotation(cfg->Get_Alpha()) * set_scale();
        chart_trans = set_translation(cfg->Get_dX(), cfg->Get_dY())
                * set_rotation(cfg->Get_Alpha())
                * set_scale();
    }

    // Setup canvas
    Set_Range();
    dc->SetBackground(wxBrush(wxColor(255, 255, 255)));
    dc->Clear();
    dc->SetPen(wxPen(wxColor(255, 0, 0)));
    dc->DrawRectangle(10, 10, _w - 20, _h - 20);
    dc->SetClippingRegion(wxRect(10, 10, _w - 20, _h - 20));
    dc->SetPen(wxPen(wxColor(0, 0, 0)));

    draw_axis(dc, axis_trans);
    dc->SetPen(wxPen(wxColor(0, 168, 107)));
    auto stp = (x_max - x_min) / x_step;
    for (auto i = x_min; i < x_max; i += stp)
        draw_line(dc, line2d(chart_trans, i, GetFunctionValue(i),
                            i + stp, GetFunctionValue(i + stp)));

}

void
ChartClass::draw_axis(wxDC* dc, Matrix t)
{
    // Axis
	draw_line(dc, line2d(t, cfg->Get_x0(), 0, cfg->Get_x1(), 0));
	draw_line(dc, line2d(t, 0, cfg->Get_y0(), 0, cfg->Get_y1()));

    // Arrows
	draw_line(dc, line2d(t, cfg->Get_x1(), 0, cfg->Get_x1() - 0.03, 0.03));
	draw_line(dc, line2d(t, cfg->Get_x1(), 0, cfg->Get_x1() - 0.03, -0.03));
	draw_line(dc, line2d(t, 0, cfg->Get_y1(), 0.03, cfg->Get_y1() - 0.03));
	draw_line(dc, line2d(t, 0, cfg->Get_y1(), -0.03, cfg->Get_y1() - 0.03));

    // Metric
	for (auto i = x_min; i <= x_max; i += 0.5)
		draw_line(dc, line2d(t, i, -0.03, i, 0.03));
	for (auto i = y_min; i <= y_max; i += 0.5)
		draw_line(dc, line2d(t, -0.015, i, 0.015, i));
}

void
ChartClass::draw_line(wxDC *dc, std::tuple<Vector, Vector> vectors)
{
   dc->DrawLine(std::get<0>(vectors).GetX(), _h - std::get<0>(vectors).GetY(),
                std::get<1>(vectors).GetX(), _h - std::get<1>(vectors).GetY()); 
}

std::tuple<Vector, Vector>
ChartClass::line2d(Matrix t, double x1, double y1, double x2, double y2)
{
    // Initialize vector to multiply by matrix
    Vector start, end;
    start.Set(x1, y1); end.Set(x2, y2);
    // Multiply
    start = t * start; end = t * end;
    return std::make_tuple(start, end);
}

double
ChartClass::Get_Y_min()
{
    Set_Range(); return y_min;
}

double
ChartClass::Get_Y_max()
{
    Set_Range(); return y_max;
}

Matrix
ChartClass::set_rotation(double phi)
{
	double x, y;

	if (cfg->RotateScreenCenter()) {
		x = _w / 2.0;
		y = _h / 2.0;
	} else {
		auto tmp = set_scale();
		x = tmp.data[0][2]; y = tmp.data[1][2];
	}

	Matrix rotation;
	auto cos_phi = cos(phi * M_PI / 180);
	auto sin_phi = sin(phi * M_PI / 180);

	rotation.data[0][0] = cos_phi;
	rotation.data[0][1] = -sin_phi;
	rotation.data[1][0] = sin_phi;
	rotation.data[1][1] = cos_phi;
	rotation.data[0][2] = x * (1 - cos_phi) + y * sin_phi;
	rotation.data[1][2] = -x * sin_phi + y * (1 - cos_phi);

	return rotation;
}

Matrix
ChartClass::set_translation(double x, double y)
{
	Matrix translation;
	translation.data[0][0] = 1.0; translation.data[1][1] = 1.0;
	translation.data[0][2] = x; translation.data[1][2] = -y;
	return translation;
}

Matrix
ChartClass::set_scale()
{
	Matrix scale;
	scale.data[0][0] = (_w - 15.0) / (cfg->Get_x1() - cfg->Get_x0());
	scale.data[1][1] = (_h - 15.0) / (cfg->Get_y1() - cfg->Get_y0());
	scale.data[0][2] = 10.0 - scale.data[0][0] * cfg->Get_x0();
	scale.data[1][2] = 10.0 - scale.data[1][1] * cfg->Get_y0();
	return scale;
}
