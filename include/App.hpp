#pragma once 
#include <QtWidgets>
#include "Engine.hpp"
#include "gui/MainSection.hpp"
#include "Log.hpp"

class App : public QMainWindow {
    Q_OBJECT
private:
    static const int appHeight = 900;
    static const int appWidth  = 1400;
    static App singletonApp;

    QPointer<QVBoxLayout> topLevelLayout;
    QPointer<QWidget> innerWidget;

    QPointer<MainSection> mainSection;
    QPointer<QTextEdit> textEditor;

    Engine engine = Engine::testSetup();

    void log(std::string newLogText);

    void setupLogger();
    void setupGUI();
    void setupGame();

private slots:
    void nextTurnButtonClicked();

public:
    explicit App(QWidget *parent = nullptr);
    int setup();
};

