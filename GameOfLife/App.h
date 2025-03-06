#ifndef APP_H
#define APP_H

#include <wx/app.h>

class MainWindow;

class App : public wxApp {
public:
    virtual bool OnInit() override;
};

#endif