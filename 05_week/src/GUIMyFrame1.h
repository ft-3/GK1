#pragma once

/**
@file
Subclass of MyFrame1, which is generated by wxFormBuilder.
*/

#include "GUI.h"

//// end generated include

#include <wx/filedlg.h>
#include <wx/dcmemory.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/colourdata.h>
#include <wx/colordlg.h>

/** Implementing MyFrame1 */
class GUIMyFrame1 : public MyFrame1
{
protected:
    // Handlers for MyFrame1 events.
    void WxPanel_Repaint( wxUpdateUIEvent& event );
    void m_button_load_geometry_click( wxCommandEvent& event );
    void Scrolls_Updated( wxScrollEvent& event );
public:
    /** Constructor */
    GUIMyFrame1( wxWindow* parent );
    void Repaint();

};
