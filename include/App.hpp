#pragma once 
#include <QtWidgets>

class App : public QMainWindow {
    Q_OBJECT
private:
    static const int appHeight = 1200;
    static const int appWidth  = 900;
    static App singletonApp;
    static QPointer<QTextEdit> messagesWidget;

    QPointer<QVBoxLayout> topLevelLayout;
    QPointer<QWidget> innerWidget;
    QPointer<QWidget> mainSection;
    QPointer<QWidget> bottomSection;
    QPointer<QPushButton> nextTurnButton;
    QPointer<QTextEdit> textEditor;

private slots:
    void nextTurnButtonClicked();

public:
    explicit App(QWidget *parent = nullptr);
    int setup();

    static std::shared_ptr<App> getInstance();
    static void log(std::string newLogText);
};

