#include "SettingsDialog.h"
#include "GameSettings.h"
#include "MainWindow.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/event.h>

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
EVT_SPINCTRL(ID_GRID_SIZE_SPIN, SettingsDialog::OnGridSizeChange)
EVT_SPINCTRL(ID_INTERVAL_SPIN, SettingsDialog::OnIntervalChange)
EVT_COLOURPICKER_CHANGED(ID_LIVE_COLOR_PICKER, SettingsDialog::OnLiveColorChange)
EVT_COLOURPICKER_CHANGED(ID_DEAD_COLOR_PICKER, SettingsDialog::OnDeadColorChange)
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* parent, wxWindowID id, const wxString& title, GameSettings* settings)
    : wxDialog(parent, id, title), gameSettings(settings), mainWindow(dynamic_cast<MainWindow*>(parent))
{
    gridSizeSpinCtrl = new wxSpinCtrl(this, ID_GRID_SIZE_SPIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, settings->gridSize);
    intervalSpinCtrl = new wxSpinCtrl(this, ID_INTERVAL_SPIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 1000, settings->interval);
    liveColorPicker = new wxColourPickerCtrl(this, ID_LIVE_COLOR_PICKER, settings->getLivingCellColor());
    deadColorPicker = new wxColourPickerCtrl(this, ID_DEAD_COLOR_PICKER, settings->getDeadCellColor());

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Grid Size:"), 0, wxALL, 5);
    sizer->Add(gridSizeSpinCtrl, 0, wxALL | wxEXPAND, 5);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Interval (ms):"), 0, wxALL, 5);
    sizer->Add(intervalSpinCtrl, 0, wxALL | wxEXPAND, 5);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Live Cell Color:"), 0, wxALL, 5);
    sizer->Add(liveColorPicker, 0, wxALL | wxEXPAND, 5);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Dead Cell Color:"), 0, wxALL, 5);
    sizer->Add(deadColorPicker, 0, wxALL | wxEXPAND, 5);

    SetSizerAndFit(sizer);
}

void SettingsDialog::OnGridSizeChange(wxSpinEvent& event) {
    gameSettings->gridSize = gridSizeSpinCtrl->GetValue();
    if (mainWindow) {
        mainWindow->setGridSize(gameSettings->gridSize);
    }
}

void SettingsDialog::OnIntervalChange(wxSpinEvent& event) {
    gameSettings->interval = intervalSpinCtrl->GetValue();
}

void SettingsDialog::OnLiveColorChange(wxColourPickerEvent& event) {
    gameSettings->setLivingCellColor(liveColorPicker->GetColour());
}

void SettingsDialog::OnDeadColorChange(wxColourPickerEvent& event) {
    gameSettings->setDeadCellColor(deadColorPicker->GetColour());
}