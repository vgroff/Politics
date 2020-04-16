#pragma once

#include <wx/wx.h>
#include "common/Date.hpp"
#include "Nation.hpp"

class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};


class Engine : public wxApp {
private:
    time_point currentDate;
    Nation playerNation = Nation::testSetupSingleNation();
public:
    Engine(time_point startingDate);
    void playTurn();
    void setupGUI();

    virtual bool OnInit();

    static Engine testSetup();
};