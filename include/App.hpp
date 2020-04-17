#pragma once 
#include <QtWidgets>
#include "Engine.hpp"
#include "Log.hpp"

class App : public QMainWindow {
    Q_OBJECT
private:
    static const int appHeight = 1200;
    static const int appWidth  = 900;
    static App singletonApp;

    QPointer<QVBoxLayout> topLevelLayout;
    QPointer<QWidget> innerWidget;
    QPointer<QWidget> mainSection;
    QPointer<QWidget> bottomSection;
    QPointer<QPushButton> nextTurnButton;
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

