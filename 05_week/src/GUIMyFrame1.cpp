#include "GUIMyFrame1.h"
#include <vector>
#include <fstream>
#include "vecmat.h"
#include <iostream>

Matrix4 translation(double, double, double);
Matrix4 rotation(char, double);
Matrix4 scale(double, double, double);
inline Matrix4 center();

struct Point {
    float x, y, z;
    Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct Color {
    int R, G, B;
    Color(int _R, int _G, int _B) : R(_R), G(_G), B(_B) {}
};

struct Segment {
    Point begin, end;
    Color color;
    Segment(Point _begin, Point _end, Color _color) : begin(_begin), end(_end), color(_color) {}
};

std::vector<Segment> data;

GUIMyFrame1::GUIMyFrame1( wxWindow* parent ) : MyFrame1( parent )
{
    m_button_load_geometry->SetLabel(_("Wczytaj geometrie"));
    m_staticText25->SetLabel(_("Obrot X:"));
    m_staticText27->SetLabel(_("Obrot Y:"));
    m_staticText29->SetLabel(_("Obrot Z:"));

    WxSB_TranslationX->SetRange(0, 200); WxSB_TranslationX->SetValue(100);
    WxSB_TranslationY->SetRange(0, 200); WxSB_TranslationY->SetValue(100);
    WxSB_TranslationZ->SetRange(0, 200); WxSB_TranslationZ->SetValue(100);

    WxSB_RotateX->SetRange(0, 360); WxSB_RotateX->SetValue(0);
    WxSB_RotateY->SetRange(0, 360); WxSB_RotateY->SetValue(0);
    WxSB_RotateZ->SetRange(0, 360); WxSB_RotateZ->SetValue(0);

    WxSB_ScaleX->SetRange(1, 200); WxSB_ScaleX->SetValue(100);
    WxSB_ScaleY->SetRange(1, 200); WxSB_ScaleY->SetValue(100);
    WxSB_ScaleZ->SetRange(1, 200); WxSB_ScaleZ->SetValue(100);
}

void GUIMyFrame1::WxPanel_Repaint( wxUpdateUIEvent& event )
{
    Repaint();
}

void GUIMyFrame1::m_button_load_geometry_click( wxCommandEvent& event )
{
    wxFileDialog WxOpenFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("Geometry file (*.geo)|*.geo"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (WxOpenFileDialog.ShowModal() == wxID_OK) {
        double x1, y1, z1, x2, y2, z2;
        int r, g, b;

        std::ifstream in(WxOpenFileDialog.GetPath().ToAscii());
        if (in.is_open()) {
            data.clear();
            while (!in.eof()) {
                in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> r >> g >> b;
                data.push_back(Segment(Point(x1, y1, z1), Point(x2, y2, z2), Color(r, g, b)));
            }
            in.close();
        }
    }
}

void GUIMyFrame1::Scrolls_Updated( wxScrollEvent& event )
{
    WxST_TranslationX->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationX->GetValue() - 100) / 50.0));
    WxST_TranslationY->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationY->GetValue() - 100) / 50.0));
    WxST_TranslationZ->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationZ->GetValue() - 100) / 50.0));

    WxST_RotateX->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateX->GetValue()));
    WxST_RotateY->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateY->GetValue()));
    WxST_RotateZ->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateZ->GetValue()));

    WxST_ScaleX->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleX->GetValue() / 100.0));
    WxST_ScaleY->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleY->GetValue() / 100.0));
    WxST_ScaleZ->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleZ->GetValue() / 100.0));


    Repaint();
}


