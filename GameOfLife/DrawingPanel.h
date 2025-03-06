#ifndef DRAWINGPANEL_H
#define DRAWINGPANEL_H

#include <wx/panel.h>
#include <vector>

struct GameSettings;
class wxPaintEvent;
class wxMouseEvent;
class wxSizeEvent;
class wxGraphicsContext;

class DrawingPanel : public wxPanel {
public:
    DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& board);
    ~DrawingPanel();

    void setGameSettings(GameSettings* settings) { gameSettings = settings; }
    void SetSize(const wxSize& size);
    void setGridSize(int size);
    void setNeighborCounts(const std::vector<std::vector<int>>& counts);

protected:
    void OnPaint(wxPaintEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void OnSize(wxSizeEvent& event);

private:
    GameSettings* gameSettings = nullptr;
    std::vector<std::vector<bool>>& gameBoard;
    std::vector<std::vector<int>> neighborCounts;
    int gridSize;
    double cellSize; // Single variable for cell size

    void InitializeGrid();
    void DrawGrid(wxGraphicsContext* context);

    wxDECLARE_EVENT_TABLE();
};

#endif