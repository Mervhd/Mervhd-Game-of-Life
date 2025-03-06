#include "MainWindow.h"
#include "DrawingPanel.h"
#include "GameSettings.h"
#include "SettingsDialog.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include <wx/toolbar.h>
#include <wx/timer.h>
#include <wx/event.h>
#include <wx/menu.h>
#include <wx/defs.h> // For wxID_SETTINGS
#include <ctime>

enum {
    ID_Play = 10001,
    ID_Pause,
    ID_Next,
    ID_Clear,
    ID_ShowNeighborCounts = 10005
};

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::OnSizeChanged)
EVT_TOOL(ID_Play, MainWindow::OnPlay)
EVT_TOOL(ID_Pause, MainWindow::OnPause)
EVT_TOOL(ID_Next, MainWindow::OnNext)
EVT_TOOL(ID_Clear, MainWindow::OnClear)
EVT_TIMER(wxID_ANY, MainWindow::OnTimer)
EVT_MENU(wxID_SETTINGS, MainWindow::OnSettings)
EVT_MENU(ID_ShowNeighborCounts, MainWindow::OnToggleShowNeighbors)
wxEND_EVENT_TABLE()

MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(400, 400)),
    gameSettings(new GameSettings()),
    drawingPanel(nullptr),
    boxSizer(new wxBoxSizer(wxVERTICAL)),
    statusBar(nullptr),
    toolBar(nullptr),
    timer(new wxTimer(this, wxID_ANY)),
    gridSize(0),
    generation(0),
    livingCells(0),
    timerInterval(50),
    isRunning(false)
{
    gameSettings->Load();
    gridSize = gameSettings->gridSize;
    timerInterval = gameSettings->interval;

    initializeGrid();
    drawingPanel = new DrawingPanel(this, gameBoard);

    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            gameBoard[i][j] = (rand() % 3 == 0);
            if (gameBoard[i][j]) livingCells++;
        }
    }

    statusBar = CreateStatusBar();
    toolBar = CreateToolBar();

    toolBar->AddTool(ID_Play, "Play", wxBitmap(play_xpm), "Play");
    toolBar->AddTool(ID_Pause, "Pause", wxBitmap(pause_xpm), "Pause");
    toolBar->AddTool(ID_Next, "Next", wxBitmap(next_xpm), "Step to next generation");
    toolBar->AddTool(ID_Clear, "Clear", wxBitmap(trash_xpm), "Clear grid");
    toolBar->Realize();

    boxSizer->Add(drawingPanel, 1, wxEXPAND);
    boxSizer->Add(statusBar, 0, wxEXPAND);
    SetSizer(boxSizer);

    drawingPanel->setGameSettings(gameSettings);
    drawingPanel->SetMinSize(wxSize(200, 200));

    updateStatusBar();

    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* optionsMenu = new wxMenu();
    wxMenu* viewMenu = new wxMenu();

    viewMenu->AppendCheckItem(ID_ShowNeighborCounts, "Show Neighbor Count");
    menuBar->Append(viewMenu, "View");

    optionsMenu->Append(wxID_SETTINGS, "Settings");
    menuBar->Append(optionsMenu, "Options");
    SetMenuBar(menuBar);

    Layout();
    drawingPanel->Refresh();
}

MainWindow::~MainWindow() {
    delete timer;
    delete gameSettings;
    delete drawingPanel;
    delete boxSizer;
}

void MainWindow::nextGeneration() {
    if (gameBoard.empty() || gameBoard.size() != static_cast<size_t>(gridSize) || gameBoard[0].size() != static_cast<size_t>(gridSize)) {
        initializeGrid();
        return;
    }

    std::vector<std::vector<bool>> sandbox(gridSize, std::vector<bool>(gridSize, false));
    neighborCounts.assign(gridSize, std::vector<int>(gridSize, 0));
    int newLivingCells = 0;

    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            int livingNeighbors = countLivingNeighbors(i, j);
            neighborCounts[i][j] = livingNeighbors;

            if (i < static_cast<int>(gameBoard.size()) && j < static_cast<int>(gameBoard[i].size())) {
                if (gameBoard[i][j]) {
                    sandbox[i][j] = (livingNeighbors == 2 || livingNeighbors == 3);
                    if (sandbox[i][j]) newLivingCells++;
                }
                else {
                    sandbox[i][j] = (livingNeighbors == 3);
                    if (sandbox[i][j]) newLivingCells++;
                }
            }
        }
    }

    gameBoard = sandbox;
    generation++;
    livingCells = newLivingCells;

    updateStatusBar();
    drawingPanel->setNeighborCounts(neighborCounts);
    drawingPanel->Refresh();
}

void MainWindow::OnSizeChanged(wxSizeEvent& event) {
    if (drawingPanel) {
        wxSize windowSize = event.GetSize();
        drawingPanel->SetSize(windowSize);
    }
    event.Skip();
}

void MainWindow::initializeGrid() {
    gridSize = std::max(1, gridSize);
    gameBoard.assign(gridSize, std::vector<bool>(gridSize, false));
    neighborCounts.assign(gridSize, std::vector<int>(gridSize, 0));
    if (drawingPanel) {
        drawingPanel->setGridSize(gridSize);
    }
}

void MainWindow::setGridSize(int size) {
    timer->Stop();
    isRunning = false;

    gridSize = std::max(1, size);
    initializeGrid();
    livingCells = 0;
    generation = 0;
    updateStatusBar();
    drawingPanel->Refresh();
}

void MainWindow::updateStatusBar() {
    statusBar->SetStatusText(wxString::Format("Generation: %d, Living cells: %d", generation, livingCells));
}

void MainWindow::OnPlay(wxCommandEvent& event) {
    if (!isRunning) {
        isRunning = true;
        timer->Start(timerInterval);
    }
}

void MainWindow::OnPause(wxCommandEvent& event) {
    if (isRunning) {
        isRunning = false;
        timer->Stop();
    }
}

void MainWindow::OnNext(wxCommandEvent& event) {
    nextGeneration();
}

void MainWindow::OnClear(wxCommandEvent& event) {
    timer->Stop();
    isRunning = false;

    for (auto& row : gameBoard) {
        std::fill(row.begin(), row.end(), false);
    }
    livingCells = 0;
    generation = 0;
    updateStatusBar();
    drawingPanel->Refresh();
    gameSettings->Save();
}

int MainWindow::countLivingNeighbors(int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int newRow = row + i;
            int newCol = col + j;
            if (newRow >= 0 && newRow < gridSize && newCol >= 0 && newCol < gridSize) {
                if (newRow < static_cast<int>(gameBoard.size()) && newCol < static_cast<int>(gameBoard[newRow].size())) {
                    if (gameBoard[newRow][newCol]) count++;
                }
            }
        }
    }
    return count;
}

void MainWindow::OnTimer(wxTimerEvent& event) {
    if (isRunning) {
        nextGeneration();
    }
}

void MainWindow::OnSettings(wxCommandEvent& event) {
    timer->Stop();
    isRunning = false;

    SettingsDialog dialog(this, wxID_ANY, "Settings", gameSettings);
    if (dialog.ShowModal() == wxID_OK) {
        setGridSize(gameSettings->gridSize);
        timerInterval = gameSettings->interval;
        gameSettings->Save();
        drawingPanel->Refresh();
    }
}

void MainWindow::OnToggleShowNeighbors(wxCommandEvent& event) {
    gameSettings->showNeighborCounts = event.IsChecked();
    gameSettings->Save();
    drawingPanel->Refresh();
}

int MainWindow::getGridSize() {
    return gridSize;
}