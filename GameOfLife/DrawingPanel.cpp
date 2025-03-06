#include "DrawingPanel.h"
#include "MainWindow.h"
#include "GameSettings.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <wx/event.h>
#include <algorithm> // For std::min
#include <iostream>  // For debug output

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::OnMouseUp)
EVT_SIZE(DrawingPanel::OnSize)
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& board)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
    gameBoard(board),
    gridSize(dynamic_cast<MainWindow*>(parent)->getGridSize()),
    cellSize(0)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    InitializeGrid();
}

DrawingPanel::~DrawingPanel() {}

void DrawingPanel::OnPaint(wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    dc.Clear(); // Ensure full clear before redraw

    wxGraphicsContext* context = wxGraphicsContext::Create(dc);
    if (!context) return;

    if (gameSettings && gameSettings->showNeighborCounts && !neighborCounts.empty()) {
        wxFont font(wxFontInfo(8));
        context->SetFont(font, *wxRED);
        for (int row = 0; row < gridSize && row < static_cast<int>(neighborCounts.size()); ++row) {
            for (int col = 0; col < gridSize && col < static_cast<int>(neighborCounts[row].size()); ++col) {
                int count = neighborCounts[row][col];
                if (count > 0) {
                    wxString countText = wxString::Format("%d", count);
                    double textWidth, textHeight;
                    context->GetTextExtent(countText, &textWidth, &textHeight);
                    double x = col * cellSize + (cellSize - textWidth) / 2;
                    double y = row * cellSize + (cellSize - textHeight) / 2; // Fixed: Changed cellHeight to cellSize
                    context->DrawText(countText, x, y);
                }
            }
        }
    }
    DrawGrid(context);
    delete context;
}

void DrawingPanel::OnSize(wxSizeEvent& event) {
    wxSize size = event.GetSize();
    SetSize(size);
    int panelWidth, panelHeight;
    GetClientSize(&panelWidth, &panelHeight);
    cellSize = std::min(static_cast<double>(panelWidth) / gridSize, static_cast<double>(panelHeight) / gridSize);
    if (cellSize <= 0) cellSize = 1.0;
    Refresh(); // Force full repaint
    event.Skip();
}

void DrawingPanel::InitializeGrid() {
    if (gridSize <= 0) gridSize = 1;
    if (gameBoard.size() != static_cast<size_t>(gridSize)) {
        gameBoard.assign(gridSize, std::vector<bool>(gridSize, false));
    }
    neighborCounts.assign(gridSize, std::vector<int>(gridSize, 0));
    int panelWidth, panelHeight;
    GetClientSize(&panelWidth, &panelHeight);
    cellSize = std::min(static_cast<double>(panelWidth) / gridSize, static_cast<double>(panelHeight) / gridSize);
    if (cellSize <= 0) cellSize = 1.0;
    std::cout << "Initialized cellSize: " << cellSize << std::endl; // Debug output
}

void DrawingPanel::SetSize(const wxSize& size) {
    wxPanel::SetSize(size);
    int panelWidth, panelHeight;
    GetClientSize(&panelWidth, &panelHeight);
    cellSize = std::min(static_cast<double>(panelWidth) / gridSize, static_cast<double>(panelHeight) / gridSize);
    if (cellSize <= 0) cellSize = 1.0;
    std::cout << "Resized cellSize: " << cellSize << std::endl; // Debug output
    Refresh(); // Force full repaint
}

void DrawingPanel::setGridSize(int size) {
    gridSize = std::max(1, size);
    InitializeGrid();
    Refresh();
}

void DrawingPanel::DrawGrid(wxGraphicsContext* context) {
    if (cellSize <= 0) return;

    wxSize panelSize = GetClientSize();
    double penWidth = std::min(cellSize * 0.1, 2.0); // Scale pen width, cap at 2.0
    context->SetPen(wxPen(*wxBLACK, penWidth)); // Thicker black pen
    context->SetBrush(*wxTRANSPARENT_BRUSH);

    // Draw vertical lines
    for (int col = 0; col <= gridSize; ++col) {
        double x = col * cellSize;
        context->StrokeLine(x, 0, x, panelSize.GetHeight()); // Remove restrictive check
    }

    // Draw horizontal lines
    for (int row = 0; row <= gridSize; ++row) {
        double y = row * cellSize;
        context->StrokeLine(0, y, panelSize.GetWidth(), y); // Remove restrictive check
    }

    context->SetPen(*wxTRANSPARENT_PEN);
    for (int row = 0; row < gridSize && row < static_cast<int>(gameBoard.size()); ++row) {
        for (int col = 0; col < gridSize && col < static_cast<int>(gameBoard[row].size()); ++col) {
            double x = col * cellSize;
            double y = row * cellSize;
            if (gameBoard[row][col]) {
                wxBrush liveBrush(gameSettings ? gameSettings->getLivingCellColor() : *wxGREY_BRUSH);
                context->SetBrush(liveBrush);
            }
            else {
                wxBrush deadBrush(gameSettings ? gameSettings->getDeadCellColor() : *wxWHITE_BRUSH);
                context->SetBrush(deadBrush);
            }
            context->DrawRectangle(x, y, cellSize, cellSize);
        }
    }
    std::cout << "Drawn grid with cellSize: " << cellSize << std::endl; // Debug output
}

void DrawingPanel::OnMouseUp(wxMouseEvent& event) {
    if (cellSize <= 0) return;

    int x = event.GetX();
    int y = event.GetY();
    int row = static_cast<int>(y / cellSize);
    int col = static_cast<int>(x / cellSize);

    if (row >= 0 && row < gridSize && col >= 0 && col < gridSize &&
        row < static_cast<int>(gameBoard.size()) && col < static_cast<int>(gameBoard[row].size())) {
        gameBoard[row][col] = !gameBoard[row][col];
        Refresh();
    }
}

void DrawingPanel::setNeighborCounts(const std::vector<std::vector<int>>& counts) {
    neighborCounts = counts;
    Refresh();
}