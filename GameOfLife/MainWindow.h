#pragma once

#include <wx/frame.h>
#include <wx/sizer.h>    // For wxBoxSizer
#include <wx/statusbr.h> // For wxStatusBar
#include <wx/toolbar.h>  // For wxToolBar
#include <wx/timer.h>    // For wxTimer
#include <vector>

// Forward declarations
struct GameSettings; // Changed to struct
class DrawingPanel;
class SettingsDialog;
class wxCommandEvent;
class wxTimerEvent;
class wxSizeEvent;

class MainWindow : public wxFrame {
public:
    MainWindow();
    ~MainWindow();

    void setGridSize(int size);
    void updateStatusBar();
    int countLivingNeighbors(int row, int col);
    int getGridSize();

    void OnPlay(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnNext(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnSettings(wxCommandEvent& event);
    void OnToggleShowNeighbors(wxCommandEvent& event);
    void OnSizeChanged(wxSizeEvent& event);

private:
    GameSettings* gameSettings; // Pointer, no type specifier issue
    DrawingPanel* drawingPanel;
    wxBoxSizer* boxSizer;       // Properly declared
    wxStatusBar* statusBar;
    wxToolBar* toolBar;
    wxTimer* timer;             // Pointer, matches destructor usage

    std::vector<std::vector<bool>> gameBoard;
    std::vector<std::vector<int>> neighborCounts;

    int gridSize;
    int generation;
    int livingCells;
    int timerInterval;
    bool isRunning;

    void initializeGrid();
    void nextGeneration();

    wxDECLARE_EVENT_TABLE();
};