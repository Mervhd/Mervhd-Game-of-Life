#pragma once

#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>

struct GameSettings;
class MainWindow;
class wxSpinEvent;
class wxColourPickerEvent;

enum {
    ID_GRID_SIZE_SPIN = wxID_HIGHEST + 1,
    ID_INTERVAL_SPIN,
    ID_LIVE_COLOR_PICKER,
    ID_DEAD_COLOR_PICKER,
    wxID_SETTINGS = wxID_HIGHEST + 5
};

class SettingsDialog : public wxDialog {
public:
    SettingsDialog(wxWindow* parent, wxWindowID id, const wxString& title, GameSettings* gameSettings);

    bool getShowNeighborCounts() { return showNeighborCounts; }

private:
    GameSettings* gameSettings;
    MainWindow* mainWindow;
    wxSpinCtrl* gridSizeSpinCtrl;
    wxSpinCtrl* intervalSpinCtrl;
    wxColourPickerCtrl* liveColorPicker;
    wxColourPickerCtrl* deadColorPicker;
    bool showNeighborCounts = false;

    void OnGridSizeChange(wxSpinEvent& event);
    void OnIntervalChange(wxSpinEvent& event);
    void OnLiveColorChange(wxColourPickerEvent& event);
    void OnDeadColorChange(wxColourPickerEvent& event);

    wxDECLARE_EVENT_TABLE();
};