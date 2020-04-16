#include "../include/Engine.hpp"
#include <iostream>

Engine::Engine(time_point startingDate) 
: currentDate(startingDate) {
    
}

void Engine::playTurn() {
    currentDate += hours(24*7);
    std::cout << "\nToday is: " << currentDate << std::endl;
    playerNation.runIndustryTurn();
}

Engine Engine::testSetup() {
    Engine e(time_point(date(1997, 9, 20), hours(0)));
    return e;
};

enum
{
    ID_Hello = 1
};
wxIMPLEMENT_APP(Engine);
bool Engine::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}
void MyFrame::OnHello(wxCommandEvent& event)
{
   

// - So, main event loop goes:
//     - Industry produces. Then Pops consume. Then Capital re-invests. Taxes and subsidies are applied during this process. Population grows
//     - Could have electors and pops function entirely independently, where pops do the economics and electors are just attached to a certain pop
//     - How exactly will source of truth be maintained? Like if we have population attached to each industry, and to each nation, who controls it?