void GUIMyFrame1::Repaint()
{
	wxClientDC client(WxPanel); wxBufferedDC buffer(&client);

	int w, h;
	WxPanel->GetSize(&w, &h);

	buffer.SetBackground(wxBrush(wxColor(255, 255, 255)));
	buffer.Clear();
	buffer.SetClippingRegion(wxRect(0, 0, w, h));

    // Values necessary for performing transformations
    auto tr_x = (WxSB_TranslationX->GetValue() - 100.f) / 50.f;
    auto tr_y = (WxSB_TranslationY->GetValue() - 100.f) / 50.f;
    auto tr_z = (WxSB_TranslationZ->GetValue() - 100.f) / 50.f + 3.5f;

    auto rot_x = WxSB_RotateX->GetValue();
    auto rot_y = WxSB_RotateY->GetValue();
    auto rot_z = WxSB_RotateZ->GetValue();

    auto scl_x = WxSB_ScaleX->GetValue() / 100.f;
    auto scl_y = WxSB_ScaleY->GetValue() / 100.f;
    auto scl_z = WxSB_ScaleZ->GetValue() / 100.f;

    auto trsl = translation(tr_x, tr_y, tr_z);
    auto rot  = rotation('x', rot_x) * rotation('y', rot_y) * rotation('z', rot_z);
    auto scl  = scale(scl_x, scl_y, scl_z);

    auto transform = trsl * rot * scl;

    for (auto el : data) {
        buffer.SetPen(wxPen(wxColor(el.color.R, el.color.G, el.color.B)));
        Vector4 strt, end;

        strt.Set(el.begin.x, el.begin.y, el.begin.z);
        end.Set(el.end.x, el.end.y, el.end.z);
        strt = transform * strt;
        end = transform * end;

        if (strt.GetZ() < 0)
            strt.data[2] = 0.0001;
        if (end.GetZ() < 0) {
            end.data[0] = strt.data[0];
            end.data[1] = strt.data[1];
            end.data[2] = 0.0001;
        }

        strt.data[0] = strt.data[0] / strt.data[2];
        strt.data[1] = strt.data[1] / strt.data[2];
        strt = center() * strt;

        end.data[0] = end.GetX() / end.GetZ();
        end.data[1] = end.GetY() / end.GetZ();
        end = center() * end;

        buffer.DrawLine(strt.data[0] * w, strt.data[1] * h, end.data[0] * w, end.data[1] * h);
    }

}


Matrix4 translation(double x, double y, double z) {
	Matrix4 trsl;
	trsl.data[0][0] = trsl.data[1][1] = trsl.data[2][2] = 1;
	trsl.data[0][3] = x;
	trsl.data[1][3] = -y;
	trsl.data[2][3] = z;
	return trsl;
}

Matrix4 rotation(char axis, double phi) {
    Matrix4 rot;
    double cosPhi = cos(phi* M_PI / 180);
    double sinPhi = sin(phi* M_PI / 180);
    switch (axis) {
    case 'x':
        rot.data[0][0] = 1;
        rot.data[1][1] = cosPhi;
        rot.data[2][2] = cosPhi;
        rot.data[1][2] = -sinPhi;
        rot.data[2][1] = sinPhi;
        break;
    case 'y':
        rot.data[0][0] = cosPhi;
        rot.data[1][1] = 1;
        rot.data[2][2] = cosPhi;
        rot.data[0][2] = sinPhi;
        rot.data[2][0] = -sinPhi;
        break;
    case 'z':
        rot.data[0][0] = cosPhi;
        rot.data[1][1] = cosPhi;
        rot.data[2][2] = 1;
        rot.data[0][1] = -sinPhi;
        rot.data[1][0] = sinPhi;
        break;
    }
    return rot;
}

Matrix4 scale(double x, double y, double z) {
	Matrix4 scl;
	scl.data[0][0] = x;
	scl.data[1][1] = -y;
	scl.data[2][2] = z;
	return scl;
}

inline Matrix4 center() {
	Matrix4 ctr;
	ctr.data[0][0] = ctr.data[1][1] = ctr.data[2][2] = 1;
	ctr.data[0][3] = ctr.data[1][3] = 0.5;
	return ctr;
}
