#include "App.h"
#include "MainWindow.h"

bool App::OnInit() {
    MainWindow* window = new MainWindow();
    window->Show();
    return true;
}

wxIMPLEMENT_APP(App);