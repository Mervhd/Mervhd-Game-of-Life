# Conway's Game of Life

## Overview
This project is an interactive implementation of **Conway's Game of Life**, a cellular automaton devised by the British mathematician **John Horton Conway** in 1970. Built using **C++** and the **wxWidgets** library, this application allows users to visualize and manipulate the evolution of cell patterns based on simple rules. It features a dynamic grid, customizable settings, and a user-friendly interface.

## Features
- Adjustable grid size and simulation speed.
- Customizable colors for living and dead cells.
- Play, pause, next step, and clear controls.
- Option to display neighbor counts.
- Persistent settings storage via binary file.
- Responsive grid scaling with window resizing.

## Prerequisites
- **Operating System:** Windows
- **Development Environment:** Microsoft Visual Studio 2022
- **Compiler:** MSVC 14.42.3433 or later
- **Library:** wxWidgets (version compatible with Visual Studio 2022, e.g., 3.2.1 or later)
- **Git:** For cloning the repository

## Installation

### 1. Clone the Repository
```bash
git clone https://github.com/Mervhd/Mervhd-Game-of-Life.git
cd Mervhd-Game-of-Life
```

### 2. Set Up wxWidgets
1. Download and install **wxWidgets** from the [official website](https://www.wxwidgets.org/).
2. Configure Visual Studio to include wxWidgets:
   - Add the wxWidgets **include directory** to `Project Properties > VC++ Directories > Include Directories`.
   - Add the wxWidgets **library directory** to `Project Properties > VC++ Directories > Library Directories`.
   - Link against wxWidgets libraries (e.g., `wxmsw32u_core.lib`) in `Project Properties > Linker > Input > Additional Dependencies`.

### 3. Build the Project
1. Open the solution file (`.sln`) in **Visual Studio 2022**.
2. Build the solution in **Debug** or **Release** mode.
3. Ensure there are no build errors related to missing headers or libraries.

### 4. Run the Application
- Locate the generated executable in the build output directory (e.g., `Debug/GameOfLife.exe`).
- Double-click to run, or use Visual Studio's **Start** button.

## Usage

### Launching
Run the executable to open the main window.

### Interface
#### **Toolbar**
- **Play**: Starts the simulation.
- **Pause**: Pauses the simulation.
- **Next**: Advances one generation.
- **Clear**: Resets the grid to empty.

#### **Status Bar**
- Displays the **current generation** and **number of living cells**.

#### **Menu**
- **Options > Settings**: Opens a dialog to adjust grid size, interval, and cell colors.
- **View > Show Neighbor Count**: Toggles display of neighbor counts for each cell.

#### **Grid**
- Click to toggle cell states (live/dead).
- Resize the window by dragging the corners to scale the grid.

### **Settings Dialog**
- **Grid Size**: Adjust the grid dimensions (1-100).
- **Interval (ms)**: Set the simulation speed (10-1000 milliseconds).
- **Live Cell Color**: Pick the color for living cells.
- **Dead Cell Color**: Pick the color for dead cells.
- Click **OK** to apply changes or **Cancel** to discard.

## Game Rules
Conway's Game of Life evolves based on the following rules:
1. A **live** cell with **2 or 3** live neighbors **survives**.
2. A **dead** cell with exactly **3** live neighbors **becomes alive**.
3. All other cells **die** or **remain dead**.

## Troubleshooting

### **Invisible Grid Portions**
- Increase the window size or reduce the grid size in settings.

### **Crashes on Play**
- Ensure `gridSize` matches `gameBoard.size()`. Check console output for debug messages.

### **Build Errors**
- Verify wxWidgets paths and library links in **Visual Studio project properties**.

## License
This project is licensed under the **MIT License** 

## Contact
- **Author**: Mervhd (contact details TBD)

## Acknowledgments
- **Inspired by** John Horton Conway's Game of Life.
- **Built with** wxWidgets, an open-source C++ GUI framework.
