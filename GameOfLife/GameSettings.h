#pragma once

#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include <wx/colour.h>
#include <fstream>

struct GameSettings {
    unsigned int livingCellRed = 128;
    unsigned int livingCellGreen = 128;
    unsigned int livingCellBlue = 128;
    unsigned int livingCellAlpha = 255;

    unsigned int deadCellRed = 255;
    unsigned int deadCellGreen = 255;
    unsigned int deadCellBlue = 255;
    unsigned int deadCellAlpha = 255;

    int gridSize = 20;
    int interval = 50;

    bool showNeighborCounts = false;

    wxColor getLivingCellColor() { return wxColor(livingCellRed, livingCellGreen, livingCellBlue, livingCellAlpha); }
    wxColor getDeadCellColor() { return wxColor(deadCellRed, deadCellGreen, deadCellBlue, deadCellAlpha); }

    void setLivingCellColor(wxColor color) {
        livingCellRed = color.Red();
        livingCellGreen = color.Green();
        livingCellBlue = color.Blue();
        livingCellAlpha = color.Alpha();
    }

    void setDeadCellColor(wxColor color) {
        deadCellRed = color.Red();
        deadCellGreen = color.Green();
        deadCellBlue = color.Blue();
        deadCellAlpha = color.Alpha();
    }

    void Save() {
        std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
        if (file.is_open()) {
            file.write((char*)this, sizeof(GameSettings));
            file.close();
        }
    }

    void Load() {
        std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
        if (file.is_open()) {
            file.read((char*)this, sizeof(GameSettings));
            file.close();
        }
    }
};

#endif