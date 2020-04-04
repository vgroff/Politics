#include<iostream>
#include "include/Engine.hpp"
#include<random>
#include <iomanip>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// int main() {
//     std::cout << std::setprecision(4);
//     std::srand(38);
//     Engine e = Engine::testSetup();
//     for (int i = 0; i < 1; i++) {
//         e.playTurn();
//     }
//     return 0;
// }

class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

class PoliticsGame: public wxApp
{
private:
    Engine engine;
     
public:
    PoliticsGame();
    virtual bool OnInit();
};

PoliticsGame::PoliticsGame() 
    : wxApp(),
    engine(Engine::testSetup()) {

}

enum
{
    ID_Hello = 1
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Hello,   MyFrame::OnHello)
    EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(PoliticsGame);

bool PoliticsGame::OnInit() {
    std::cout << std::setprecision(4);
    std::srand(38);
    std::cout << "SHOW" << std::endl;
    MyFrame* frame = new MyFrame( "Hello World", wxPoint(50, 50), wxSize(450, 340) );
    frame->Show( true );
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Welcome to wxWidgets!" );
}

void MyFrame::OnExit(wxCommandEvent& event) {
    Close( true );
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}
void MyFrame::OnHello(wxCommandEvent& event) {
    wxLogMessage("Hello world from wxWidgets!");
}